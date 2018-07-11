// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HIOUtilityPly.h"
#include "HTools.h"
#include "vhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#undef I2V
#define I2V(x) ((void *)(MVO_POINTER_SIZED_INT)(x))

#define SWAP32(i) (\
	(((i) >> 24) & 0x000000ff) |\
	(((i) & 0x00ff0000) >> 8) |\
	(((i) & 0x0000ff00) << 8) |\
	((i) << 24) \
	)


bool HIOUtilityPly::write_one_int( FILE *f, int val ) 
{
	if( !m_binary ) {
		fprintf( f, "%d ", val );
	}
	else if( m_bigendian ) {
#if MVO_BIGENDIAN
		fwrite( &val, 4, 1, f );
#else
		int swapped = SWAP32(val);
		fwrite( &swapped, 4, 1, f );
#endif
	}
	else {
#if MVO_BIGENDIAN
		int swapped = SWAP32(val);
		fwrite( &swapped, 4, 1, f );
#else
		fwrite( &val, 4, 1, f );
#endif
	}
	return true;
}


bool HIOUtilityPly::write_one_byte( FILE *f, unsigned char val ) 
{
	if( !m_binary ) {
		fprintf( f, "%d ", val );
	}
	else {
		fwrite( &val, 1, 1, f );
	}
	return true;
}



bool HIOUtilityPly::write_one_float( FILE *f, float val ) 
{
	if( m_binary ) {
		float temp = val;
		return write_one_int( f, *((int *) &temp ) );
	}
	else {
		HCLOCALE(fprintf( f, "%f ", val ));
		return true;
	}
}


bool HIOUtilityPly::read_one_int( FILE *f, int *val ) 
{
	if( !m_binary ) {
		fscanf( f, "%d", val );
	}
	else if( m_bigendian ) {
#if MVO_BIGENDIAN
		fread( val, 4, 1, f );
#else
		int swapped;
		fread( &swapped, 4, 1, f );
		*val = SWAP32(swapped);
#endif
	}
	else {
#if MVO_BIGENDIAN
		int swapped;
		fread( &swapped, 4, 1, f );
		*val = SWAP32(swapped);
#else
		fread( val, 4, 1, f );
#endif
	}
	return true;
}


bool HIOUtilityPly::read_one_byte( FILE *f, unsigned char *val ) 
{
	if( !m_binary ) {
		int i;
		fscanf( f, "%d", &i );
		*val = (unsigned char)i;
	}
	else {
		fread( val, 1, 1, f );
	}
	return true;
}



bool HIOUtilityPly::read_one_float( FILE *f, float *val ) 
{
	if( m_binary ) {
		return read_one_int( f, (int *)val );
	}
	else {
		int num_matched;

		//no HCLOCALE here.  This is an inner loop function, so we wrap
		//it around the caller
		num_matched = fscanf(f,"%f",val);
		return num_matched != 0;
	}
}


/*
 NOTE: the following is what you might expect to see in a typical ply header
	ply
	format binary_big_endian 1.0
	element vertex 507528
	property float x
	property float y
	property float z
	property uchar red
	property uchar green
	property uchar blue
	property float nx
	property float ny
	property float nz
	element face 747948
	property list uchar int vertex_indices
	property uchar red
	property uchar green
	property uchar blue
	end_header
*/


#define ELEMENT_UNKNOWN			0
#define ELEMENT_VERTEX			1
#define ELEMENT_FACE			2
#define END_ELEMENT_VERTEX		101
#define END_ELEMENT_FACE		102
#define PROPERTY_X			3
#define PROPERTY_Y			4
#define PROPERTY_Z			5
#define PROPERTY_NX			6
#define PROPERTY_NY			7
#define PROPERTY_NZ			8
#define PROPERTY_R			9
#define PROPERTY_G			10
#define PROPERTY_B			11
#define PROPERTY_FACE_LIST		12
#define PROPERTY_FLOAT_UNKNOWN  1000

