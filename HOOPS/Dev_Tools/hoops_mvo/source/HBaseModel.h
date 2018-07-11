// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HBASEMODEL_H
#define _HBASEMODEL_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"


class HDB;
class HBaseView;
class HModelInfo;
class HStreamFileToolkit;
class HShellVertexData;
class HBhvBehaviorManager;
class HEventManager;
class HOutputHandlerOptions;
class HInputHandlerOptions;
class HBhvKeyframeGenerator;
class HBhvSegmentTracker;
#ifndef SWIG
class TK_Dispatch_XML;
#endif

//! The HBaseModel class is used to store and manage model information
/*! 
  HBaseModel provides functions which facilitate creation and management of the graphical information
  associated with the model.  It is typically referenced by an HBaseView object, which defines a specific view
  of the model.
  
  The class encapsulates a HOOPS Include Library segment.  This segment is intended to be used as the root of
  a HOOPS tree containing segments and geometry which represent graphical information for the
  application-specific model.  A file can be read into it, or geometry can be created within a segment hierarchy
  beneath it. 
*/
class MVO_API HBaseModel
{
 
public:

	/*! Constructs an HBaseModel object. */
	HBaseModel(char const * model = 0);										
	
	/*! Deletes the HOOPS segment tree associated with the model. */
	virtual ~HBaseModel();

	/*! Initializes an HBaseModel object. */
	virtual void Init();	

	/*! Flushes the HOOPS segment tree associated with the HBaseModel object. */
	virtual void Flush();

	/*! \return A boolean indicating whether a Read() has completed.  */
	bool GetFileLoadComplete() const {return m_bFileLoadComplete;};
	/*! Sets the boolean indicating whether a Read() has completed. */
	void SetFileLoadComplete(bool value, HBaseView * pHView=0);

	/*! \return A boolean indicating whether to scan model for annotations and other objects.  */
	bool GetObjectCheck() const {return m_bObjectCheck;};
	/*! Sets the boolean indicating whether to scan model for annotations and other objects. */
	void SetObjectCheck(bool value) {m_bObjectCheck=value;};

	/*! Sets the static model status at the root of the model segment. */
	void SetStaticModel(bool value);
	/*! Returns the existence of a static model at the root of the model segment. */
	bool GetStaticModel() const;
	/*! Returns if a static model is requested at the root of the model segment. */
	bool GetStaticModelOn() const;

	/*! Sets the LMV model status at the root of the model segment. */
	void SetLMVModel(bool value);
	/*! Returns the existence of a LMV model at the root of the model segment. */
	bool GetLMVModel() const;

	/*! \return A boolean indicating whether HBaseModel has called FitWorld on this model after the last call to Read().  It is not useful for model with multiple views.*/
	bool GetFirstFitComplete() const {return m_bFirstFitComplete;};
	/*! Sets the boolean indicating whether the first call to FitWorld has been performed. */
	void SetFirstFitComplete(bool value) {m_bFirstFitComplete=value;};




	/*!
	  Based on file extention this method loads the content of a file 
	  using HInputHandler found in the HIOManager from HDB
	  If no handler is found, it will return InputNotHandled.
	  It does not Flush existing geometry.
	  The default extention is hmf if no other is found.
	  \param FileName A character pointer denoting the name of the file to be read in
	  \param pHView A pointer to the HBaseView object containing the view-dependent information reading in the model 
	  \param clean_include if true, then includes are moved to unique include location after loading
	  \param options An optional pointer to an instance of a HInputHandlerOptions contain extra data for input operation.
	  \return HFileInputResult with result of the attempted file read.
	*/
	virtual HFileInputResult Read(const __wchar_t * FileName, HBaseView * pHView=0, bool clean_include=true, HInputHandlerOptions * options = 0);
#ifdef H_USHORT_OVERLOAD
	virtual HFileInputResult Read(const unsigned short * FileName, HBaseView * pHView=0, bool clean_include=true, HInputHandlerOptions * options = 0){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char*)FileName);
		return Read((__wchar_t*)H_WCS(utf16).encodedText(), pHView, clean_include, options);
	}
