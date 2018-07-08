// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYPOINTCLOUD_H
#define _HIOUTILITYPOINTCLOUD_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

/*!
	The HPointCloudOptions class provides a mechanism for controlling certain aspects of the point cloud import.
 */

#include "HIOManager.h"
#include <stdint.h>

class MVO_API HPointCloudOptions {
public:
	/*!
		Constructs the HPointCloudOptions object and sets the defaults by calling Defaults.
	 */
	HPointCloudOptions () { Defaults(); }
	~HPointCloudOptions () {}

	/*!
		This method sets the default values for the various control points.
	 */
	void Defaults () {
		m_numBuckets = 512;
		m_lowIntensityValue = 0;
		m_highIntensityValue = 0;
		m_maxShellSize = 10000;
		m_minShellSize = 2000;
	}

	/*!
		This field sets the number of buckets to bin gray scale values into.  If it is set to zero, no down-sampling will occur.
		This field only affects gray scale point clouds.
	 */
	unsigned int m_numBuckets;
	/*!
		This field sets the lower limit on the intensity values to be found in the file.  If it is set to zero, this value is determined
		automatically from the data.
	 */
	int m_lowIntensityValue;
	/*!
		This field sets the upper limit on the intensity values to be found in the file.  If it is set to zero, this value is determined
		automatically from the data.
	*/
	int m_highIntensityValue;
	/*!
		This field sets the maximum number of points to have in a spatially segregated shell.
	*/
	int m_maxShellSize;
	/*!
		This field sets the minimum number of points to have in a spatially segregated shell.
	*/
	int m_minShellSize;
};

#include "vbsp.h"

class PCPoint;
class RGBColor;
class BSPData;
class HBaseView;
typedef DefaultBSPNodeItemContainer<PCPoint *> PCPointContainer;

/*!
	The HIOUtilityPointCloud class is used for loading point cloud data into HOOPS.  This class is designed for
	in-core usage, i.e., the point set must be small enough to fit in memory.  The reading happens in three passes:
	  - The first pass obtains an accurate bounding box for the point cloud and generates a (temporary) binary version of the file
		to facilitate quicker processing in the subsequent pass.
	  - The second pass reads the binary version of the point cloud, and sorts the points in an octree structure.  It generates shells of
		a user-specified size (see the #HPointCloudOptions class) and stores these in a (temporary) HSF-like binary file to ultimately be 
		read into the scene graph.
	  - The third pass reads the stored shells generated from the previous step and inserts HOOPS shells into the scene graph.
	
	Note: The temporary files will be removed by the reader when it no longer needs them.  However, the size of these files will be 
	relative to the size of the original point cloud - so one will need to have the necessary disk space accomodate these files 
	(albeit temporarily) when importing.
 */

enum PointCloudFileType {
	NONE,
	UNKNOWN,
	PTS,
	PTX,
	XYZ
};

class MVO_API HIOUtilityPointCloud : public HInputHandler
{
public:
	HIOUtilityPointCloud ();
	~HIOUtilityPointCloud ();

	/*! 
		This method loads a point cloud to a given HOOPS segment key.
			\param filename The name of the file to load.
			\param key The HOOPS segment key to load model data into.
			\param options An options class containing any needed extra information for the operation.
			\return The #HFileInputResult result of the input operation.
	 */
	HFileInputResult FileInputByKey ( const __wchar_t * filename, HC_KEY key, HInputHandlerOptions * options );
	HFileInputResult FileInputByKey ( const unsigned short * filename, HC_KEY key, HInputHandlerOptions * options ){
		H_UTF16 utf16;
		utf16.encodedText(filename);
		return FileInputByKey(H_WCS(utf16).encodedText(), key, options);
	};
	HFileInputResult FileInputByKey ( const char * filename, HC_KEY key, HInputHandlerOptions * options ) {
		return FileInputByKey(H_WCS(filename).encodedText(), key, options);
	};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetInputName() { return "HIOUtilityPointCloud"; }

	/*!
		This method returns the extensions handled by the reader.
	 */
	const char * GetInputTypesString ();
	HInputHandlerStyle GetInputStyle () { return HInputHandlerStyleModel; };
	/*!
		This method registers the "pts" and "ptx" extensions as those handled by the reader.
	 */
	void RegisterInputHandlerTypes ();

private:
	bool Initialize ( const __wchar_t * filename );
	HFileInputResult SpatiallySortPointCloud ( HC_KEY seg_key );

	float GetScaledIntensity ( PCPoint * cpt );

	// functions when walking the hash
	static void DeleteBSPs ( VBSP<PCPoint *> * bsp, int bucket, const void * const user_data );
	static void WriteShellsToFile ( VBSP<PCPoint *> * bsp, int bucket, const void * const user_data );

	// function when walking the BSP
	static VBSPStatus GetShellsFromBSP ( PCPointContainer * container, void * user_data );

	bool InsertShells ( BSPData * bsp_data );

	FILE * 		  			m_file;
	int64_t		  			m_file_size;
	wchar_t 	  			m_bin_file_name[MVO_BUFFER_SIZE];
	FILE * 		  			m_bin_file;
                  			
	PointCloudFileType		m_point_cloud_file_type;
	unsigned long 			m_num_points;
	float 		  			m_std_bbox[6];
	float 		  			m_matrix[16];
	int 		  			m_min_intensity;
	int 		  			m_max_intensity;

	HBaseView * 			m_pHView;
	HPointCloudOptions * 	m_point_cloud_options;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
