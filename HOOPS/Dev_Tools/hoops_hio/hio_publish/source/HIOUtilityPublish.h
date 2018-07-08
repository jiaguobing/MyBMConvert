// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYPUBLISH_H
#define _HIOUTILITYPUBLISH_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "vlist.h"
#include "varray.h"

#ifdef HIO_PUBLISH
#  ifdef _MSC_VER
#	 define HIO_API __declspec (dllexport)
#  else
#	 define HIO_API __attribute__ ((visibility ("default")))
#  endif
#else
#  define HIO_API
#endif


enum HPDFLayoutImageType
{
	HJPG = 0,        
	HPNG = 1,    
};

enum HPDFLayoutVisibility 
{
	HUnchanged = 0,        
	HVisibile = 1,    
	HInvisible = 2,      
};


class HPDFField
{
public:
	HPDFField()
	{
		m_text = 0;
		m_name = 0;
		m_imagename = 0;
		m_javascript = 0;
		m_visibility = HUnchanged;
	}
	~HPDFField()
	{
		delete [] m_text;
		delete [] m_name;
		delete [] m_imagename;
		delete [] m_javascript;
	}

	void Set(const wchar_t *name, const wchar_t *text, const wchar_t *javascript = 0, HPDFLayoutVisibility visibility = HUnchanged)
	{
		if (name)
		{
			m_name = new wchar_t[wcslen(name)+1];
			wcscpy(m_name, name);
		}
	
		if (text)
		{
			m_text = new wchar_t[wcslen(text)+1];
			wcscpy(m_text, text);
		}
		if (javascript)
		{
			m_javascript = new wchar_t[wcslen(javascript)+1];
			wcscpy(m_javascript, javascript);
		}
		
		m_visibility = visibility;
	}

	void SetImage(const wchar_t *name,const wchar_t *imagename, HPDFLayoutImageType imagetype, int imagewidth, int imageheight)
	{
		if (name)
		{
			m_name = new wchar_t[wcslen(name)+1];
			wcscpy(m_name, name);
		}

		if (imagename)
		{
			m_imagename = new wchar_t[wcslen(imagename)+1];
			wcscpy(m_imagename,imagename);
		}

		m_imagetype = imagetype;
		m_imagewidth = imagewidth;
		m_imageheight = imageheight;
	}

	wchar_t const *GetText() { return m_text; }
	wchar_t const *GetImage() { return m_imagename; }
	wchar_t const *GetName() { return m_name; }
	wchar_t const *GetJavascript() { return m_javascript; }
	HPDFLayoutVisibility GetVisibility() { return m_visibility; }
	HPDFLayoutImageType GetImageType() { return m_imagetype; }
	int GetImageWidth() { return m_imagewidth; }
	int GetImageHeight() { return m_imageheight; }



private:

	wchar_t *m_text;
	wchar_t *m_imagename;
	
	wchar_t *m_name;
	wchar_t *m_javascript;
	HPDFLayoutVisibility m_visibility;
	HPDFLayoutImageType m_imagetype;
	int m_imagewidth, m_imageheight;
};


/*!The HPDFLayoutManager class allows for modifications of an existing PDF template
*/

class HIO_API HPDFLayoutManager
{
public:
	/*! Constructs an HPDFLayoutManager object. */
	HPDFLayoutManager()
	{
		m_templatefile = 0;
		m_3DFieldName = 0;
		m_FieldList = new_vlist(malloc,free);

	}

	/*! HPDFLayoutManager destructor. */
	~HPDFLayoutManager()
	{
		START_LIST_ITERATION(HPDFField, m_FieldList);
			delete temp;			
		END_LIST_ITERATION(m_FieldList);
		delete_vlist(m_FieldList);
		delete [] m_templatefile;
		delete [] m_3DFieldName;
	}

	/*! This method sets the filename of the template to modify
		\param name Fully qualified path of PDF template
	*/
	void SetTemplateName(const wchar_t *name)
	{
		if (name)
		{
			m_templatefile = new wchar_t[wcslen(name)+1];
			wcscpy(m_templatefile, name);
		}
	}
	/*! This method sets the name of the PDF field which serves as the 3D container 
		\param name Name of PDF field
	*/
	void Set3DFieldName(const wchar_t *name)
	{
		if (name)
		{
			m_3DFieldName = new wchar_t[wcslen(name)+1];
			wcscpy(m_3DFieldName, name);
		}
	}


	/*! This method allows the user to attach javascript to a field and change its visibility
		\param name Name of PDF form field
		\param javascript Javascript code to attach to this PDF form field 
		\param visibility New visibility setting for this PDF form field.
	*/
	void AddMiscModification(const wchar_t *name, wchar_t *javascript = 0, HPDFLayoutVisibility visibility = HUnchanged)
	{
		HPDFField * field = new HPDFField;
		field->Set(name, 0, javascript, visibility);
		vlist_add_last(m_FieldList, field);
	}

	/*! This method allows the user to change the text of a PDF form field
		\param name Name of PDF form field
		\param text New text of PDF form field 
		\param isText Specifies if PDF form field to modify is text field or button
	*/
	void AddTextModification(const wchar_t *name, const wchar_t *text)
	{
		HPDFField * field = new HPDFField;
		field->Set(name, text, 0, HUnchanged);
		vlist_add_last(m_FieldList, field);
	}


