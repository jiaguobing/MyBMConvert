// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _MSC_VER
#	define _FILE_OFFSET_BITS 64
#endif


#include "HIOUtilityPointCloud.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <limits>

#include "varray.h"
#include "vhash.h"
#include "vlist.h"
#include "tempfile_utils.h"

#include "HBaseView.h"
#include "HUtilityLocaleString.h"


//////////////////////////////////////////////////////////////////////////


#if _MSC_VER
static int big_fseek (FILE * file, int64_t offset, int origin)
{
	return _fseeki64(file, offset, origin);
}

static int64_t big_ftell (FILE * file)
{
	return _ftelli64(file);
}
#else
static int big_fseek (FILE * file, int64_t offset, int origin)
{
	return fseeko(file, offset, origin);
}

static int64_t big_ftell (FILE * file)
{
	return ftello(file);
}
#endif


//////////////////////////////////////////////////////////////////////////


#define POINT_LIST_CHUNK 5000000

// size of buffer for chunking ASCII and binary data
#define BUFFER_CHUNK_SIZE 131072

#define VALID_POINT		0x01
#define VALID_INTENSITY	0x02
#define VALID_RGB		0x04

class RGBColor {
public:
	float red, green, blue;

	RGBColor ( float red_in = 0.0f, float green_in = 0.0f, float blue_in = 0.0f ) {
		red = red_in;
		green = green_in;
		blue = blue_in;
	}

	RGBColor ( unsigned char red_in, unsigned char green_in, unsigned char blue_in ) {
		red = (float) red_in / 255.0f;
		green = (float) green_in / 255.0f;
		blue = (float) blue_in / 255.0f;
	}

	RGBColor ( RGBColor const & rgb_in ) {
		red = rgb_in.red;
		green = rgb_in.green;
		blue = rgb_in.blue;
	}
};

class PCPoint {
public:
	HPoint 			pt;
	HPoint 			pt_copy; // this is here to avoid allocating memory for bboxen in VBSP
	int 			intensity;
	RGBColor 		rgb_color;
	unsigned char 	bit_field;

	PCPoint ( float x = 0.0f, float y = 0.0f, float z = 0.0f, int intensity = 0, 
			  unsigned char r = 0, unsigned char g = 0, unsigned char b = 0 )
	{
		this->pt = HPoint(x, y, z);
		this->pt_copy = pt;
		this->intensity = intensity;
		this->rgb_color = RGBColor(r, g, b);
		this->bit_field = VALID_POINT | VALID_INTENSITY | VALID_RGB;
	}

	PCPoint ( PCPoint const & new_pt ) {
		pt = new_pt.pt;
		pt_copy = pt;
		intensity = new_pt.intensity;
		rgb_color = new_pt.rgb_color;
		bit_field = new_pt.bit_field;
	}
};

class PointPool {
protected:
	unsigned long 		m_max_array_size;
	unsigned long 		m_count;
	unsigned long 		m_current_node;
	VList<PCPoint *> * 	m_point_array_list;
public:
	PointPool () {
		m_point_array_list = new VList<PCPoint *>();
		m_max_array_size = 25000;
		m_count = 0;
		m_current_node = 0;
	}

	~PointPool () {
		m_point_array_list->MapFunction(FreeArrays, 0);
		delete m_point_array_list;
	}

	PCPoint * GetNewPoint ( PCPoint cpt ) {
		PCPoint *point_array = m_point_array_list->NthItem(m_current_node);
		if (point_array == 0) {
			point_array = new PCPoint [m_max_array_size];
			m_point_array_list->AddLast(point_array);
			m_count = 0;
		}
		else if (m_count == m_max_array_size) {
			m_current_node++;
			point_array = m_point_array_list->NthItem(m_current_node);
			if (point_array == 0) {
				point_array = new PCPoint [m_max_array_size];
				m_point_array_list->AddLast(point_array);
			}
			m_count = 0;
		}

		point_array[m_count++] = cpt;

		return &(point_array[m_count - 1]);
	}

	void Reset () {
		m_current_node = 0;
		m_count = 0;
	}

	static void FreeArrays ( PCPoint * point_array, void * user_data ) {
		UNREFERENCED(user_data);
		delete [] point_array;
	}
};

enum ReadWriteStatus {
	OK,
	ERROR,
	END_OF_FILE
};

class BufferedFile {
protected:
	FILE * m_fp;

public:
	BufferedFile ( FILE * fp_in ) {
		m_fp = fp_in;
	}

	virtual ~BufferedFile () {}
};

class BufferedWriter : public BufferedFile {
protected:
	char 	m_write_buffer[BUFFER_CHUNK_SIZE];
	size_t 	m_write_pos;

public:
	BufferedWriter ( FILE * fp_in ) : BufferedFile(fp_in) {
		m_write_pos = 0;
	}

	virtual ~BufferedWriter () {
		Flush();
	}

	ReadWriteStatus Write ( void * bytes, size_t num_bytes ) {
		size_t amount_written;

		if (m_write_pos + num_bytes > BUFFER_CHUNK_SIZE) {
			amount_written = fwrite(m_write_buffer, 1, m_write_pos, m_fp);
			if (amount_written == 0)
				return ERROR;
			m_write_pos = 0;
		}
		memcpy(m_write_buffer + m_write_pos, bytes, num_bytes);
		m_write_pos += num_bytes;

		return OK;
	}