#endif
	virtual HFileInputResult Read(const char * FileName, HBaseView * pHView=0, bool clean_include=true, HInputHandlerOptions * options = 0){
		return Read((__wchar_t*)H_WCS(FileName).encodedText(), pHView, clean_include, options);
	}

	/*!
+	  Reads the visualization contents of the file into the HOOPS segment tree (visualization contents refers to graphical information/attributes).  The segment tree will be created underneath the segment denoted by m_ModelKey.
	  The HIO handler registered to handle the specified file extension will be used for import.
	  
	  This function can be overloaded to support reading of additional file types.
	  \param FileName A character pointer denoting the name of the file to write 
	  \param pHView A pointer to the HBaseView object containing the view-dependent information  
	  \param options An optional pointer to an instance of a HOutputHandlerOptions contain extra data for output operation.
	  \return HFileOutputResult with result of the attempted file write.
	*/
	virtual HFileOutputResult WriteWithOptions(const __wchar_t * FileName, HBaseView * pHView=0, HOutputHandlerOptions * options = 0);
#ifdef H_USHORT_OVERLOAD
	virtual HFileOutputResult WriteWithOptions(const unsigned short * FileName, HBaseView * pHView=0, HOutputHandlerOptions * options = 0){
		return WriteWithOptions((__wchar_t*)H_WCS(FileName).encodedText(), pHView, options);
	}
#endif
	virtual HFileOutputResult WriteWithOptions(const char * FileName, HBaseView * pHView=0, HOutputHandlerOptions * options = 0){
		return WriteWithOptions((__wchar_t*)H_WCS(FileName).encodedText(), pHView, options);
	}

	/*!
	  Writes the contents of the HOOPS segment tree to a file.  The HOOPS segment tree beginning with the segment denoted by m_ModelKey will be written.
	  The HIO handler registered to handle the specified file extension will be used for export. 

	  This function should be overloaded to support writing of additional file types.
	  \note for the '.pdf' file extension, a 2D-PDF will be exported by default.   
	  \param FileName A character pointer denoting the name of the file to write 
	  \param pHView A pointer to the HBaseView object containing the view-dependent information
	  \param width For 2D or 'paper' export formats, the m_Window_Width and m_Paper_Width are set to 'width'.   For image formats, the width of the actual image in pixels will be 'width' * m_Image_Dpi.  Default is 0.
	  \param height For 2D or 'paper' export formats, the m_Window_Height and m_Paper_Height are set to 'height'.   For image formats, the height of the actual image in pixels will be 'height' * m_Image_Dpi.  Default is 0.
	  \return HFileOutputResult with result of the attempted file write.
	*/
	virtual HFileOutputResult Write(const __wchar_t * FileName, HBaseView * pHView=0, float width = 0, float height = 0);
#ifdef H_USHORT_OVERLOAD
	virtual HFileOutputResult Write(const unsigned short * FileName, HBaseView * pHView=0, float width = 0, float height = 0){
		return Write((__wchar_t*)H_WCS(FileName).encodedText(), pHView, width, height);
	}
#endif
	virtual HFileOutputResult Write(const char * FileName, HBaseView * pHView=0, float width = 0, float height = 0){
		return Write((__wchar_t*)H_WCS(FileName).encodedText(), pHView, width, height);
	}

	/*! \return A pointer to the HModelInfo object with the current model info. */
	HModelInfo * GetHModelInfo() const;

	/*! \return A pointer to the stream toolkit object used with this model for writing and reading HSF files. */
	virtual HStreamFileToolkit *GetStreamFileTK() const { return m_pHFile; }

	/*! \return The key of the encapsulated HOOPS segment denoting the top of the model hierarchy .*/
	virtual HC_KEY	GetModelKey() const { return m_ModelKey; }

	/*! \return The key of the encapsulated HOOPS segment denoting the top of the model's include segment hierarchy. */
	virtual HC_KEY	GetModelIncludeKey() const { return m_ModelIncludeKey; }

	/*! \return The key of the HOOPS segment containing the model's bounding box geometry. */
	virtual HC_KEY	GetBBoxKey() const { return m_BBoxKey; }

	/*!
	  \return A boolean indicating whether data originated from a CAD dataset.  This is used so that we
	  do not use HOOPS' edges for display but rather the polylines which are provided by the modeller.  This 
	  is set by setting a User_Option of 1 in the root of your scene-graph.  This is automatically set if the 
	  data originated from ACIS or Parasolid and you used the framework to generate the file.
	*/
	virtual bool	IsBRepGeometry() const {return m_bBRepGeometry;};
	/*!  This method indicates if this model represents geometric modeler data but wasn't necessarily loaded from the modelling kernel. Pass true if yes and false if otherwise.*/
	virtual void	SetBRepGeometry(bool brep){m_bBRepGeometry = brep;};

	/*!  \return A boolean indicating whether data originated from a FEA dataset.  Developers ensure that this flag is automatically set if a User_Option of 2 is set in the root node of their scene-graph. */
	virtual bool	IsFEAGeometry() const {return m_bFEAGeometry;};
	/*!  This method indicates if the model represents finite-element data. Pass true if yes and false if otherwise. */
	virtual void	SetFEAGeometry(bool fea){m_bFEAGeometry = fea;};

	/*!  \return A boolean indicating whether the data is only vector data.  This allows HOOPS to force vector printing always and not turn on shading.  This is set by setting a User_Option of 3 in the root of your scene-graph.  */
	virtual bool	IsVectorGeometry() const {return m_bVectorGeometry;}; 
	/*! This method indicates if the model only contains vector data.  Pass true if yes and false if otherwise.*/
	virtual	void	SetVectorGeometry(bool vector){m_bVectorGeometry = vector;};

	/*! Utility function which moves global includes to model specific include segments. */
	virtual void CleanIncludes();

	/*! Scans for all the segments which are included by the model and deletes them. */
	virtual void DeleteIncludedSegments();

	/*! \return A pointer to the HBhvBehaviorManager object. */
	virtual HBhvBehaviorManager * GetBhvBehaviorManager();

	/*! Sets the HBhvBehaviorManager object. */
	void	SetBhvBehaviorManager( HBhvBehaviorManager* BehaviorManager ); 

	/*! \returns A pointer to the HEventManager object. */
	HEventManager *GetEventManager();

	/*! Performs a rendering update. 
	  \param antialias  If true, then an anti-aliased version of the scene will be drawn.
	  \param forceUpdate  If true, then an update will be forced.
	*/	
	void Update(bool antialias = false, bool forceUpdate = false);

	void MakeAnimationSnapshot(int tick);
	virtual HFileOutputResult WriteWithAnimation(const __wchar_t * FileName, HBaseView * pHView=0, float width = 0, float height = 0);