	/*! This method allows the user to change the image of a PDF form field
		\param name Name of PDF form field
		\param imagename Full path to the image
		\param imagetype Type of image 
		\param imagewidth Width of Image
		\param imageheight Height of Image
	*/
	void AddImageModification(const wchar_t *name, const wchar_t *imagename, HPDFLayoutImageType imagetype, int imagewidth, int imageheight)
	{
		HPDFField * field = new HPDFField;
		field->SetImage(name, imagename, imagetype, imagewidth, imageheight);
		vlist_add_last(m_FieldList, field);
	}


	/*! \return A pointer to the list of field modifications. */
	vlist_s *GetFieldModificationList()
	{
		return m_FieldList;
	}
	/*! \return A pointer to the template name */
	wchar_t  *GetTemplateName()
	{
		return m_templatefile;
	}
	/*! \return A pointer to the PDF field name for the 3d object. */
	wchar_t  *Get3DFieldName()
	{
		return m_3DFieldName;
	}

private:
	wchar_t  * m_templatefile;
	wchar_t  * m_3DFieldName;
	struct vlist_s *		m_FieldList;

};


class HPublishExchangeView;
typedef VArray<HPublishExchangeView *> HPEViewsArray;

/*! The HIOUtilityPublish class is used for exporting either the HOOPS scene graph or an existing prc model as a 3D PDF. It requires an installation of HOOPS Publish.

*/
class HIO_API HIOUtilityPublish: public HInputHandler, public HOutputHandler
{

public:

	/*!Constructs an HIOUtility object.*/
	HIOUtilityPublish();
	virtual ~HIOUtilityPublish() { ReleaseJavascript(); };

	/*!This method registers the appropriate extensions to this handler as an input handler.*/
	void RegisterInputHandlerTypes();
	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityPublish"; }
	const char * GetInputName() { return "HIOUtilityPublish"; }

	/*!	\returns The input extension type that is handled by HIOUtilityPublish.*/
	const char * GetInputTypesString() {return ""; }
 
		

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*!
	  This method reads the modeller file 'filename' into the segment tree underneath key.
		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options An options class containing any needed extra information for operation.
		\return The #HFileInputResult result of the input operation.
	*/
	HFileInputResult FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options);

	HFileInputResult FileInputByKey(const char * FileName, HC_KEY key, HInputHandlerOptions * options){
		return FileInputByKey(H_WCS(FileName).encodedText(), key, options);
	};

	HFileInputResult FileInputByKey(const unsigned short * FileName, HC_KEY key, HInputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) FileName);
		return FileInputByKey(H_WCS(utf16).encodedText(), key, options);
	};

	HFileOutputResult FileOutputByKey(const wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) filename);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}

	HFileOutputResult FileOutputByKey(const char * filename, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey(H_WCS(filename).encodedText(), key, options);
	}

	void RegisterOutputHandlerTypes();
	
	/*!
		!This method creates a A3DAsmModelFile object from a HOOPS segment tree 
		\param options Output Handler Options. The m_pHBaseView must be set
		\param key Start Segment Key
 		\return A3DAsmModelFile pointer
	*/
	virtual void *BuildPRCModel(HOutputHandlerOptions * options, HC_KEY key);
 

	const char *  GetOutputTypesString() {
		return "prc;iges;step;x_t;stl";
	}

	HOutputHandlerStyle GetOutputStyle() {
		return HOutputHandlerStyleModel;
	}
	/*!
		This method creates a PDF file from a prc model
		\param file_name Name of output file
		\param PRCModel A3DAsmModelFile pointer
		\param options PDF Options
		\param javascript_file JavaScript to attach to the 3d object
		\param array of additional views
 	*/
	virtual HFileIOResult WriteSimplePDF(wchar_t const * file_name, void *PRCModel, HOutputHandlerOptions * options, wchar_t const *javascript_file, const HPEViewsArray& hoopsviews);
/*!
		This method creates a PDF file from an existing template
		\param file_name Name of output file
		\param PRCModel A3DAsmModelFile pointer
		\param layoutmanager Pointer to HPDFLayoutManager object which allows for modification of PDF fields
		\param javascript_file JavaScript to attach to the 3d object
 	*/	
	virtual HFileIOResult WritePDFFromTemplate(wchar_t const * file_name, void *PRCModel, HOutputHandlerOptions * options, HPDFLayoutManager * layoutmanager, wchar_t const *javascript_file);	 

/*!
		This method creates javascript code from an animation stored in the HBhvBehaviorManager of the HBaseView object. Make sure to call ReleaseJavascript to free memory		
		\param file_name 
		\param PRCModel A3DAsmModelFile pointer
		\param layoutmanager Pointer to HPDFLayoutManager object which allows for modification of PDF fields
		\param javascript_file JavaScript to attach to the 3d object
 	*/	
	char * GenerateJavascript(HBaseView *view);
/*!
		This method frees the buffer allocated by a previous call to GenerateJavascript
 	*/	
	void ReleaseJavascript();

private:
	char *m_pAnimationData;

};





/*!The H3DXOptions creates a PRC file from an existing HOOPS segment tree. It requires an installation of HOOPS 3D Exchange.
*/

class HIO_API H3DXOptions
{
public:
	/*! Constructs an H3DXOptions object. */
	H3DXOptions() {Reset();};
	/*! This method sets this class to a default state. */
	void Reset(){
		m_bSaveToPDF = false;
		m_pAsmModelFile = 0;
	};

 	bool	m_bSaveToPDF;
	void *	m_pAsmModelFile;
};




#ifdef HIO_PUBLISH
extern "C" {
	HIO_API void * CreateInput(HIOManager *manager);
	HIO_API void * CreateOutput(HIOManager *manager);
	HIO_API void * CreateConnector(void * unused);
	HIO_API void Free(HIOUtilityPublish *);
 }


#endif

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