	void Flush () {
		if (m_write_pos > 0) {
			fwrite(m_write_buffer, 1, m_write_pos, m_fp);
			m_write_pos = 0;
		}
	}
};

class BufferedReader : public BufferedFile {
protected:
	char 	m_read_buffer[BUFFER_CHUNK_SIZE];
	size_t 	m_read_pos;
	size_t 	m_amount_read;
	bool 	m_is_initialized;
	bool 	m_is_eof;

public:
	BufferedReader ( FILE * fp_in ) : BufferedFile(fp_in) {
		m_amount_read = 0;
		m_read_pos = 0;
		m_is_initialized = false;
		m_is_eof = false;
	}

	ReadWriteStatus Read ( void * bytes, long num_bytes ) {
		ReadWriteStatus status = OK;

		if (m_is_eof) {
			if (m_read_pos + num_bytes == m_amount_read)
				status = END_OF_FILE;
			else if (m_read_pos + num_bytes > m_amount_read)
				return ERROR;
		}
		else if (!m_is_initialized || m_read_pos + num_bytes > BUFFER_CHUNK_SIZE) {
			if (m_is_initialized) {
				memcpy(m_read_buffer, m_read_buffer + m_read_pos, BUFFER_CHUNK_SIZE - m_read_pos);
				m_read_pos = BUFFER_CHUNK_SIZE - m_read_pos;
			}
			m_amount_read = fread(m_read_buffer + m_read_pos, 1, BUFFER_CHUNK_SIZE - m_read_pos, m_fp);
			m_is_initialized = true;

			if (ferror(m_fp))
				return ERROR;

			m_amount_read += m_read_pos;
			if (m_amount_read < BUFFER_CHUNK_SIZE) {
				m_is_eof = true;
				if (m_read_pos + num_bytes == m_amount_read)
					status = END_OF_FILE;
			}

			m_read_pos = 0;
		}

		memcpy(bytes, m_read_buffer + m_read_pos, num_bytes);
		m_read_pos += num_bytes;

		return status;
	}

	int64_t FileSize () {
		int64_t curr_pos = big_ftell(m_fp);
		big_fseek(m_fp, 0, SEEK_END);
		int64_t file_size = big_ftell(m_fp);
		big_fseek(m_fp, curr_pos, SEEK_SET);
		return file_size;
	}
};

class BSPData {
public:
	class BadShell {
	protected:
		HC_KEY 	m_shell_key;
		int 	m_point_count;
		int 	m_max_shell_size;

	public:
		BadShell ( int max_shell_size_in ) {
			Reset();
			m_max_shell_size = max_shell_size_in;
		}

		void AddPoints ( int point_count_in, HPoint * point_list_in, RGBColor * colors = 0 ) {
			if (m_point_count + point_count_in > m_max_shell_size) {
				m_shell_key = INVALID_KEY;
				m_point_count = 0;
			}

			if (m_shell_key == INVALID_KEY)
				m_shell_key = HC_KInsert_Shell(point_count_in, point_list_in, 0, 0);
			else
				HC_Edit_Shell_Points(m_shell_key, m_point_count, 0, point_count_in, point_list_in);

			if (colors)
				HC_MSet_Vertex_Colors_By_Value(m_shell_key, "vertices", m_point_count, "RGB", point_count_in, colors);

			m_point_count += point_count_in;
		}

		void AddPoints ( int point_count_in, float * point_list_in, float * colors = 0 ) {
			if (m_point_count + point_count_in > m_max_shell_size) {
				m_shell_key = INVALID_KEY;
				m_point_count = 0;
			}

			if (m_shell_key == INVALID_KEY)
				m_shell_key = HC_KInsert_Shell(point_count_in, point_list_in, 0, 0);
			else
				HC_Edit_Shell_Points(m_shell_key, m_point_count, 0, point_count_in, point_list_in);

			if (colors)
				HC_MSet_Vertex_Colors_By_Value(m_shell_key, "vertices", m_point_count, "RGB", point_count_in, colors);

			m_point_count += point_count_in;
		}

		void Reset () {
			m_shell_key = INVALID_KEY;
			m_point_count = 0;
		}
	};

	BSPData ( HIOUtilityPointCloud * this_io_in, HPointCloudOptions * m_point_cloud_options_in = 0 ) {
		m_this_io = this_io_in;
		m_point_cloud_options = m_point_cloud_options_in;
		m_is_offset_bucket = false;
		if (m_point_cloud_options) {
			m_point_list = new HPoint [m_point_cloud_options->m_maxShellSize];
			m_colors = new RGBColor [m_point_cloud_options->m_maxShellSize];
		}
		else {
			m_point_list = 0;
			m_colors = 0;
		}

		int bucket_hash_size = 4096;
		if (m_point_cloud_options && m_point_cloud_options->m_numBuckets)
			bucket_hash_size = m_point_cloud_options->m_numBuckets;
		m_bucket_seg_hash = new VHash<int, HC_KEY>(bucket_hash_size);
		m_bucket_bad_shell_hash = new VHash<int, BadShell *>(bucket_hash_size);
		m_gray_scale = -1.0f;
		m_writer = 0;
		m_reader = 0;
	}