#ifdef H_USHORT_OVERLOAD
	virtual HFileOutputResult WriteWithAnimation(const unsigned short * FileName, HBaseView * pHView=0, float width = 0, float height = 0){
		return WriteWithAnimation((__wchar_t*)H_WCS(FileName).encodedText(), pHView, width, height);
	}
#endif
	virtual HFileOutputResult WriteWithAnimation(const char * FileName, HBaseView * pHView=0, float width = 0, float height = 0){
		return WriteWithAnimation((__wchar_t*)H_WCS(FileName).encodedText(), pHView, width, height);
	}

	virtual void CreateIncludeSegment();

	/*! \return A pointer to the DispatchXML object which allows registering of multiple xml handlers. */	
	TK_Dispatch_XML * GetDispatchXML();

protected:

	HC_KEY      m_ModelKey;			/*!< Key of the HOOPS segment denoting the top of model hierarchy. */											
	HC_KEY      m_ModelIncludeKey;	/*!< Key of the HOOPS segment denoting the top of model include hierarchy. */											
	HC_KEY      m_BBoxKey;			/*!< Key of the HOOPS segment containing model's bounding box geometry. */


	/*! A pointer to the HStreamFileToolkit object. */
	HStreamFileToolkit * m_pHFile;

	/*! A pointer to the HModelInfo object. */
	mutable HModelInfo * m_pModelInfo;

	bool m_bBRepGeometry;	/*!< Set this value to true if the model represents geometric modeler data but wasn't necessarily loaded from the modelling kernel. */
	bool m_bFEAGeometry;    /*!< Set this value to true if the model represents finite-element data. */
	bool m_bVectorGeometry; /*!< Set this value to true if the model only contains vector data. */
	bool m_bNeedsStencilBuffer;  /*!< Set this value to true if the model contains a CuttingAnimation XML behavior (see: StencilProbe). */

	/*! Utility member intended to be set to true if the model is being streamed in and the 
	    file has finished loading in. */
	bool m_bFileLoadComplete; 
	
	/*! Utility member intended to be set to true if the model is to be scanned for annotations and other "objects" */
	bool m_bObjectCheck;


	/*! Utility member intended to be set to true if the model is being streamed in and the first
	    'fit scene to camera' has occurred. */
	bool m_bFirstFitComplete; 

	/*! A pointer to the HBhvBehaviorManager object. */
	HBhvBehaviorManager * m_pBhvBehaviorManager;

	/*! A pointer to the HEventManager object. */
	HEventManager * m_pEventManager;

#ifndef SWIG
	/*! Pointer to the Dispatch XML object. */
	TK_Dispatch_XML *m_pDispatchXML;
#endif

	HBhvKeyframeGenerator *m_pKeyframeGenerator;
	HBhvSegmentTracker *m_pSegmentTracker;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
