// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HDWFSTREAM_H
#define _HDWFSTREAM_H

#include "hc.h"
#include "HStream.h"


class HW3DPoint 
{ 
public:
	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.

	/*! default constructor */
	HW3DPoint() : x(0), y(0), z(0) {}
	/*! constructor that accepts three floats */
	HW3DPoint(float X, float Y, float Z=0.0f) : x(X), y(Y), z(Z) {}
	/*! constructor that accepts a pointer to an HW3DPoint object */
	HW3DPoint(HW3DPoint const *p) { x = p->x; y = p->y; z = p->z;};
	/*! constructor that accepts an HW3DPoint reference */
	HW3DPoint(const HW3DPoint & p) { x = p.x; y = p.y; z = p.z; };

	/*!  Packs an HW3DPoint object with coordinate values.  */
	void Set(float X,float Y,float Z = 0.0f) { x = X; y = Y; z = Z; };
	/*!  Packs an HW3DPoint object with coordinate values.  */
	void Set(HW3DPoint * p) { x = p->x; y = p->y; z = p->z; };
	/*!  Packs an HW3DPoint object with coordinate values.  */
	void Set(HW3DPoint const * p) { x = p->x; y = p->y; z = p->z; };
	/*!  Packs an HW3DPoint object with coordinate values.  */
	void Set(HW3DPoint & p) { x = p.x; y = p.y; z = p.z; };
	/*!  Packs an HW3DPoint object with coordinate values.  */
	void Set(const HW3DPoint & p) { x = p.x; y = p.y; z = p.z; };
};

class HW3DCamera {
public:
	bool		is_valid;		//!< is data valid
	HW3DPoint	position;		//!< position
	HW3DPoint	target;			//!< target 
	HW3DPoint	up_vector;		//!< up_vector 
	float		field_width;	//!< field width
	float		field_height;	//!< field height
	char		projection[128];//!< projection type
};

class HW3DStream_Initial_View : public HTK_Camera 
{
public:
	HW3DStream_Initial_View(HW3DCamera * initial_view) : HTK_Camera(TKE_View), m_pInitialCamera(initial_view) {;}

	TK_Status Interpret(BStreamFileToolkit & /*tk*/, ID_Key /*key*/, int /*variant*/)
	{
		if(!m_pInitialCamera)
			return TK_Normal;

		m_settings[0] = m_pInitialCamera->position.x; 
		m_settings[1] = m_pInitialCamera->position.y; 
		m_settings[2] = m_pInitialCamera->position.z;

		m_settings[3] = m_pInitialCamera->target.x; 
		m_settings[4] = m_pInitialCamera->target.y; 
		m_settings[5] = m_pInitialCamera->target.z;

		m_settings[6] = m_pInitialCamera->up_vector.x; 
		m_settings[7] = m_pInitialCamera->up_vector.y; 
		m_settings[8] = m_pInitialCamera->up_vector.z;

		m_settings[9]  = m_pInitialCamera->field_width; 
		m_settings[10] = m_pInitialCamera->field_height;

		if(m_pInitialCamera->projection[0] == 'p')
			m_projection = TKO_Camera_Perspective;
		else
			m_projection = TKO_Camera_Orthographic;

		SetView("default");

		return TK_Normal;
	}


	TK_Status Execute(BStreamFileToolkit & /*tk*/) alter
	{
		TK_Status status=TK_Normal;

		if (!strcmp(GetView(),"default"))
		{
			m_pInitialCamera->is_valid = true;

			m_pInitialCamera->position.Set(m_settings[0],m_settings[1],m_settings[2]);
			m_pInitialCamera->target.Set(m_settings[3],m_settings[4],m_settings[5]);
			m_pInitialCamera->up_vector.Set(m_settings[6],m_settings[7],m_settings[8]);
			m_pInitialCamera->field_width = m_settings[9];
			m_pInitialCamera->field_height = m_settings[10];

			switch ((int) m_projection) {

			case TKO_Camera_Orthographic:	
				strcpy_s (m_pInitialCamera->projection, "orthographic");	
				break;

			case TKO_Camera_Perspective:	
				strcpy_s (m_pInitialCamera->projection, "perspective");	
				break;

			default: 
				strcpy_s (m_pInitialCamera->projection, "stretched"); 
				break;
			}

		}

		return (status);
	};

private:
	HW3DCamera * m_pInitialCamera;

};
#if 0
// Translate all the TKE_Image's to W3DE_Image
class HW3DStream_Image_Converter : public HTK_Image
{
private:
	HW3DStream_Image_Converter(const HW3DStream_Image_Converter&);
	HW3DStream_Image_Converter& operator=(const HW3DStream_Image_Converter&);

public:

	HW3DStream_Image_Converter ()
	{ 
		m_opcode = W3DE_Image;
		m_size[0] = m_size[1] = 0; 
	}

	~HW3DStream_Image_Converter()
	{
		delete [] m_name;
	}

	TK_Status   Write (BStreamFileToolkit & tk) alter
	{
		TK_Status status = TK_Normal;

		switch (m_stage) {
		case 0: {
			if ((status = PutOpcode (tk)) != TK_Normal)
				return status;
			m_stage++;
				}

		case 1: {

			if (m_name_length > 0) {
				unsigned char byte = (unsigned char)m_name_length;
				// need handling for string > 255 ?
				if ((status = PutData (tk, byte)) != TK_Normal)
					return status;
			}
			m_stage++;
				}

		case 2: {
			if (m_name_length > 0) {
				if ((status = PutData (tk, m_name, m_name_length)) != TK_Normal)
					return status;
			}
			m_stage++;
				}
		case 3:
			{
				if ((status = PutData(tk, m_size[0])) != TK_Normal)
					return status;
				if ((status = PutData(tk, m_size[1])) != TK_Normal)
					return status;
				if ((status = PutData(tk, 24)) != TK_Normal)
					return status;
				m_stage++;

			}

		case 4: {
			if (Tagging (tk))
				status = Tag (tk);

			m_stage = -1;
				}   break;

		default:
			return tk.Error();
		}

		return status;
	}
};
#endif

bool HW3D_Read_W3D_File(DWFInputStream * input_stream, 
	void * pModelSection, 
										HW3DCamera * ret_initial_camera,
	bool importMetaData);


bool generate_object_definitions(std::map<HC_KEY, int> const & tk_map,
	HC_KEY key, 
	void * arg1,
	void * arg2
	);
#define W3D_READ_BUFFER_SIZE 16*1024

#endif	// _HDWFSTREAM_H