	~BSPData () {
		delete [] m_point_list;
		delete [] m_colors;
		delete m_bucket_seg_hash;
		if (m_bucket_bad_shell_hash) {
			m_bucket_bad_shell_hash->DeleteItems();
			delete m_bucket_bad_shell_hash;
		}
		delete m_writer;
		delete m_reader;
	}

	HIOUtilityPointCloud * 		m_this_io;
	HPointCloudOptions *   		m_point_cloud_options;
	bool 				   		m_is_offset_bucket;
	HPoint * 			   		m_point_list;
	RGBColor * 			   		m_colors;
	VHash<int, HC_KEY> *   		m_bucket_seg_hash;
	VHash<int, BadShell *> * 	m_bucket_bad_shell_hash;
	float 						m_gray_scale;
	int 						m_bucket;
	BufferedWriter * 			m_writer;
	BufferedReader * 			m_reader;
};

HIOUtilityPointCloud::HIOUtilityPointCloud () {}

HIOUtilityPointCloud::~HIOUtilityPointCloud () {}

char const * HIOUtilityPointCloud::GetInputTypesString () {
	return "pts,ptx,xyz";
}

void HIOUtilityPointCloud::RegisterInputHandlerTypes () {
	HIORegisterInputType("pts", this);
	HIORegisterInputType("ptx", this);
	HIORegisterInputType("xyz", this);
}


static unsigned long ReadPtxMetadata (FILE * file, char * buffer, int buffer_size, float * matrix_4x4) {
	// read in number of columns and rows
	unsigned long total_points = 1;
	for (int i = 0; i < 2; ++i) {
		unsigned long count;
		fgets(buffer, buffer_size, file);
		sscanf(buffer, "%lu", &count);
		total_points *= count;
	}

	// get translation, and rotation (discard - grabbed below)
	for (int i = 0; i < 4; ++i) {
		fgets(buffer, buffer_size, file);
	}

	// get the modeling matrix (duplicates of data above)
	for (int i = 0; i < 4; ++i) {
		fgets(buffer, buffer_size, file);
		float * const row = matrix_4x4 + 4 * i;
		HCLOCALE(sscanf(buffer, "%f%*c%f%*c%f%*c%f", row + 0, row + 1, row + 2, row + 3));
	}
	return total_points;
}


static char const * strchrs (char const * str, char const * cs, size_t max_count) {
	for (size_t i = 0; i < max_count && *str; ++i) {
		if (strchr(cs, *str)) {
			return str;
		}
		++str;
	}
	return 0;
}


/* 
 *	Run through the ASCII file, get an accurate bounding and min and max intensity values,
 *	  and generate a binary version for use in octree generation
 */
