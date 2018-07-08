// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYPLY_H
#define _HIOUTILITYPLY_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "varray.h"
#include <stdio.h>

/*! The HIOUtilityPly is used for reading PLY files.  
*/

class MVO_API HIOUtilityPly: public HInputHandler, public HOutputHandler
{
private:
        bool m_binary;
        bool m_bigendian;

        bool m_vertex_colors;
        bool m_vertex_normals;
        bool m_face_colors;
        
        bool read_ply_header( FILE *f, int *point_count, int *face_count, int *opcodes );
        bool write_ply_header( FILE *f, int point_count, int face_count, HPoint *min=0, HPoint *max=0 );

        bool count( int *total_point_count, int *total_face_count );
        bool write_points( FILE *f );
        bool write_faces( FILE *f );

        bool count_recursive( int *total_point_count, int *total_face_count );
        bool write_points_recursive( FILE *f, float *modelling_matrix_in );
        bool write_faces_recursive( FILE *f, int *point_offset );

        bool write_one_int( FILE *f, int val );
        bool write_one_byte( FILE *f, unsigned char val );
        bool write_one_float( FILE *f, float val );
        bool read_one_int( FILE *f, int *val );
        bool read_one_byte( FILE *f, unsigned char *val );
        bool read_one_float( FILE *f, float *val );

		class Color_Node {
		    public:
			Color_Node() { entity_list = new VArray<int>; }
			~Color_Node() { delete entity_list; }
			float rgb_color_f[3];
			VArray<int> *entity_list;
		};

public:

	/*!Constructs an HIOUtilityPly object.*/
	HIOUtilityPly() {
                m_binary = false;
                m_bigendian = false;
		SetInputOps(HInputOpFileInputByKey);
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	virtual ~HIOUtilityPly() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityPly"; }
	const char * GetInputName() { return "HIOUtilityPly"; }

	/*!This method registers the "ply" extension to this handler as an input handler.*/
	void RegisterInputHandlerTypes() {
		HIORegisterInputType("ply",this);
	};

	/*!This method registers the "ply" extension to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("ply",this);
	};

	/*!	\returns The input extension type that is handled by HIOUtilityPly which is "ply".*/
	const char * GetInputTypesString() {return "ply";};

	/*! \return The string of the type of output this handler creates which is "ply".*/
	const char * GetOutputTypesString() {return "ply";};

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*! \return The type of output style which is HOutputHandlerStyleModel.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleModel;};

	/*!
	  This method reads a ply file which is commonly used in academic research.  The file contents are mapped
	  to a HOOPS shell primitive that is inserted into the HOOPS segment specified by the key.

		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options This parameter is currently not being used.
		\return The #HFileInputResult result of the input operation.
	*/
	HFileInputResult FileInputByKey(const __wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options);

	HFileInputResult FileInputByKey(const char * FileName, HC_KEY key, HInputHandlerOptions * options){
		return FileInputByKey(H_WCS(FileName).encodedText(), key, options);
	};

	HFileInputResult FileInputByKey(const unsigned short * FileName, HC_KEY key, HInputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) FileName);
		return FileInputByKey(H_WCS(utf16).encodedText(), key, options);
	};

	/*! 
		This method writes write a ply file which is commonly used in academic research.  
		\param filename The name of the file to output.
		\param key The HOOPS segment key to output model data from.
		\param options This parameter is currently not being used.
		\return The #HFileOutputResult result of the output operation.
	*/
	HFileOutputResult FileOutputByKey(const __wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) filename);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}

	HFileOutputResult FileOutputByKey(const char * filename, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey(H_WCS(filename).encodedText(), key, options);
	}

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