bool HIOUtilityPly::read_ply_header( FILE *f, int *point_count, int *face_count, int *opcodes )
{
	char line[MVO_BUFFER_SIZE];
	char *ptr;
	int element_type = ELEMENT_UNKNOWN;
	int *op = opcodes;

	*point_count = -1;
	*face_count = -1;
	m_vertex_colors = m_vertex_normals = m_face_colors = false;

	memset(line, 0, MVO_BUFFER_SIZE);
	fgets( line, MVO_BUFFER_SIZE, f );
	while( !feof( f ) ) {
		if( !strncmp( line, "element", 7 )) {
			//terminate the old element (if there was one)
			if( element_type == ELEMENT_VERTEX )
				*op++ = END_ELEMENT_VERTEX;
			else if( element_type == ELEMENT_FACE )
				*op++ = END_ELEMENT_FACE;

			if ((ptr = strstr( line, "vertex" )) != 0 ) {
				ptr += strlen( "vertex" );
				while( *ptr == ' ' )
					ptr++;
				*point_count = atoi( ptr );
				element_type = ELEMENT_VERTEX;
			}
			else if ((ptr = strstr( line, "face" )) != 0 ) {
				ptr += strlen( "face" );
				while( *ptr == ' ' )
					ptr++;
				*face_count = atoi( ptr );
				element_type = ELEMENT_FACE;
			}
			else {
				element_type = ELEMENT_UNKNOWN;
			}
			*op++ = element_type;
		}
		else if( !strncmp( line, "format", 6 )) {
			if( strstr( line, "binary_big_endian" )) {
				m_binary = true;
				m_bigendian = true;
			}
			else if( strstr( line, "binary_little_endian" )) {
				m_binary = true;
				m_bigendian = false;
			}
			else if( strstr( line, "ascii" )) {
				m_binary = false;
				m_bigendian = false;
			}
		}
		else if( !strncmp( line, "property", 8 )) {
			if (element_type == ELEMENT_VERTEX) {
				if( strstr( line, "float x" ) || strstr( line, "float32 x" ) ) {
					*op++ = PROPERTY_X;					
				}
				else if( strstr( line, "float y" ) || strstr( line, "float32 y" ) ) {
					*op++ = PROPERTY_Y;					
				}
				else if( strstr( line, "float z" ) || strstr( line, "float32 z" ) ) {
					*op++ = PROPERTY_Z;					
				}
				else if( strstr( line, "uchar red" )) {
					*op++ = PROPERTY_R;	
					m_vertex_colors = true;				
				}
				else if( strstr( line, "uchar green" )) {
					*op++ = PROPERTY_G;	
					m_vertex_colors = true;				
				}
				else if( strstr( line, "uchar blue" )) {
					*op++ = PROPERTY_B;	
					m_vertex_colors = true;				
				}
				else if( strstr( line, "float nx" ) || strstr( line, "float32 nx" )) {
					*op++ = PROPERTY_NX;
					m_vertex_normals = true;
				}
				else if( strstr( line, "float ny" ) || strstr( line, "float32 ny" )) {
					*op++ = PROPERTY_NY;
					m_vertex_normals = true;
				}
				else if( strstr( line, "float nz" ) || strstr( line, "float32 nz" )) {
					*op++ = PROPERTY_NZ;
					m_vertex_normals = true;
				}
				else if(strstr( line, "float")){
					*op++ = PROPERTY_FLOAT_UNKNOWN;
				}
			}
			else if (element_type == ELEMENT_FACE) {
				if( strstr( line, "vertex_indices" ))
					*op++ = PROPERTY_FACE_LIST;
				if( strstr( line, "uchar red" )) {
					*op++ = PROPERTY_R;	
					m_face_colors = true;
				}
				if( strstr( line, "uchar green" )) {
					*op++ = PROPERTY_G;	
					m_face_colors = true;
				}
				if( strstr( line, "uchar blue" )) {
					*op++ = PROPERTY_B;	
					m_face_colors = true;
				}
			}
		}
		else if( !strncmp( line, "end_header", 10 )) {
			//terminate the old element (if there was one)
			if( element_type == ELEMENT_VERTEX )
				*op++ = END_ELEMENT_VERTEX;
			else if( element_type == ELEMENT_FACE )
				*op++ = END_ELEMENT_FACE;
			*op++ = 0;
			break;
		}
		memset(line, 0, MVO_BUFFER_SIZE);
		fgets( line, MVO_BUFFER_SIZE, f );
	}
	if( *point_count < 0 )
		return false;
	if( *face_count < 0 )
		return false;
	return true;
}

float edge_length( HPoint *p1, HPoint *p2 ) {
	return sqrt( 
		((p1->x - p2->x) * (p1->x - p2->x)) + 
		((p1->y - p2->y) * (p1->y - p2->y)) + 
		((p1->z - p2->z) * (p1->z - p2->z))
	);
}


bool HIOUtilityPly::write_ply_header( FILE *f, int point_count, int face_count, HPoint *min, HPoint *max )
{
	if( point_count < 0 )
		return false;
	if( face_count < 0 )
		return false;

	fputs( "ply\n", f );
	if( m_binary ) {
		if( m_bigendian )
			fputs( "format binary_big_endian 1.0\n", f );
		else
			fputs( "format binary_little_endian 1.0\n", f );
	}
	else
		fputs( "format ascii 1.0\n", f );
	fputs( "comment created by HOOPS/MVO\n", f );
	HCLOCALE(fprintf( f, "comment bounding: (%f %f %f) to (%f %f %f)\n", min->x, min->y, min->z, max->x, max->y, max->z ));
	HCLOCALE(fprintf( f, "comment bounding diagonal length: %f\n", edge_length( min, max ) ));
	fprintf( f, "element vertex %d\n", point_count );
	fputs( "property float x\n", f );
	fputs( "property float y\n", f );
	fputs( "property float z\n", f );
	fprintf( f, "element face %d\n", face_count );
	fputs( "property list uchar int vertex_indices\n", f );
	fputs( "end_header\n", f );

	return true;
}


