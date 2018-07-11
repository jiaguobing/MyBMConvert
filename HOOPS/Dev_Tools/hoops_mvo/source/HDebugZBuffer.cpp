// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HDebugZBuffer.h"

#ifdef ENABLE_DEBUG_ZBUFFER
#include <string.h>
#include <stdio.h>

#include "HBaseView.h"
#include "HImManager.h"
#include "hic.h"

#ifdef OSX_SYSTEM

# undef OS_INLINE
# define OS_INLINE static __inline__ /* workaround for a problem in agl headers */
# include <AGL/agl.h>
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <Carbon/Carbon.h>

#else

#ifdef WINDOWS_SYSTEM
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
# include <windows.h>
#endif

# include <GL/gl.h>
# include <GL/glu.h>

#endif

#endif

HDebugZBuffer::HDebugZBuffer( HBaseView *view )
{
#ifdef ENABLE_DEBUG_ZBUFFER
	int standalone_segname_length;
    const char *driver_type;
	const char *driver_base = "/driver/opengl/window0+";

	m_ViewKey = m_StandaloneKey = m_GreyImageKey = INVALID_KEY;	
	m_width = m_height = -1;
	m_StandaloneSegmentName = 0;
	m_FloatBuffer = 0;
	m_GreyBuffer = 0;

    driver_type = view->GetDriverType();
    if (!streq(driver_type, "opengl")) {
		HDB::ReportError( "HDebugZBuffer supports opengl only" );
		m_ViewKey = INVALID_KEY;
		return;
    }
	m_ViewKey = view->GetViewKey();
	HC_Open_Segment_By_Key( m_ViewKey );
		HC_Set_User_Index( H_DEBUG_ZBUFFER_MAGIC, this );
	HC_Close_Segment();
	HImSetCallbackByKey( m_ViewKey, "finish picture", "debug zbuffer finish picture" );
	standalone_segname_length = (int)(strlen(driver_base) + 10);
	m_StandaloneSegmentName = new char[ standalone_segname_length ];
	sprintf( m_StandaloneSegmentName, "%s%ld", driver_base, HDB::GetNextCookie()+1 );
	m_StandaloneKey = HC_KOpen_Segment( m_StandaloneSegmentName );
		HC_Set_Driver_Options( "isolated" );
		HC_Set_Driver_Options( "disable input=all" );
		HC_Set_Driver_Options( "double buffering" );
		HC_Update_One_Display( m_StandaloneSegmentName );
	HC_Close_Segment();
	m_width = m_height = 0;
	view->SetDebugZBuffer( this );
#else
	UNREFERENCED(view);
#endif
}


HDebugZBuffer::~HDebugZBuffer() 
{
#ifdef ENABLE_DEBUG_ZBUFFER
	if( m_ViewKey != INVALID_KEY ) {
		HC_Open_Segment_By_Key( m_ViewKey );
			HC_UnSet_One_User_Index( H_DEBUG_ZBUFFER_MAGIC );
			HImUnSetOneCallbackByKey( m_ViewKey, "finish picture");
		HC_Close_Segment();
		H_SAFE_DELETE( m_StandaloneSegmentName );
		H_SAFE_DELETE( m_FloatBuffer );
		H_SAFE_DELETE( m_GreyBuffer );
		HC_Delete_By_Key( m_GreyImageKey );
	}
#endif
}

#ifndef BIG_FLOAT 
#  define BIG_FLOAT 1e10
#endif


