// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HIOUtilityHsf.h"
#include "HStream.h"
#include "HUtilityLocaleString.h"
#include "HBaseView.h"
#include "HBaseModel.h"
#include "HBhvUtility.h"
#include "HDispatchXML.h"

#include <ctype.h>

#if defined(WIN64) || defined(_M_X64) || defined(_WIN64)
#       define HLONG __int64
#else
#       define HLONG long
#endif

class TK_MVO_Initial_View : public HTK_Camera 
{
public:
	TK_MVO_Initial_View(HBaseView* view) : HTK_Camera(TKE_View) { m_pHBaseView = view; }

	TK_Status Interpret(BStreamFileToolkit &tk, ID_Key key, int variant) {
		UNREFERENCED(tk);
		UNREFERENCED(key);
		UNREFERENCED(variant);

		HPoint pos, tar, up;
		float fieldx=0, fieldy=0;
		char proj[MVO_SMALL_BUFFER_SIZE];
		int ob_offset = 0;
		float oblique_x=0.f, oblique_y=0.f;
		char* ctmp=0;

		m_pHBaseView->GetCamera(&pos,&tar,&up,fieldx,fieldy,proj);

		m_settings[0] = pos.x; m_settings[1] = pos.y; m_settings[2] = pos.z;
		m_settings[3] = tar.x; m_settings[4] = tar.y; m_settings[5] = tar.z;
		m_settings[6] = up.x ; m_settings[7] = up.y ; m_settings[8] = up.z;
		m_settings[9] = fieldx ; m_settings[10] = fieldy;

		if(strstr(proj, "oblique"))
		{
			ob_offset=8;
			ctmp=strstr(proj, "(");
			HCLOCALE(sscanf(++ctmp, "%f, %f", &oblique_y, &oblique_x));
			m_details[0]=oblique_y;
			m_details[1]=oblique_x;
		}

		switch ((int) proj[ob_offset]) 
		{
			case 'o': m_projection = TKO_Camera_Orthographic; break;
			case 'p': m_projection = TKO_Camera_Perspective; break;
			case 's': m_projection = TKO_Camera_Stretched; break;
			default: m_projection = TKO_Camera_Stretched; break;
		}
		
		if(ob_offset)
			m_projection|=TKO_Camera_Oblique_Mask;

		SetView("default");

		return TK_Normal;
	}

	TK_Status Execute(BStreamFileToolkit & tk) {
		UNREFERENCED(tk);

		TK_Status status=TK_Normal;

		HPoint pos, tar, up;
		float fieldx, fieldy;

		pos.Set(m_settings[0],m_settings[1],m_settings[2]);
		tar.Set(m_settings[3],m_settings[4],m_settings[5]);
		up.Set(m_settings[6],m_settings[7],m_settings[8]);
		fieldx = m_settings[9];
		fieldy = m_settings[10];

		char proj[16];

		switch ((int) m_projection) {
			case TKO_Camera_Orthographic:	strcpy (proj, "o");	break;
			case TKO_Camera_Perspective:	strcpy (proj, "p");	break;
			default: strcpy (proj, "s"); break;
		}


		if (!strcmp(GetView(),"default"))
		{
			m_pHBaseView->SetCamera(pos,tar,up,fieldx,fieldy,proj);
			m_pHBaseView->CameraPositionChanged();
			m_pHBaseView->HasInitialView(true);
		}

		return status;
	};

private:
	HBaseView* m_pHBaseView;

};


// W3D_Image class definition taken from partviewer's CSolidHoopsView.cpp,h renamed to HW3D_Image
class HW3D_Image : public BBaseOpcodeHandler
{
    private:
        HW3D_Image(const HW3D_Image&);
        HW3D_Image& operator=(const HW3D_Image&);

    protected:

        char *                  m_name;         /*!< internal use; name applied to image (if any) */
        int                     m_name_length;  /*!< internal use; Length of name */
        int                     m_size[2];
        char                    m_bpp;

        //! internal use
        void    set_name (char const * string);
        //! internal use
        void    set_name (int length);

    public:
        /*! constructor */
        HW3D_Image () : BBaseOpcodeHandler ((unsigned char)TKE_HW3D_Image), m_name (0), m_name_length (0), m_bpp(0)
        { m_size[0] = m_size[1] = 0; }
        ~HW3D_Image();

        TK_Status   Read (BStreamFileToolkit & tk);
        TK_Status   Write (BStreamFileToolkit & tk);
		TK_Status   Execute (BStreamFileToolkit & tk);
        void        Reset ();

        /*! Sets the name of the image.  Allocates the buffer and copies the string */
        void            SetName (char const * string)                   { set_name (string);        }
        /*! Sets the name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
        void            SetName (int length)                            { set_name (length);        }
        /*! Returns the image name string */
        char const *    GetName () const                                { return m_name;            }
        /*! Returns the image name string buffer, which may be modified directly */
        char *          GetName ()                                      { return m_name;            }

        void            SetSize (int x, int y)                          { m_size[0] = x; m_size[1] = y ;}
        int const*      GetSize () const                                { return m_size;            }
        int *           GetSize ()                                      { return m_size;            }