HFileInputResult HIOUtilityPly::FileInputByKey(const wchar_t *FileName, HC_KEY key, HInputHandlerOptions * options) 
{
	UNREFERENCED(options);

	FILE *f = NULL;
	float *points = 0;
	int *face_list = 0;
	float *vertex_colors = 0, *vertex_normals = 0, *face_colors = 0;
	int i, j, point_count, face_count, face_list_length, face_list_used, count;
	HFileInputResult status = InputOK;
	unsigned char byte;
	HC_KEY shell_key;
	int opcodes[64];
	int *op, *save_op;
	Color_Node *cursor = 0, *temp_node = 0;
	VHash<int, Color_Node *> face_color_hash, vertex_color_hash;
	int color_hash_key = 0;
	VHash<int, Color_Node *>::PairList *vertex_color_list, *face_color_list; 
	VHash<int, Color_Node *>::Pair *v_c_l_cursor, *f_c_l_cursor, *max_cursor = NULL;
	char saved_locale[4096] = {""};
	char const * internal_locale_buffer;

	if(!FileName) 
		return InputBadFileName;
	
	if(key == INVALID_KEY) 
		return InputFail;

	SetStartingInput();

	f = wfopen(FileName, L"r");  // open the file as ASCII

	if (f == NULL) 
		return InputBadFileName;

	//take care of locale handling so that we don't have to keep calling
	//HCLOCALE on the inner loop
#ifdef _MSC_VER
		int was_type = _configthreadlocale(0);
		if (was_type != _ENABLE_PER_THREAD_LOCALE)
			_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
#endif
    internal_locale_buffer = setlocale(LC_ALL, 0);
	if (internal_locale_buffer != 0)
		strcpy(saved_locale, internal_locale_buffer);
	setlocale(LC_ALL, "C");


	if( !read_ply_header( f, &point_count, &face_count, opcodes ) ||
		point_count <= 0) {
			status = InputFail;
			goto cleanup;
	}

	if( m_binary ) {
		// close and reopen the file as binary
		fclose( f );
		f = wfopen(FileName, L"rb");
		if (f == NULL) 
			return InputBadFileName; //shouldn't ever happen, since we already successfully opened as ascii
		//re-read the header to reposition the file pointer
		read_ply_header( f, &point_count, &face_count, opcodes );
	}

	/* allocate */
	if( face_count > 0 ) {
		face_list_length = 5 * face_count + 2;
		face_list = (int *) malloc (face_list_length * sizeof( int ));
	}
	else {
		face_list_length = 0;
	}
	points = (float *) malloc (3 * point_count * sizeof( float ));
	if (m_vertex_colors)
		vertex_colors = (float *) malloc (3 * point_count * sizeof( float ));
	if (m_vertex_normals)
		vertex_normals = (float *) malloc (3 * point_count * sizeof( float ));
	if (m_face_colors)
		face_colors = (float *) malloc (3 * face_count * sizeof( float ));

	save_op = op = opcodes;
	/* read the points array */
	for( i = 0 ; i < point_count ; i++ ) {
		op = save_op;
		if( *op++ != ELEMENT_VERTEX ||
			feof( f ) ) {
				status = InputFail;
				goto cleanup;
		}
		do {
			switch( *op++ ) {
				case PROPERTY_X:
					if( !read_one_float( f, &points[i*3 + 0] )) {
						status = InputFail;
						goto cleanup;
					}
					break;
				case PROPERTY_Y:
					if( !read_one_float( f, &points[i*3 + 1] )) {
						status = InputFail;
						goto cleanup;
					}
					break;
				case PROPERTY_Z:
					if( !read_one_float( f, &points[i*3 + 2] )) {
						status = InputFail;
						goto cleanup;
					}
					break;
				case PROPERTY_R:
					if( !read_one_byte( f, &byte ) ) {
						status = InputFail;
						goto cleanup;
					}
					vertex_colors[i*3 + 0] = (float)byte / 255.0f;
					break;
				case PROPERTY_G:
					if( !read_one_byte( f, &byte ) ) {
						status = InputFail;
						goto cleanup;
					}
					vertex_colors[i*3 + 1] = (float)byte / 255.0f;
					break;
				case PROPERTY_B:
					if( !read_one_byte( f, &byte ) ) {
						status = InputFail;
						goto cleanup;
					}
					vertex_colors[i*3 + 2] = (float)byte / 255.0f;
					break;
				case PROPERTY_NX:
					if( !read_one_float( f, &vertex_normals[i*3 + 0] )) {
						status = InputFail;
						goto cleanup;
					}
					break;
				case PROPERTY_NY:
					if( !read_one_float( f, &vertex_normals[i*3 + 1] )) {
						status = InputFail;
						goto cleanup;
					}
					break;
				case PROPERTY_NZ:
					if( !read_one_float( f, &vertex_normals[i*3 + 2] )) {
						status = InputFail;
						goto cleanup;
					}
					break;
			case PROPERTY_FLOAT_UNKNOWN:{
				float not_used;
				if( !read_one_float( f, &not_used ) ){
					status = InputFail;
					goto cleanup;
				}
			}break;
				default: 
					status = InputFail;
					goto cleanup;
			}
		} while( *op != END_ELEMENT_VERTEX );

		if (m_vertex_colors) {
			temp_node = new Color_Node;
			color_hash_key = (int)(256*256*vertex_colors[i*3 + 0] + 256*vertex_colors[i*3 + 1] + vertex_colors[i*3 + 2]);
			if (vertex_color_hash.LookupItem(color_hash_key, &cursor) == VHASH_STATUS_FAILED) {
				temp_node->entity_list->Append(i);
				temp_node->rgb_color_f[0] = vertex_colors[i*3 + 0];
				temp_node->rgb_color_f[1] = vertex_colors[i*3 + 1];
				temp_node->rgb_color_f[2] = vertex_colors[i*3 + 2];
				vertex_color_hash.InsertItem(color_hash_key, temp_node);
				temp_node = 0;
			}
			else {
				delete temp_node;
				temp_node = 0;
				cursor->entity_list->Append(i);
			}
		}
	}
	op++;
	save_op = op;

	if( *op == 0 )
		goto cleanup; //just a point cloud this time -- no faces


	/* read the face list */
	face_list_used = 0;
	for( i = 0 ; i < face_count ; i++ ) {
		op = save_op;
		if( *op++ != ELEMENT_FACE ||
			feof( f ) ) {
				status = InputFail;
				goto cleanup;
		}
		do {
			switch( *op++ ) {
				case PROPERTY_FACE_LIST:
					if( !read_one_byte( f, &byte ) ) {
						status = InputFail;
						goto cleanup;
					}
					count = byte;
					face_list[ face_list_used ] = count;
					face_list_used++;
					for( j = 0 ; j < count ; j++ )
					{
						if( face_list_used >= face_list_length - 1 ) {
							int *temp;
							if( face_list_length < 10 )
								face_list_length = 20;
							else
								face_list_length *= 2;
							temp = (int *) malloc (face_list_length * sizeof( int ));
							memcpy( temp, face_list, face_list_used * sizeof( int ));
							free( face_list );
							face_list = temp;
						}
						if( !read_one_int( f, &face_list[ face_list_used ] ) ) {
							status = InputFail;
							goto cleanup;
						}
						face_list_used++;
					}
					break;
				case PROPERTY_R:
					color_hash_key = 0;
					temp_node = new Color_Node;
					for( j = 0 ; j < 3 ; j++ ) {
						if( !read_one_byte( f, &byte ) ) {
							status = InputFail;
							goto cleanup;
						}
						face_colors[i*3 + j] = (float)byte / 255.0f;
						temp_node->rgb_color_f[j] = face_colors[i*3 + j];
						color_hash_key *= 256;
						color_hash_key += (int)byte;
					}
					if (face_color_hash.LookupItem(color_hash_key, &cursor) == VHASH_STATUS_FAILED) {
						temp_node->entity_list->Append(i);
						face_color_hash.InsertItem(color_hash_key, temp_node);
						temp_node = 0;
					}
					else {
						delete temp_node;
						temp_node = 0;
						cursor->entity_list->Append(i);
					}
					break;
				case PROPERTY_G:
				case PROPERTY_B:
					break;
				default: 
					status = InputFail;
					goto cleanup;
			}
		} while( *op != END_ELEMENT_FACE );
	}

	HC_Open_Segment_By_Key(key);
	shell_key = HC_KInsert_Shell( point_count, points, face_list_used, face_list );
	HC_Close_Segment();

	if (m_vertex_colors) {
		/* we have two cases:
		    (1) All vertices have the same color, in which case we just set the face color; OR
		    (2) We have differing vertex colors, in which case we set the color which is specified
		        on the largest number of vertices as the face color, and then set the remaining vertex
		        colors individually.
		*/
		int num_vertex_colors = vertex_color_hash.Count();
		char rgb_vertex_color_spec[MVO_BUFFER_SIZE];
		if (num_vertex_colors == 1) { // all vertices have the same color
			vertex_color_list = vertex_color_hash.GetPairList();
			cursor = vertex_color_list->PeekFirst()->Item();
			HCLOCALE(sprintf(rgb_vertex_color_spec, "faces = (r=%.6f g=%.6f b=%.6f)", 
					cursor->rgb_color_f[0], cursor->rgb_color_f[1], cursor->rgb_color_f[2]));
			HC_Open_Geometry(shell_key);
			    HC_Set_Color(rgb_vertex_color_spec);
			HC_Close_Geometry();
			delete vertex_color_list;
		}
		else { // we've got vertices with different colors
			vertex_color_list = vertex_color_hash.GetPairList();

			// first find the color specified on the largest number of faces
			int max_num_vertices_of_same_color = 0;
			vertex_color_list->ResetCursor();
			for ( v_c_l_cursor = vertex_color_list->PeekCursor() ;
				  (v_c_l_cursor = vertex_color_list->PeekCursor()) != 0 ;
				  vertex_color_list->AdvanceCursor() ) {
				if ((int)v_c_l_cursor->Item()->entity_list->Count() > max_num_vertices_of_same_color) {
					max_num_vertices_of_same_color = (int)v_c_l_cursor->Item()->entity_list->Count();
					max_cursor = v_c_l_cursor;
				}
			}

			// set our default color
			cursor = max_cursor->Item();
			HCLOCALE(sprintf(rgb_vertex_color_spec, "faces = (r=%.6f g=%.6f b=%.6f)", 
					cursor->rgb_color_f[0], cursor->rgb_color_f[1], cursor->rgb_color_f[2]));
			HC_Open_Geometry(shell_key);
			    HC_Set_Color(rgb_vertex_color_spec);
			HC_Close_Geometry();

			// set the remaining vertex colors by setting all vertex colors and then unsetting those
			//  which share the default color set above
			HC_MSet_Vertex_Colors_By_Value (shell_key, "faces", 0, "rgb", point_count, vertex_colors);

			HC_Open_Geometry(shell_key);
				for ( i = 0 ; i < max_num_vertices_of_same_color ; i++ ) {
					HC_Open_Vertex(cursor->entity_list->GetData(i));
						HC_UnSet_Color();
					HC_Close_Vertex();
				}
			HC_Close_Geometry();

			delete vertex_color_list;
		}
	}
	if (m_vertex_normals)
		HC_MSet_Vertex_Normals (shell_key, 0, point_count, vertex_normals);
	if (m_face_colors && !m_vertex_colors) {
		/* we have two cases:
		    (1) All faces have the same color, in which case we just set the face color; OR
		    (2) We have differing face colors, in which case we set the color which is specified
		        on the largest number of faces as the face color, and then set the remaining face
		        colors individually.
		*/
		int num_face_colors = face_color_hash.Count();
		char rgb_face_color_spec[MVO_BUFFER_SIZE];
		if (num_face_colors == 1) { // all faces are the same color
			face_color_list = face_color_hash.GetPairList();
			cursor = face_color_list->PeekFirst()->Item();
			HCLOCALE(sprintf(rgb_face_color_spec, "faces = (r=%.6f g=%.6f b=%.6f)", 
					cursor->rgb_color_f[0], cursor->rgb_color_f[1], cursor->rgb_color_f[2]));
			HC_Open_Geometry(shell_key);
			    HC_Set_Color(rgb_face_color_spec);
			HC_Close_Geometry();
			delete face_color_list;
		}
		else { // find the default color and set the other face colors appropriately
			face_color_list = face_color_hash.GetPairList();

			// first find the color specified on the largest number of faces
			int max_num_faces_of_same_color = 0;
			face_color_list->ResetCursor();
			for ( f_c_l_cursor = face_color_list->PeekCursor() ;
				  (f_c_l_cursor = face_color_list->PeekCursor()) != 0 ;
				  face_color_list->AdvanceCursor() ) {
				if ((int)f_c_l_cursor->Item()->entity_list->Count() > max_num_faces_of_same_color) {
					max_num_faces_of_same_color = (int)f_c_l_cursor->Item()->entity_list->Count();
					max_cursor = f_c_l_cursor;
				}
			}

			// set the default color
			cursor = max_cursor->Item();
			HCLOCALE(sprintf(rgb_face_color_spec, "faces = (r=%.6f g=%.6f b=%.6f)", 
					cursor->rgb_color_f[0], cursor->rgb_color_f[1], cursor->rgb_color_f[2]));
			HC_Open_Geometry(shell_key);
			    HC_Set_Color(rgb_face_color_spec);
			HC_Close_Geometry();

			// now we'll set the rest of the face colors
			//  we do this by setting all faces colors, and unsetting those which inherit from
			//  the default color
			HC_MSet_Face_Colors_By_Value (shell_key, "face", 0, "rgb", face_count, face_colors);

			HC_Open_Geometry(shell_key);
			for ( i = 0 ; i < max_num_faces_of_same_color ; i++ ) {
				HC_Open_Face(cursor->entity_list->GetData(i));
					HC_UnSet_Color();
				HC_Close_Face();
			}
			HC_Close_Geometry();

			delete face_color_list;
		}        
	}

cleanup:

	setlocale(LC_ALL, saved_locale);
#ifdef _MSC_VER
		if (was_type != _ENABLE_PER_THREAD_LOCALE)
			_configthreadlocale(was_type);
#endif

	if( points )
		free( points );
	if( vertex_colors )
		free( vertex_colors );
	if( vertex_normals )
		free( vertex_normals );
	if( face_list )
		free( face_list );
	if( face_colors )
		free( face_colors );
	if( f )
		fclose( f );

	vertex_color_list = vertex_color_hash.GetPairList();
	vertex_color_list->ResetCursor();
	for ( v_c_l_cursor = vertex_color_list->PeekCursor() ;
		  (v_c_l_cursor = vertex_color_list->PeekCursor()) != 0 ;
		  vertex_color_list->AdvanceCursor() ) {
		delete v_c_l_cursor->Item();
	}
	delete vertex_color_list;

	face_color_list = face_color_hash.GetPairList();
	face_color_list->ResetCursor();
	for ( f_c_l_cursor = face_color_list->PeekCursor() ;
		  (f_c_l_cursor = face_color_list->PeekCursor()) != 0 ;
		  face_color_list->AdvanceCursor() ) {
		delete f_c_l_cursor->Item();
	}
	delete face_color_list;

	if ( temp_node )
		delete temp_node;

	SetFinishedInput();

	return status;

} //end function HIOUtilityPly::FileInputByKey