bool HIOUtilityPointCloud::Initialize ( const wchar_t * filename ) {
	m_file = NULL;
	m_bin_file = NULL;
	m_num_points = 0;

	m_file = wfopen(filename, L"rb");

	GenerateTempFileName(m_bin_file_name);
	m_bin_file = wfopen(m_bin_file_name, L"wb");

	if (m_file == NULL)
		return false;

	// initialize file size
	big_fseek(m_file, 0, SEEK_END);
	m_file_size = big_ftell(m_file);
	big_fseek(m_file, 0, SEEK_SET);

	m_min_intensity = 0;
	m_max_intensity = 0;
	m_std_bbox[0] = 0.0f; m_std_bbox[1] = 0.0f; m_std_bbox[2] = 0.0f;
	m_std_bbox[3] = 0.0f; m_std_bbox[4] = 0.0f; m_std_bbox[5] = 0.0f;

	char read_buffer[BUFFER_CHUNK_SIZE];
	assert(BUFFER_CHUNK_SIZE <= std::numeric_limits<long>::max());

	if (m_point_cloud_file_type == PTX) {
		m_num_points += ReadPtxMetadata(m_file, read_buffer, sizeof(read_buffer), m_matrix);
	}
	else if (m_point_cloud_file_type == PTS) {
		// read in the point count
		fgets(read_buffer, sizeof(read_buffer), m_file);
		sscanf(read_buffer, "%lu", &m_num_points);
	}
	else if (m_point_cloud_file_type == XYZ) {
		// nothing to be done -- no additional data
	}

	ReportInputProgress("Parsing text file...");

	BufferedWriter writer(m_bin_file);
	// placeholders for bounding info
	writer.Write(m_std_bbox, 6 * sizeof(float));
	// placeholders for min/max intensity
	writer.Write(&m_min_intensity, sizeof(int));
	writer.Write(&m_max_intensity, sizeof(int));

	size_t header_offset = 6 * sizeof(float) + 2 * sizeof(int);

	int64_t prev_pos = 0;
	int64_t curr_pos = 0;
	int64_t percent_byte_count = m_file_size / 10000;
	float inv_file_size = 1.0f / (float)m_file_size;

	bool initial_point = true;
	bool success = true;
	bool eof = false;
	bool no_more_reads = false;

	unsigned long line_no = 0;
	unsigned long read_point_count = 0;
	unsigned long valid_read_point_count = 0;

	char const * const new_line_chars = "\n\r";

	while (true) {
		size_t amount_read = fread(read_buffer, 1, sizeof(read_buffer), m_file);

		if (amount_read == 0 && feof(m_file))
			break;

		if (amount_read < sizeof(read_buffer))
			no_more_reads = true;

		size_t read_pos = 0;
		while (strchr(new_line_chars, read_buffer[read_pos])) {
			++read_pos;
		}
		while (read_pos < amount_read) {
			if (read_point_count == m_num_points) {
				size_t amount_unprocessed = amount_read - read_pos;
				if (m_point_cloud_file_type == PTX) {
					big_fseek(m_file, -static_cast<int64_t>(amount_unprocessed), SEEK_CUR);
					m_num_points += ReadPtxMetadata(m_file, read_buffer, sizeof(read_buffer), m_matrix);
					break;
				}
			}

			size_t line_len = 0;
			{
				size_t amount_unprocessed = amount_read - read_pos;
				char const * new_line_pos = strchrs(read_buffer + read_pos, new_line_chars, amount_unprocessed);
				if (new_line_pos) {
					while (new_line_pos < read_buffer + amount_read && strchr(new_line_chars, *new_line_pos)) {
						++new_line_pos;
					}
					line_len = new_line_pos - (read_buffer + read_pos);
					read_buffer[read_pos + line_len - 1] = '\0';
				}
				else {
					if (no_more_reads) {
						// fread does not null-terminate buffer, so make room for null-terminator
						assert(read_pos + 1 < amount_read);
						memmove(read_buffer, read_buffer + read_pos, amount_unprocessed);
						read_pos = 0;
						amount_read = amount_unprocessed;
						line_len = amount_unprocessed;
						read_buffer[amount_unprocessed] = '\0';
					}
					else {
						// fread buffer did not grab entire line. Rewind to line beginning and fread again
						size_t amount_unprocessed = amount_read - read_pos;
						big_fseek(m_file, -static_cast<int64_t>(amount_unprocessed), SEEK_CUR);
						break;
					}
				}
			}
			++line_no;

			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float intensity = 0.0f;
			int r = 0;
			int g = 0;
			int b = 0;
			int result;
			HCLOCALE(
				result = sscanf(read_buffer + read_pos, "%f%*c%f%*c%f%*c%f%*c%d%*c%d%*c%d", &x, &y, &z, &intensity, &r, &g, &b);
			);
			if (result == EOF) {
				ReportInputProgress(H_FORMAT_TEXT("ERROR! Failed to parse line # %lu\n%s", line_no, read_buffer));
				success = false;
				break;
			}
			if (!eof) {
				read_pos += line_len;
			}
			if (read_pos >= amount_read) {
				if (no_more_reads) {
					eof = true;
				}
			}

			++read_point_count;
			bool valid_point = false;
			PCPoint cpt(x, y, z, (int)intensity, (unsigned char)r, (unsigned char)g, (unsigned char)b);

			float const epsilon = 1e-8f;
			if (fabs(x) < epsilon && fabs(y) < epsilon && fabs(z) < epsilon && fabs(intensity - 0.5f) < epsilon &&
				r == 0.0f && g == 0.0f && b == 0.0f)
			{
				cpt.bit_field = (unsigned char)(~VALID_POINT & ~VALID_INTENSITY & ~VALID_RGB);
			}
			else {
				if (result == 3) {
					cpt.bit_field &= ~VALID_INTENSITY;
					cpt.bit_field &= ~VALID_RGB;
					valid_point = true;
				}
				else if (result == 4) {
					cpt.bit_field &= ~VALID_RGB;
					valid_point = true;
				}
				else if (result == 6) {
					cpt.bit_field &= ~VALID_INTENSITY;
					valid_point = true;
				}
				else if (result == 7)
					valid_point = true;
			}

			if (valid_point) {
				++valid_read_point_count;
				HPoint temp(x, y, z);
				// if we have a PTX file, apply the transformation matrix
				if (m_point_cloud_file_type == PTX)
					HC_Compute_Transformed_Points(1, &temp, m_matrix, &temp);
				cpt.pt = temp;

				if (m_point_cloud_file_type == PTX)
					intensity *= 1e6f;
				
				if (initial_point) {
					m_min_intensity = (int)intensity;
					m_max_intensity = (int)intensity;
					m_std_bbox[0] = cpt.pt.x; m_std_bbox[1] = cpt.pt.y; m_std_bbox[2] = cpt.pt.z;
					m_std_bbox[3] = cpt.pt.x; m_std_bbox[4] = cpt.pt.y; m_std_bbox[5] = cpt.pt.z;
					initial_point = false;
				}
				else {
					if ((int)intensity < m_min_intensity)
						m_min_intensity = (int)intensity;
					if ((int)intensity > m_max_intensity)
						m_max_intensity = (int)intensity;
					if (cpt.pt.x < m_std_bbox[0])
						m_std_bbox[0] = cpt.pt.x;
					if (cpt.pt.y < m_std_bbox[1])
						m_std_bbox[1] = cpt.pt.y;
					if (cpt.pt.z < m_std_bbox[2])
						m_std_bbox[2] = cpt.pt.z;
					if (cpt.pt.x > m_std_bbox[3])
						m_std_bbox[3] = cpt.pt.x;
					if (cpt.pt.y > m_std_bbox[4])
						m_std_bbox[4] = cpt.pt.y;
					if (cpt.pt.z > m_std_bbox[5])
						m_std_bbox[5] = cpt.pt.z;
				}

				writer.Write(&cpt.pt.x, sizeof(float));
				writer.Write(&cpt.pt.y, sizeof(float));
				writer.Write(&cpt.pt.z, sizeof(float));
				writer.Write(&cpt.intensity, sizeof(int));
				writer.Write(&cpt.rgb_color.red, sizeof(float));
				writer.Write(&cpt.rgb_color.green, sizeof(float));
				writer.Write(&cpt.rgb_color.blue, sizeof(float));
				writer.Write(&cpt.bit_field, sizeof(unsigned char));
			}

			curr_pos += line_len;
			if (curr_pos - prev_pos >= percent_byte_count) {
				float progress = (float)curr_pos * inv_file_size;
				ReportInputPercentProgress(progress);
				prev_pos = curr_pos;
			}

			if (eof)
				break;
		}

		if (eof || !success)
			break;
	}
	writer.Flush();
	ReportInputPercentProgress(1.0f);

	if (m_point_cloud_file_type == PTS && valid_read_point_count != m_num_points)
		ReportInputProgress("WARNING! Number of points reported by file doesn't match number found in the file.");

	if (success) {
		// go back to beginning, add actual bounding box and min/max intensities
		big_fseek(m_bin_file, 0, SEEK_SET);
		fwrite(m_std_bbox, 1, 6 * sizeof(float), m_bin_file);
		fwrite(&m_min_intensity, 1, sizeof(int), m_bin_file);
		fwrite(&m_max_intensity, 1, sizeof(int), m_bin_file);

		fclose(m_bin_file);
		m_bin_file = NULL;
		fclose(m_file);
		m_file = NULL;

		ReportInputProgress("Done.");

		m_file = wfopen(m_bin_file_name, L"rb");
		// reinitialize file size
		big_fseek(m_file, 0, SEEK_END);
		m_file_size = big_ftell(m_file);

		// position file cursor at the right spot
		big_fseek(m_file, (int64_t)header_offset, SEEK_SET);
	}
	else {
		if (m_file) {
			fclose(m_file);
			m_file = NULL;
		}
		if (m_bin_file) {
			fclose(m_bin_file);
			m_bin_file = NULL;
		}
	}

	return success;
}