void HDebugZBuffer::OpenglFinishPicture( HIC_Rendition const *nr, bool swap_buffers )
{
#ifdef ENABLE_DEBUG_ZBUFFER

	float min = BIG_FLOAT, max = -BIG_FLOAT;
	float scale = 0;
	float *ptr, *end;
	unsigned char *cptr, val;
	char label[MVO_SMALL_BUFFER_SIZE];

	if( HIC_Driver_Get_Window_Width(nr) != m_width ||
		HIC_Driver_Get_Window_Height(nr) != m_height ) {
		if( m_width != 0 && m_height != 0 ) {
			H_SAFE_DELETE( m_FloatBuffer );
			H_SAFE_DELETE( m_GreyBuffer );
			HC_Delete_By_Key( m_GreyImageKey );
		}
		m_width = HIC_Driver_Get_Window_Width(nr);
		m_height = HIC_Driver_Get_Window_Height(nr);
		m_FloatBuffer = new float[ m_width * m_height ];
		m_GreyBuffer = new unsigned char[ 3 * m_width * m_height ];
		m_GreyImageKey = HC_QKInsert_Image( m_StandaloneSegmentName, 0, 0, 0, "rgb", m_width, m_height, m_GreyBuffer );
	}

	if (HIC_Driver_Is_Double_Buffering(nr))
		glReadBuffer(GL_BACK);
	else
		glReadBuffer(GL_FRONT);
	glFlush();
	glReadPixels (0, 0, m_width, m_height, GL_DEPTH_COMPONENT, GL_FLOAT, m_FloatBuffer );
	ptr = m_FloatBuffer;
	end = m_FloatBuffer + m_width * m_height;
	while( ptr < end ) {
		if( *ptr < 0.99 ) {
			if( *ptr < min )
				min = *ptr;
			if( *ptr > max )
				max = *ptr;
		}
		ptr++;
	}
	if( min != BIG_FLOAT ) {
		if( min == max )
			scale = 0;
		else
			scale = 1.0f / (max - min);
	}
	else {
		max = min = 1;
	}
	ptr = m_FloatBuffer;
	cptr = m_GreyBuffer;
	while( ptr < end ) {
		if( *ptr < min )
			val = 0;
		else if( *ptr > max )
			val = 255;
		else
			val = (unsigned char)(((*ptr-min) * scale) * 255.99);
		cptr[0] = cptr[1] = cptr[2] = val;
		ptr++;
		cptr += 3;
	}
	HC_Edit_Image( m_GreyImageKey, 0, 0, m_width, m_height, m_GreyBuffer );

	HC_Open_Segment_By_Key( m_StandaloneKey );
		HC_Open_Segment( "range" );
			HC_Set_Color( "red" );
			HC_Flush_Contents( ".", "text" );
			HCLOCALE(sprintf( label, "%f", min ));
			HC_Insert_Text( -0.5, -0.85, 0, label );
			HCLOCALE(sprintf( label, "%f", max ));
			HC_Insert_Text( 0.5, -0.85, 0, label );
		HC_Close_Segment();
	HC_Close_Segment();
	HIC_Finish_Picture( nr, swap_buffers );
#else
	UNREFERENCED(nr);
	UNREFERENCED(swap_buffers);
#endif
}


void HDebugZBuffer::Update()
{
#ifdef ENABLE_DEBUG_ZBUFFER

	if( !HC_QShow_Existence( m_StandaloneSegmentName, "self" ))
		return;
	HC_Control_Update( m_StandaloneSegmentName, "redraw everything" );
	HC_Update_One_Display( m_StandaloneSegmentName );
#endif
}


void HDebugZBuffer::ogl_finish_picture(HIC_Rendition const *nr, bool swap_buffers)
{
#ifdef ENABLE_DEBUG_ZBUFFER

	HDebugZBuffer *dzb;

	dzb = (HDebugZBuffer *)(MVO_POINTER_SIZED_INT) HIC_Show_User_Index( nr, H_DEBUG_ZBUFFER_MAGIC );
	if( dzb != 0 )
		dzb->OpenglFinishPicture(nr, swap_buffers);
	else
		HIC_Finish_Picture( nr, swap_buffers );
#else
	UNREFERENCED(nr);
	UNREFERENCED(swap_buffers);
#endif
}


void HDebugZBuffer::RegisterCallbacks()
{
#ifdef ENABLE_DEBUG_ZBUFFER
	HImRegisterCallback("debug zbuffer finish picture", ogl_finish_picture);
#endif
}