bool HIOUtilityPly::count_recursive( int *total_point_count, int *total_face_count )
{
	HC_KEY key;

	HC_Begin_Contents_Search( ".", "shells, subsegments" );
	{
		char type[64];

		while( HC_Find_Contents( type, &key ) ) {
			/* special handling to follow includes */
			if( streq( type, "include" ) ) {
				key = HC_KShow_Include_Segment( key );
				HC_Open_Segment_By_Key( key );
				if( !count_recursive( total_point_count, total_face_count ) )
					return false;
				HC_Close_Segment();
			}
			/* recurse into subsegments */
			else if( streq( type, "segment" ) ) {
				HC_Open_Segment_By_Key( key );
				// <<AS: 19/01/09 - Don't export invisible segments
				char visibility[MVO_BUFFER_SIZE];
				HC_Show_Net_Visibility(visibility);
				if(streq(visibility, "off"))
				{
					HC_Close_Segment();
					continue;
				}
				// >>AS: 19/01/09 - Don't export invisible segments
				if( !count_recursive( total_point_count, total_face_count ) )
					return false;
				HC_Close_Segment();
			}
			/* process the shell inside */
			if( streq( type, "shell" ) ) {
				int point_count, face_list_length, len;
				int *face_list, *ptr, *end;

				HC_Show_Shell_Size( key, &point_count, &face_list_length );
				HC_Show_Geometry_Pointer( key, "face list", &face_list );
				*total_point_count += point_count;
				if( face_list ) {
					ptr = face_list;
					end = face_list + face_list_length;
					while( ptr < end ) {
						if( ptr[0] > 0 ) {
							//normal face
							len = ptr[0] + 1; 
							(*total_face_count)++;
						}
						else {
							//don't increment for holes -- ply doesn't support them
							len = -ptr[0] + 1; 
						}
						ptr += len;
					}
				}
			}
		}
	}
	HC_End_Contents_Search();
	return true;
} //end function count_recursive