float HIOUtilityPointCloud::GetScaledIntensity ( PCPoint * cpt ) {
	float scaled_intensity;
	if (m_point_cloud_file_type == PTX)
		scaled_intensity = (float)cpt->intensity / 1e6f;
	else {
		if (m_point_cloud_options->m_lowIntensityValue < 0)
			scaled_intensity = (float)(cpt->intensity + abs(m_point_cloud_options->m_lowIntensityValue));
		else
			scaled_intensity = (float)(cpt->intensity - m_point_cloud_options->m_lowIntensityValue);
		// this should not be a division by zero
		scaled_intensity /= (float)(m_point_cloud_options->m_highIntensityValue - m_point_cloud_options->m_lowIntensityValue);
	}

	return scaled_intensity;
}

/* 
 *	Using the binary file generated by the Initialize function, generate an octree
 *	  - the points are handled in POINT_LIST_CHUNK size chunks
 */
HFileInputResult HIOUtilityPointCloud::SpatiallySortPointCloud ( HC_KEY seg_key ) {
	if (!feof(m_file)) {
		ReportInputProgress("Generating spatially segregated shells...");
		BSPData bsp_data(this, m_point_cloud_options);
		VHash<int, VBSP<PCPoint *> *> * gray_point_bsp_hash = new VHash<int, VBSP<PCPoint *> *>();
		VBSP<PCPoint *> * gray_point_bsp = 0;
		VBSP<PCPoint *> * color_point_bsp = 0;
		PointPool * point_pool = new PointPool();
		unsigned long point_count = 0;
		bool has_rgb_points = false;

		wchar_t dump_file[MVO_BUFFER_SIZE];
		GenerateTempFileName(dump_file);
		FILE * temp_fp = wfopen(dump_file, L"wb");
		bsp_data.m_writer = new BufferedWriter(temp_fp);

		ReportInputPercentProgress(big_ftell(m_file) / (float)m_file_size);

		int64_t prev_pos = 0;
		int64_t curr_pos = 0;
		int64_t percent_byte_count = m_file_size / 10000;
		float inv_file_size = 1.0f / (float)m_file_size;

		long read_pos = 0, float_size = sizeof(float), uchar_size = sizeof(unsigned char), int_size = sizeof(int), 
			 cpt_size = 3 * float_size + int_size + 3 * float_size + uchar_size;
		bool eof = false;

		BufferedReader reader(m_file);
		char * cpt_buffer = new char[cpt_size];
		ReadWriteStatus status;
		unsigned long valid_point_count = 0;
		while (true) {
			status = reader.Read(cpt_buffer, cpt_size);
			
			if (status == ERROR)
				break;

			if (status == END_OF_FILE)
				eof = true;

			read_pos = 0;

			PCPoint cpt;
			memcpy(&cpt.pt.x, cpt_buffer + read_pos, float_size);
			read_pos += float_size;
			memcpy(&cpt.pt.y, cpt_buffer + read_pos, float_size);
			read_pos += float_size;
			memcpy(&cpt.pt.z, cpt_buffer + read_pos, float_size);
			read_pos += float_size;
			memcpy(&cpt.intensity, cpt_buffer + read_pos, int_size);
			read_pos += int_size;
			memcpy(&cpt.rgb_color.red, cpt_buffer + read_pos, float_size);
			read_pos += float_size;
			memcpy(&cpt.rgb_color.green, cpt_buffer + read_pos, float_size);
			read_pos += float_size;
			memcpy(&cpt.rgb_color.blue, cpt_buffer + read_pos, float_size);
			read_pos += float_size;
			memcpy(&cpt.bit_field, cpt_buffer + read_pos, uchar_size);
			read_pos += uchar_size;

			if (cpt.bit_field & VALID_POINT) {
				valid_point_count++;
				point_count++;
				PCPoint *new_cpt = point_pool->GetNewPoint(cpt);
				if (cpt.bit_field & VALID_RGB) {
					if (!has_rgb_points)
						color_point_bsp = new VBSP<PCPoint *>(m_std_bbox, -1, m_point_cloud_options->m_maxShellSize, true, 0.50f, 0.50f);
					has_rgb_points = true;
					color_point_bsp->Insert(new_cpt, (float *)new_cpt);
				}
				else if (cpt.bit_field & VALID_INTENSITY) {
					int new_bucket;
					if (m_point_cloud_options->m_numBuckets)
						new_bucket = (int)(GetScaledIntensity(&cpt) * m_point_cloud_options->m_numBuckets);
					else
						new_bucket = cpt.intensity;

					if (gray_point_bsp_hash->LookupItem(new_bucket, &gray_point_bsp) == VHASH_STATUS_FAILED) {
						gray_point_bsp = new VBSP<PCPoint *>(m_std_bbox, -1, m_point_cloud_options->m_maxShellSize, true, 0.50f, 0.50f);
						gray_point_bsp_hash->InsertItem(new_bucket, gray_point_bsp);
					}
					PCPoint *new_cpt = point_pool->GetNewPoint(cpt);
					gray_point_bsp->Insert(new_cpt, (float *)new_cpt);
				}
				else {
					if (gray_point_bsp_hash->LookupItem(0, &gray_point_bsp) == VHASH_STATUS_FAILED) {
						gray_point_bsp = new VBSP<PCPoint *>(m_std_bbox, -1, m_point_cloud_options->m_maxShellSize, true, 0.50f, 0.50f);
						gray_point_bsp_hash->InsertItem(0, gray_point_bsp);
					}
					gray_point_bsp->Insert(new_cpt, (float *)new_cpt);
				}
			}

			curr_pos += cpt_size;
			if (curr_pos - prev_pos >= percent_byte_count) {
				float progress = (float)curr_pos * inv_file_size;
				ReportInputPercentProgress(progress);
				prev_pos = curr_pos;
			}

			if (point_count == POINT_LIST_CHUNK || eof) {
				bool reset_num_buckets = false;
				if (m_point_cloud_options->m_numBuckets == 0) {
					reset_num_buckets = true;
					if (m_point_cloud_file_type == PTX)
						m_point_cloud_options->m_numBuckets = 1000000;
					else {
						m_point_cloud_options->m_numBuckets = m_point_cloud_options->m_highIntensityValue - m_point_cloud_options->m_lowIntensityValue;
						bsp_data.m_is_offset_bucket = true;
					}	
				}

				if (has_rgb_points)
					WriteShellsToFile(color_point_bsp, -1, &bsp_data);
				gray_point_bsp_hash->MapFunction(WriteShellsToFile, &bsp_data);

				if (has_rgb_points)
					DeleteBSPs(color_point_bsp, 0, 0);
				has_rgb_points = false;

				gray_point_bsp_hash->MapFunction(DeleteBSPs, 0);
				delete gray_point_bsp_hash;
				if (eof)
					gray_point_bsp_hash = 0;
				else
					gray_point_bsp_hash = new VHash<int, VBSP<PCPoint *> *>();

				delete point_pool;
				if (eof)
					point_pool = 0;
				else
					point_pool = new PointPool();

				point_count = 0;
				if (reset_num_buckets)
					m_point_cloud_options->m_numBuckets = 0;
			}

			if (eof) {
				fclose(m_file);
				m_file = NULL;
				break;
			}
		}
		delete bsp_data.m_writer;
		bsp_data.m_writer = 0;
		fclose(temp_fp);

		delete [] cpt_buffer;
		if (point_pool) {
			delete point_pool;
			point_pool = 0;
		}
		if (gray_point_bsp_hash) {
			delete gray_point_bsp_hash;
			gray_point_bsp_hash = 0;
		}

		ReportInputPercentProgress(1.0f);
		ReportInputProgress("Done.");

		wremove(m_bin_file_name);

		ReportInputProgress("Inserting shells...");
		ReportInputPercentProgress(0.0f);
		temp_fp = wfopen(dump_file, L"rb");
		big_fseek(temp_fp, 0, SEEK_SET);
		bsp_data.m_reader = new BufferedReader(temp_fp);
		HC_Open_Segment_By_Key(seg_key);{
			bool success = InsertShells(&bsp_data);
			fclose(temp_fp);
			if (!success) {
				ReportInputProgress("ERROR!  File is corrupt.");
				ReportInputPercentProgress(1);
				return InputFail;
			}
		}HC_Close_Segment();
		delete bsp_data.m_reader;
		bsp_data.m_reader = 0;
		wremove(dump_file);
		
		ReportInputPercentProgress(1.0f);
		ReportInputProgress("Done.");

		// set up visibility of markers
		HC_Open_Segment_By_Key(seg_key);{
			HC_Set_Rendering_Options("color interpolation = markers");
			if (m_pHView) {
				HC_Open_Segment_By_Key(m_pHView->GetSceneKey());{
					HC_Set_Visibility("vertices = on");
				}HC_Close_Segment();
			}
			else
				HC_Set_Visibility("vertices = on");
			HC_Set_Marker_Symbol(".");
			HC_Set_Marker_Size(0);
		}HC_Close_Segment();
	} 
	else {
		ReportInputPercentProgress(1);
		ReportInputProgress(H_FORMAT_TEXT("ERROR! Couldn't open input file..."));
		return InputBadFileName;
	}
	return InputOK;
}