        void            SetBitDepth(char bpp)                           { m_bpp = bpp;              }
        char const      GetBitDepth () const                            { return m_bpp;            }
        char            GetBitDepth ()                                  { return m_bpp;            }

};


HW3D_Image::~HW3D_Image() {
    delete [] m_name;
}


TK_Status HW3D_Image::Write (BStreamFileToolkit & tk) {
#ifndef BSTREAM_READONLY
    TK_Status       status = TK_Normal;

    switch (m_stage) {
        case 0: {
            if ((status = PutOpcode (tk)) != TK_Normal)
                return status;
            m_stage++;
        }   //nobreak;

        case 1: {

            if (m_name_length > 0) {
                unsigned char       byte = (unsigned char)m_name_length;
                // need handling for string > 255 ?
                if ((status = PutData (tk, byte)) != TK_Normal)
                    return status;
            }
            m_stage++;
        }   //nobreak;

        case 2: {
            if (m_name_length > 0) {
                if ((status = PutData (tk, m_name, m_name_length)) != TK_Normal)
                    return status;
            }
            m_stage++;
        }   //nobreak;

        case 3:
        {
            if ((status = PutData(tk, m_size[0])) != TK_Normal)
                return status;
            if ((status = PutData(tk, m_size[1])) != TK_Normal)
                return status;
            if ((status = PutData(tk, m_bpp)) != TK_Normal)
                return status;
            m_stage++;
            //nobreak;
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
#else
    return tk.Error (stream_readonly);
#endif
}


TK_Status HW3D_Image::Read (BStreamFileToolkit & tk) {
    TK_Status       status = TK_Normal;

    switch (m_stage) {

        case 0: {
                unsigned char       byte;
                if ((status = GetData (tk, byte)) != TK_Normal)
                    return status;
                set_name ((int)byte);

            m_stage++;
        }   //nobreak;

        case 1: {

            if (m_name_length > 0) {
                if ((status = GetData (tk, m_name, m_name_length)) != TK_Normal)
                    return status;
            }
            m_stage++;
        }   //nobreak;

        case 2:
        {
            if ((status = GetData(tk, m_size[0])) != TK_Normal)
                return status;
            if ((status = GetData(tk, m_size[1])) != TK_Normal)
                return status;
            if ((status = GetData(tk, m_bpp)) != TK_Normal)
                return status;
            m_stage=-1;
            break;
        }

        default:
            return tk.Error();
    }

    return status;
}


TK_Status   HW3D_Image::Execute (BStreamFileToolkit & tk)
{
	UNREFERENCED(tk);

	return TK_Normal;
}

void HW3D_Image::Reset () {

    delete [] m_name;
    m_name = 0;

    BBaseOpcodeHandler::Reset();
}

void HW3D_Image::set_name (int length) {
    m_name_length = length;
    delete [] m_name;
    m_name = new char [m_name_length + 1];
    m_name[m_name_length] = '\0';
}

void HW3D_Image::set_name (char const * name) {
    if( name != 0 ) {
        set_name ((int) strlen (name));
        strcpy (m_name, name);
    }
    else {
        delete [] m_name;
        m_name = 0;
        m_name_length = 0;
    }
}


#if 0
class TK_MVO_Image : public HTK_Image {
private:
	HBaseView*	m_pHBaseView;	// view(?) will hold our map

public:
	TK_MVO_Image (HBaseView * view) : HTK_Image() m_pHBaseView (view) {}

	TK_Status Interpret (BStreamFileToolkit &tk, ID_Key key, int variant) {
		HIOManager *	manager = HIOManager::GetCurrentHIOManager();
		TK_Status		status;
		char			token[64];

		// let default set up stuff like size, position, etc
		if ((status = HTK_Image::Interpret (tk, key, variant)) != TK_Normal)
			return status;

		// now see if we have jpeg data to use instead
		HC_Open_Geometry (image_key);
			HC_Show_One_User_Options ("hoops mvo jpeg image reference", token);
		HC_Close_Geometry ();

		if (manager->LookupJPEG (token, &size, &buffer))
			SetBytes (size, buffer, TKO_Compression_JPEG);

		return TK_Normal;
	}

	TK_Status Execute (BStreamFileToolkit & tk) {
		if ((status = HTK_Image::Execute (tk)) != TK_Normal)
			return status;

		if (GetCompression() == TKO_Compression_JPEG) {
			//  save away jpeg data
			HIOManager *	manager = HIOManager::GetCurrentHIOManager();
			HC_KEY			image_key;
			char			string[256];
			char			token[64];

			unsigned char *	buffer;
			unsigned int	size;

			size = m_work_area.Used();
			buffer = new unsigned char [size];
			memcpy (buffer, m_work_area.Buffer(), size);

			manager->SaveJPEG (size, buffer, token);

			sprintf (string, "hoops mvo jpeg image reference=%s", token);

			tk.GetLastKey (&image_key);
			HC_Open_Geometry (image_key);
				HC_Set_User_Options (string);
			HC_Close_Geometry ();
		}
	}
}
#endif

void HIOUtilityHsf::RegisterInputHandlerTypes() {
	HIORegisterInputType("hsf",this);
};

const char * HIOUtilityHsf::GetInputTypesString() {
	return "hsf";
};
static bool progress_callback (unsigned HLONG so_far, unsigned HLONG expected, void * user_data)
{
	float percent=(float)so_far/(float)expected;

	((HIOUtilityHsf*)user_data)->ReportInputPercentProgress(percent);

	return true;
}

HFileInputResult HIOUtilityHsf::FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options)
{
	if(!FileName) return InputBadFileName;
	if(key == INVALID_KEY) return InputFail;

	HStreamFileToolkit * tk;
	HStreamFileToolkit * cleanup_tk = 0;
	
	if(options && options->m_tk)
		tk=options->m_tk;
	else {
		tk = new HStreamFileToolkit();
		cleanup_tk = tk;
	}

	if(options)
	{
		if(options->m_pHBaseView && tk!=options->m_tk) 
		{
			tk->SetOpcodeHandler (TKE_HW3D_Image, new HW3D_Image());
			tk->SetOpcodeHandler (TKE_View, new TK_MVO_Initial_View(options->m_pHBaseView));
			tk->SetPrewalkHandler(new TK_MVO_Initial_View(options->m_pHBaseView));
			HBhvUtility::SetupAnimationReadFromHSF(tk, options->m_pHBaseView->GetModel());
			tk->SetOpcodeHandler (TKE_XML,  options->m_pHBaseView->GetModel()->GetDispatchXML()->MakeCopy());
		}
	}

	tk->SetProgressCallback(progress_callback);
	tk->SetProgressValue(this);

	SetStartingInput((char*)H_UTF8(FileName).encodedText());

	HC_Open_Segment_By_Key(key);
		TK_Status status = HTK_Read_Stream_File (FileName, tk); 
	HC_Close_Segment();

	SetFinishedInput();
	HFileInputResult result = InputOK;
	if (status == TK_Error) {
		result = InputFail;

		//see if it was because of a bad file name
		status = tk->OpenFile(FileName);
		if (status == TK_Error)
			result = InputBadFileName; //error was the result of a missing or unopenable (e.g. because of permissions) file 
		else
			tk->CloseFile (); //error must have been caused by something else 
	}
	else if (status == TK_LibraryNotFound) {
		result = InputLibraryNotFound;
	}
	else if (status == TK_Version)
		result = InputVersionMismatch;

	if(cleanup_tk)
		delete tk;

	return result;
}

void HIOUtilityHsf::RegisterOutputHandlerTypes() {
	HIORegisterOutputType("hsf",this);
};

const char * HIOUtilityHsf::GetOutputTypesString() { 
	return "hsf";
};


HFileOutputResult HIOUtilityHsf::FileOutputByKey(const wchar_t * FileName, HC_KEY key,  HOutputHandlerOptions * options)
{
	if(!FileName) return OutputBadFileName;
	if(key == INVALID_KEY) return OutputFail;
	return FileOutputCommon(FileName, key, options);
}


HFileOutputResult HIOUtilityHsf::FileOutputCommon(const wchar_t * FileName, HC_KEY key,  HOutputHandlerOptions * options)
{
	HStreamFileToolkit *tk = 0;
	bool alloc_tk = false;
	bool found;
	TK_Status	status;

	SetStartingOutput();

	if(options->m_pExtendedData)
		tk = (HStreamFileToolkit*)options->m_pExtendedData;
	
	if(!tk && options->m_pHBaseView){
		tk = options->m_pHBaseView->GetModel()->GetStreamFileTK();
		tk->Restart();
	}

	if(!tk) {
		tk = new HStreamFileToolkit();
		alloc_tk = true;
		tk->Restart();
	}

	if (options->m_bWriteAnimations)
		HBhvUtility::SetupAnimationWriteFromHSF(tk,options->m_pHBaseModel);
		
		
	if(options->m_Version > 0)
		tk->SetTargetVersion(options->m_Version);

	do {
		found = false;
		HC_Open_Segment_By_Key(key); {
			HC_Begin_Contents_Search("...", "streaming mode"); {
				HC_KEY search_key = 0;
				char search_type[MVO_BUFFER_SIZE];
				if (HC_Find_Contents(search_type, &search_key)) {
					found = true;
					HC_Optimize_Segment_Tree_By_Key(search_key, "reorganize=(color=on)");
				}
			} HC_End_Contents_Search();
			if (HC_Show_Existence ("streaming mode")) {
				HC_UnSet_Streaming_Mode ();
			}
		} HC_Close_Segment();
	} while (found);
	

	HC_Open_Segment_By_Key(key);
		status = HTK_Write_Stream_File (FileName, tk);
	HC_Close_Segment();

	if(alloc_tk)
		delete tk;

	//TODO report something for InputBadFileName
	HFileOutputResult result = OutputOK;

	if (status == TK_Error)
		result = OutputFail;

	SetFinishedOutput();

	return result;
}