bool HIOUtilityPly::write_points_recursive( FILE *f, float *modelling_matrix_in )
{
	HC_KEY key;
	float tempmatrix[16];
	float *modelling_matrix;

	if( HC_Show_Existence( "modelling matrix" )) {
		HC_Show_Modelling_Matrix( tempmatrix );
		if( modelling_matrix_in )
			HC_Compute_Matrix_Product( tempmatrix, modelling_matrix_in, tempmatrix );
		modelling_matrix = tempmatrix;
	}
	else
		modelling_matrix = modelling_matrix_in;

	HC_Begin_Contents_Search( ".", "shells, subsegments" );
	{
		char type[64];

		while( HC_Find_Contents( type, &key ) ) {
			/* special handling to follow includes */
			if( streq( type, "include" ) ) {
				key = HC_KShow_Include_Segment( key );
				HC_Open_Segment_By_Key( key );
				if( !write_points_recursive(f, modelling_matrix) )
					return false;
				HC_Close_Segment();
			}
			/* recurse into subsegments */
			else if( streq( type, "segment" ) ) {
				HC_Open_Segment_By_Key( key );

				// <<AS: 19/01/09 - Don't export invisible segments
				char visibility[MVO_BUFFER_SIZE];
				HC_Show_Net_Visibility(visibility);
				if(streq(visibility, "off"))
				{
					HC_Close_Segment();
					continue;
				}
				// >>AS: 19/01/09 - Don't export invisible segments
				if( !write_points_recursive(f, modelling_matrix) )
					return false;
				HC_Close_Segment();
			}
			/* process the shell inside */
			if( streq( type, "shell" ) ) {
				int point_count, i;
				HPoint *points;

				HC_Show_Shell_Size( key, &point_count, 0 );
				HC_Show_Geometry_Pointer( key, "points", &points );
				if( !points )
					return false;
				if( modelling_matrix ) {
					HPoint pt;
					for( i = 0 ; i < point_count ; i++ ) {
						HC_Compute_Transformed_Points( 1, &points[i], modelling_matrix, &pt );
						write_one_float( f, pt.x );
						write_one_float( f, pt.y );
						write_one_float( f, pt.z );
						if( !m_binary )
							fputs( "\n", f );
					}
				}
				else {
					for( i = 0 ; i < point_count ; i++ ) {
						write_one_float( f, points[i].x );
						write_one_float( f, points[i].y );
						write_one_float( f, points[i].z );
						if( !m_binary )
							fputs( "\n", f );
					}
				}
			}
		}
	}
	HC_End_Contents_Search();
	return true;

} //end function write_points_recursive 