void HIOUtilityPointCloud::WriteShellsToFile ( VBSP<PCPoint *> * bsp, int bucket, const void * const user_data ) {
	BSPData *bsp_data = (BSPData *)user_data;

	if (bucket >= 0) {
		if (bsp_data->m_is_offset_bucket) {
			if (bsp_data->m_point_cloud_options->m_lowIntensityValue < 0)
				bucket += abs(bsp_data->m_point_cloud_options->m_lowIntensityValue);
			else
				bucket -= bsp_data->m_point_cloud_options->m_lowIntensityValue;
		}

		bsp_data->m_gray_scale = (float)bucket / (float)(bsp_data->m_point_cloud_options->m_numBuckets);

		HC_KEY bucket_seg_key;
		if (bsp_data->m_bucket_seg_hash->LookupItem(bucket, &bucket_seg_key) == VHASH_STATUS_FAILED) {
			bucket_seg_key = HC_KOpen_Segment("");
			HC_Set_Color(H_FORMAT_TEXT("r = %g g = %g b = %g", bsp_data->m_gray_scale, bsp_data->m_gray_scale, bsp_data->m_gray_scale));
			HC_Close_Segment();
			bsp_data->m_bucket_seg_hash->InsertItem(bucket, bucket_seg_key);
		}
	}
	else {
		bsp_data->m_gray_scale = -1.0f;
	}

	bsp_data->m_bucket = bucket;
	bsp->PerNodeMapFunction(GetShellsFromBSP, bsp_data);
}