bool HIOUtilityPly::write_faces_recursive( FILE *f, int *point_offset )
{
	HC_KEY key;

	HC_Begin_Contents_Search( ".", "shells, subsegments" );
	{
		char type[64];

		while( HC_Find_Contents( type, &key ) ) {
			/* special handling to follow includes */
			if( streq( type, "include" ) ) {
				key = HC_KShow_Include_Segment( key );
				HC_Open_Segment_By_Key( key );
				if( !write_faces_recursive(f, point_offset) )
					return false;
				HC_Close_Segment();
			}
			/* recurse into subsegments */
			else if( streq( type, "segment" ) ) {
				HC_Open_Segment_By_Key( key );
				// <<AS: 19/01/09 - Don't export invisible segments
				char visibility[MVO_BUFFER_SIZE];
				HC_Show_Net_Visibility(visibility);
				if(streq(visibility, "off"))
				{
					HC_Close_Segment();
					continue;
				}
				// >>AS: 19/01/09 - Don't export invisible segments
				if( !write_faces_recursive(f, point_offset) )
					return false;
				HC_Close_Segment();
			}
			/* process the shell inside */
			if( streq( type, "shell" ) ) {
				int point_count, face_list_length, len;
				int *face_list, *ptr, *end, i;

				HC_Show_Shell_Size( key, &point_count, &face_list_length );
				HC_Show_Geometry_Pointer( key, "face list", &face_list );
				if( face_list ) {
					ptr = face_list;
					end = face_list + face_list_length;
					while( ptr < end ) {
						if( ptr[0] > 0 ) {
							//normal face
							len = ptr[0] + 1;
							write_one_byte( f, static_cast<unsigned char>(ptr[0]) );
							for( i = 1 ; i < len ; i++ ) {
								write_one_int( f, ptr[i] + *point_offset );
							}
							if( !m_binary )
								fputs( "\n", f );
						}
						else {
							//don't increment for holes -- ply doesn't support them
							len = -ptr[0] + 1; 
						}
						ptr += len;
					}
				}
				(*point_offset) += point_count;
			}
		}
	}
	HC_End_Contents_Search();
	return true;

} //end function write_faces_recursive


bool HIOUtilityPly::count( int *total_point_count, int *total_face_count )
{
	*total_point_count = 0;
	*total_face_count = 0;
	return count_recursive( total_point_count, total_face_count );
}


bool HIOUtilityPly::write_points( FILE *f )
{
	return write_points_recursive( f, 0 );
}


bool HIOUtilityPly::write_faces( FILE *f )
{
	int point_offset = 0;
	return write_faces_recursive( f, &point_offset );
}


typedef struct {
	HPoint *points;
	int points_allocated, points_used;
	int *face_list;
	int face_list_allocated, face_list_used;
} shell_info_t;


void append_triangle_recursive_subdivide( 
	shell_info_t *sh, 
	float longest_edge, 
	int a,
	int b,
	int c )
{
	int ab, bc, ca;

	if( ( edge_length( &sh->points[a], &sh->points[b] ) > longest_edge ) ||
		( edge_length( &sh->points[b], &sh->points[c] ) > longest_edge ) ||
		( edge_length( &sh->points[c], &sh->points[a] ) > longest_edge ) ) {
			if( sh->points_used + 3 >= sh->points_allocated ) {
				HPoint *temp;
				sh->points_allocated *= 2;
				temp = new HPoint[ sh->points_allocated ];
				memcpy( temp, sh->points, sh->points_used * sizeof( HPoint ) );
				delete [] sh->points;
				sh->points = temp;
			}
			sh->points[ sh->points_used ].x = (sh->points[a].x + sh->points[b].x) / 2;
			sh->points[ sh->points_used ].y = (sh->points[a].y + sh->points[b].y) / 2;
			sh->points[ sh->points_used ].z = (sh->points[a].z + sh->points[b].z) / 2;
			ab = sh->points_used++;
			sh->points[ sh->points_used ].x = (sh->points[b].x + sh->points[c].x) / 2;
			sh->points[ sh->points_used ].y = (sh->points[b].y + sh->points[c].y) / 2;
			sh->points[ sh->points_used ].z = (sh->points[b].z + sh->points[c].z) / 2;
			bc = sh->points_used++;
			sh->points[ sh->points_used ].x = (sh->points[c].x + sh->points[a].x) / 2;
			sh->points[ sh->points_used ].y = (sh->points[c].y + sh->points[a].y) / 2;
			sh->points[ sh->points_used ].z = (sh->points[c].z + sh->points[a].z) / 2;
			ca = sh->points_used++;
			append_triangle_recursive_subdivide( sh, longest_edge, a, ab, ca );
			append_triangle_recursive_subdivide( sh, longest_edge, ab, b, bc );
			append_triangle_recursive_subdivide( sh, longest_edge, bc, c, ca );
			append_triangle_recursive_subdivide( sh, longest_edge, ab, bc, ca );
	}
	else {
		if( sh->face_list_used + 4 >= sh->face_list_allocated ) {
			int *temp;
			sh->face_list_allocated *= 2;
			temp = new int[ sh->face_list_allocated ];
			memcpy( temp, sh->face_list, sh->face_list_used * sizeof( int ) );
			delete [] sh->face_list;
			sh->face_list = temp;
		}
		sh->face_list[ sh->face_list_used++ ] = 3;
		sh->face_list[ sh->face_list_used++ ] = a;
		sh->face_list[ sh->face_list_used++ ] = b;
		sh->face_list[ sh->face_list_used++ ] = c;
	}
} // end function append_triangle_recursive_subdivide


static void process_one_shell( float longest_edge, float *modelling_matrix, HC_KEY key )
{
	HPoint *points = 0;
	int *face_list = 0, *ptr, *end;
	int point_count, face_list_length, i;
	bool needs_processing = false;
	shell_info_t sh = { 0, 0, 0, 0, 0, 0 };
	shell_info_t sh2 = { 0, 0, 0, 0, 0, 0 };

	HC_Show_Shell_Size( key, &point_count, &face_list_length );
	if( point_count <= 0 )
		return;

	if( modelling_matrix ) {
		float *original_points;
		points = new HPoint[ point_count ];
		HC_Show_Geometry_Pointer( key, "points", &original_points );
		HC_Compute_Transformed_Points( point_count, original_points, modelling_matrix, points );
		needs_processing = true;
	}
	else {
		HC_Show_Geometry_Pointer( key, "points", &points );
		HC_Show_Geometry_Pointer( key, "face list", &face_list );
		if( face_list ) {
			ptr = face_list;
			end = face_list + face_list_length;
			while( ptr < end ) {
				if( ( ptr[0] != 3 ) ||
					( edge_length( &points[ptr[1]], &points[ptr[2]] ) > longest_edge ) ||
					( edge_length( &points[ptr[2]], &points[ptr[3]] ) > longest_edge ) ||
					( edge_length( &points[ptr[3]], &points[ptr[1]] ) > longest_edge ) )
				{
					needs_processing = true;
					break;
				}
				ptr += 4;
			}
		}
	}
	if( !needs_processing )
		goto cleanup;

	if( !points )
		HC_Show_Geometry_Pointer( key, "points", &points );
	if( !face_list )
		HC_Show_Geometry_Pointer( key, "face list", &face_list );

	sh.points_allocated = point_count * 2;
	sh.points = new HPoint[ sh.points_allocated ];
	memcpy( sh.points, points, point_count * sizeof( HPoint ) );
	sh.points_used = point_count;
	sh.face_list_allocated = face_list_length * 2;
	sh.face_list = new int[ sh.face_list_allocated ];
	sh.face_list_used = 0;

	ptr = face_list;
	end = face_list + face_list_length;
	while( ptr < end ) {
		if( ptr[0] < 0 ) {
			ptr += -ptr[0] + 1;
			continue;
		}
		for( i = 3 ; i <= ptr[0] ; i++ )
			append_triangle_recursive_subdivide( &sh, longest_edge, ptr[i-1], ptr[i], ptr[1] );
		ptr += ptr[0] + 1;
	}

	HC_Delete_By_Key( key );
	sh2.points = new HPoint[ sh.points_used ];
	sh2.face_list = new int[ sh.face_list_used ];
	HC_Compute_Optimized_Shell( 
		sh.points_used, sh.points, 0, sh.face_list_used, sh.face_list,
		"",
		&sh2.points_used, sh2.points, &sh2.face_list_used, sh2.face_list, 
		0, 0
		);
	HC_Insert_Shell( sh2.points_used, sh2.points, sh2.face_list_used, sh2.face_list );

cleanup:
	if( sh.points )
		delete [] sh.points;
	if( sh.face_list )
		delete [] sh.face_list;
	if( sh2.points )
		delete [] sh2.points;
	if( sh2.face_list )
		delete [] sh2.face_list;
	if( modelling_matrix )
		delete [] points;

} // end function process_one_shell