VBSPStatus HIOUtilityPointCloud::GetShellsFromBSP ( PCPointContainer * container, void * user_data ) {
	BSPData *bsp_data = (BSPData *)user_data;
	size_t int_size = sizeof(int), float_size = sizeof(float);

	// write out bucket, number of points
	bsp_data->m_writer->Write(&(bsp_data->m_bucket), int_size);
	int container_count = container->Count();
	bsp_data->m_writer->Write(&container_count, int_size);

	// write out points
	for (PCPointContainer::iterator i = container->begin(); i != container->end(); ++i) {
		bsp_data->m_writer->Write(&(*i)->GetItem()->pt.x, float_size);
		bsp_data->m_writer->Write(&(*i)->GetItem()->pt.y, float_size);
		bsp_data->m_writer->Write(&(*i)->GetItem()->pt.z, float_size);
	}

	// write out RGB values (if necessary)
	if (bsp_data->m_bucket < 0) {
		RGBColor temp_rgb;
		float rgb_val[3];
		for (PCPointContainer::iterator i = container->begin(); i != container->end(); ++i) {
			temp_rgb = (*i)->GetItem()->rgb_color;
			rgb_val[0] = temp_rgb.red;
			rgb_val[1] = temp_rgb.green;
			rgb_val[2] = temp_rgb.blue;
			bsp_data->m_writer->Write(rgb_val, 3 * float_size);
		}
	}

	return VBSP_STATUS_CONTINUE;
}

bool HIOUtilityPointCloud::InsertShells ( BSPData * bsp_data ) {
	int64_t file_size = bsp_data->m_reader->FileSize();

	int64_t prev_pos = 0;
	int64_t curr_pos = 0;
	int64_t percent_byte_count = file_size / 10000;
	float inv_file_size = 1.0f / (float)file_size;

	int bucket;
	int point_list_len = 0;
	float *point_list = new float [3 * bsp_data->m_point_cloud_options->m_maxShellSize];
	float *colors = new float [3 * bsp_data->m_point_cloud_options->m_maxShellSize];

	ReadWriteStatus status = OK;
	bool has_rgb_colors;
	HC_KEY shell_key;
	unsigned long total_point_count = 0;
	while (true) {
		has_rgb_colors = false;
		bsp_data->m_reader->Read(&bucket, sizeof(int));

		HC_KEY bucket_seg_key;
		if (bucket < 0) {
			has_rgb_colors = true;
			bucket_seg_key = HC_KCreate_Segment("color point cloud");
		}
		else if (bsp_data->m_bucket_seg_hash->LookupItem(bucket, &bucket_seg_key) == VHASH_STATUS_FAILED) {
			// we shouldn't hit this
			delete [] point_list;
			delete [] colors;
			return false;
		}

		HC_Open_Segment_By_Key(bucket_seg_key);{
			bsp_data->m_reader->Read(&point_list_len, sizeof(int));

			curr_pos += 2 * sizeof(int);

			total_point_count += point_list_len;
			for (int i = 0; i < point_list_len; ++i) {
				status = bsp_data->m_reader->Read(&(point_list[3*i]), 3 * sizeof(float));

				curr_pos += 3 * sizeof(float);
				if (curr_pos - prev_pos >= percent_byte_count) {
					float progress = (float)curr_pos * inv_file_size;
					ReportInputPercentProgress(progress);
					prev_pos = curr_pos;
				}
			}

			if (has_rgb_colors) {
				for (int i = 0; i < point_list_len; ++i) {
					status = bsp_data->m_reader->Read(&(colors[3*i]), 3 * sizeof(float));

					curr_pos += 3 * sizeof(float);
					if (curr_pos - prev_pos >= percent_byte_count) {
						float progress = (float)curr_pos * inv_file_size;
						ReportInputPercentProgress(progress);
						prev_pos = curr_pos;
					}
				}
			}

			if (point_list_len > bsp_data->m_point_cloud_options->m_minShellSize) {				
				shell_key = HC_KInsert_Shell(point_list_len, point_list, 0, 0);
				if (has_rgb_colors)
					HC_MSet_Vertex_Colors_By_Value(shell_key, "vertices", 0, "RGB", point_list_len, colors);
			}
			else {
				BSPData::BadShell * bad_shell = 0;
				if (bsp_data->m_bucket_bad_shell_hash->LookupItem(bucket, &bad_shell) == VHASH_STATUS_FAILED) {
					bad_shell = new BSPData::BadShell(bsp_data->m_point_cloud_options->m_maxShellSize);
					bsp_data->m_bucket_bad_shell_hash->InsertItem(bucket, bad_shell);
				}
				HC_Open_Segment("bad_shells");{
					if (has_rgb_colors)
						bad_shell->AddPoints(point_list_len, point_list, colors);
					else
						bad_shell->AddPoints(point_list_len, point_list);
				}HC_Close_Segment();
			}
		}HC_Close_Segment();

		if (status == END_OF_FILE)
			break;
	}

	delete [] point_list;
	delete [] colors;

	return true;
}

void HIOUtilityPointCloud::DeleteBSPs ( VBSP<PCPoint *> * bsp, int bucket, const void * const user_data ) {
	UNREFERENCED(bucket);
	UNREFERENCED(user_data);

	delete bsp;
}

HFileInputResult HIOUtilityPointCloud::FileInputByKey ( const wchar_t * filename, HC_KEY key, HInputHandlerOptions * options ) {
	ReportInputPercentProgress(0);
	ReportInputProgress("Starting file load...");
	if (!filename) {
		ReportInputPercentProgress(1);
		ReportInputProgress("ERROR! No filename...");
		return InputBadFileName;
	}

	if (key == INVALID_KEY) {
		ReportInputPercentProgress(1);
		ReportInputProgress("ERROR! No segment...");
		return InputFail;
	}

	m_pHView = options->m_pHBaseView;
	if (m_pHView == 0) {
		ReportInputProgress("WARNING! HBaseView pointer is NULL.  Marker visibility will be set in segment passed to this function.");
	}

	bool free_m_point_cloud_options = false;
	m_point_cloud_options = (HPointCloudOptions *)options->m_pExtendedData;
	if (m_point_cloud_options == 0) {
		free_m_point_cloud_options = true;
		m_point_cloud_options = new HPointCloudOptions();
		ReportInputProgress("Using default HPointCloudOptions values.");
	}

	const wchar_t *extension = wcsrchr(filename, L'.');
	extension++;

	m_point_cloud_file_type = UNKNOWN;
	if (wcsieq(extension, L"pts"))
		m_point_cloud_file_type = PTS;
	else if (wcsieq(extension, L"ptx"))
		m_point_cloud_file_type = PTX;
	else if (wcsieq(extension, L"xyz"))
		m_point_cloud_file_type = XYZ;
	else if (extension == 0)
		m_point_cloud_file_type = NONE;

	HFileInputResult retval = InputFail;
	
	if (m_point_cloud_file_type != UNKNOWN && m_point_cloud_file_type != NONE) {
		if (Initialize(filename)) {
			if (m_point_cloud_options->m_maxShellSize <= 0)
				ReportInputProgress("ERROR! Must use a positive maximum shell size.");
			else if (m_point_cloud_options->m_minShellSize < 0)
				ReportInputProgress("ERROR! Must use a non-negative minimum shell size.");
			else if (m_point_cloud_options->m_maxShellSize <= m_point_cloud_options->m_minShellSize)
				ReportInputProgress("ERROR! Max shell size must be strictly greater than minimum shell size.");
			else if (m_point_cloud_options->m_numBuckets < 0)
				ReportInputProgress("ERROR! Must use a non-negative number of buckets.");
			else {
				if (m_point_cloud_options->m_highIntensityValue == m_point_cloud_options->m_lowIntensityValue) {
					m_point_cloud_options->m_highIntensityValue = m_max_intensity;
					m_point_cloud_options->m_lowIntensityValue = m_min_intensity;
				}
				
				retval = SpatiallySortPointCloud(key);
			}
		}
		else
			ReportInputProgress("Aborting import (invalid or missing file).");
	}

	ReportInputPercentProgress(1);
	ReportInputProgress("Finished file load.");
	if (free_m_point_cloud_options)
		delete m_point_cloud_options;
	return retval;
}