static bool process_recursive( float longest_edge, float *modelling_matrix_in ) 
{
	HC_KEY key;
	float tempmatrix[16];
	float *modelling_matrix;

	if( HC_Show_Existence( "modelling matrix" )) {
		HC_Show_Modelling_Matrix( tempmatrix );
		//HC_UnSet_Modelling_Matrix();
		if( modelling_matrix_in )
			HC_Compute_Matrix_Product( tempmatrix, modelling_matrix_in, tempmatrix );
		modelling_matrix = tempmatrix;
	}
	else
		modelling_matrix = modelling_matrix_in;

	HC_Begin_Contents_Search( ".", "shells, subsegments" );
	{
		char type[64];

		while( HC_Find_Contents( type, &key ) ) {
			/* special handling to follow includes */
			if( streq( type, "include" ) ) {
				key = HC_KShow_Include_Segment( key );
				HC_Open_Segment_By_Key( key );
				if( !process_recursive(longest_edge, modelling_matrix) )
					return false;
				HC_Close_Segment();
			}
			/* recurse into subsegments */
			else if( streq( type, "segment" ) ) {
				HC_Open_Segment_By_Key( key );
				// <<AS: 19/01/09 - Don't export invisible segments
				//char path[MVO_BUFFER_SIZE];
				char visibility[MVO_BUFFER_SIZE];
				HC_Show_Net_Visibility(visibility);
				if(streq(visibility, "off"))
				{
					HC_Close_Segment();
					continue; 
				}
				// >>AS: 19/01/09 - Don't export invisible segments
				if( !process_recursive(longest_edge, modelling_matrix) )
					return false;
				HC_Close_Segment();
			}
			/* process the shell inside */
			if( streq( type, "shell" ) ) {
				process_one_shell( longest_edge, modelling_matrix, key );
			}
		}
	}
	HC_End_Contents_Search();
	return true;

} //end function process_recursive 


static void hack( HC_KEY key, int res ) 
{
	HPoint min, max;
	float diagonal, longest_edge;

	HC_Open_Segment_By_Key( key );
	HC_Show_Bounding_Cuboid( &min, &max );
	diagonal = edge_length( &min, &max );
	longest_edge = diagonal / (0x1 << res);
	process_recursive( longest_edge, 0 );
	HC_Close_Segment();
}


HFileOutputResult HIOUtilityPly::FileOutputByKey(const wchar_t *FileName, HC_KEY key, HOutputHandlerOptions * options) 
{
	UNREFERENCED(options);

	FILE *f = NULL;
	int total_point_count, total_face_count;
	HFileOutputResult status = OutputOK;
	HPoint min, max;
	HC_KEY temp;

	if(!FileName) 
		return OutputBadFileName;
	if(key == INVALID_KEY) 
		return OutputFail;

	
	SetStartingOutput();

#if 0
	m_binary = false;
#else
	char segname[MVO_BUFFER_SIZE];
	HC_Show_Segment(key, segname);
	temp = HC_KCopy_Segment(segname, "/temporary ply include segment with a truly singular name");
	HC_Optimize_Segment_Tree_By_Key(temp, "collapse matrices=on,expand includes=on,merge shells=off,reorganize=spatial");

	hack( temp, 6 );
	m_binary = true;
	m_bigendian = true;
#endif

	if( m_binary )
		f = wfopen(FileName, L"wb");  // open the file as binary
	else
		f = wfopen(FileName, L"w");  // open the file as ASCII

	if (f == NULL) 
		return OutputBadFileName;

	HC_Open_Segment_By_Key( temp );
	HC_Show_Bounding_Cuboid( &min, &max );
	if( !count( &total_point_count, &total_face_count )) {
		status = OutputFail;
		goto cleanup;
	}
	if( total_point_count <= 0 || 
		!write_ply_header( f, total_point_count, total_face_count, &min, &max )) {
			status = OutputFail;
			goto cleanup;
	}
	if( !write_points( f )) {
		status = OutputFail;
		goto cleanup;
	}
	if( !write_faces( f )) {
		status = OutputFail;
		goto cleanup;
	}
	HC_Close_Segment();

cleanup:
	if( f )
		fclose( f );
	HC_Delete_By_Key(temp);

	
	SetFinishedOutput();

	return status;
}


