// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HUTILITYXMLPARSER_H
#define _HUTILITYXMLPARSER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "HTools.h"

class HUtilityXMLParser;
class HUtilityTextParser;

//! The HUtilityXMLProperty class encapsulates an XML property item

class MVO_API HUtilityXMLProperty
{
public:

	/*!
	Constructs an HUtilityXMLProperty object
  	*/

	HUtilityXMLProperty()
	{
		m_pText = 0;
	}
	virtual ~HUtilityXMLProperty()
	{
		delete [] m_pText;
	}
 	char *		m_pText;	    /*!< Property Name */
	bool		m_HasQuotes;	    /*!< Quoted Property */
};



//! The HUtilityXMLProperty class encapsulates an XML tag 
class MVO_API HUtilityXMLTag
{
public:

	/*!
	Constructs an HUtilityXMLTag object
	\param xmlparser Pointer to XML Parser for this tag
  	*/

	HUtilityXMLTag(HUtilityXMLParser *xmlparser = 0);
	virtual ~HUtilityXMLTag();

	/*!
	Add Float Property Item
	\param name Name of Property
 	\param f Property Data
	\return false = property already exists, true = property does not exist
    */
	bool AddPropertyFloat(const char *name, float f);
	/*!
	Add Integer Property Item
	\param name Name of Property
 	\param i Property Data
	\return false = proeprty already exists, true = property does not exist
    */
	bool AddPropertyInt(const char *name, int i);
	/*!
 		Returns whether the tag has list items
     */
	bool HasListItems();
	/*!
	Return next property item
	\param propname Itemname (returned)
 	\param prop Property (returned)
	\return false = no property item available, true = success
     */
	bool GetNextPropertyItem(char **propname, HUtilityXMLProperty **prop);
	/*!
	Initiate query for property items
     */
	void StartPropertyQuery();
	/*!
	Add list item to tag object
 	\param text item data to add
    */
	void AddListItem(const char *text);
	/*!	Returns true if more list items are available */
	bool HasMoreItems();
	/*!
	Get Property by Name
 	\param name Property name to search for
  	\param prop Property (returned)
	\return true = Property found, false = property not found
     */
   	bool GetProperty(const char *name, HUtilityXMLProperty **prop);


	bool GetProperty(const char *name, bool &ii);

	/*!
	Get Property by Name
 	\param name Property name to search for
  	\param pcont Property string(returned)
	\return true = Property found, false = property not found
     */
	bool GetProperty(const char *name, char *pcont);
	/*!
	Get Property by Name
 	\param name Property name to search for
  	\param value Property value (returned)
	\return true = Property found, false = property not found
     */
	bool GetProperty(const char *name, int &value);
	/*!
	Add new Property
 	\param name Property name to add
  	\param text Property data
	\param hasQuotes true = quotes are added to property, false = no quotes are added to property
 	\return true = property does not exist, false = property already exists
     */
	bool AddProperty(const char *name, const char *text, bool hasQuotes = true);
	/*!
	Initiate query for list items
     */
 	void StartListItemsQuery();
	/*!
	Return next list item
	\param text List item data (returned)
 	\return false = no list item available, true = success
     */
	bool GetNextListItem(char **text);
	/*!
	Set name of tag
	\param tagname Name of tag
    */
	void SetTagname(const char *tagname);
	/*! Returns the name of tag */
	char * GetTagname() { return m_pTagName; }
	/*!
	Free internal memory
     */
	void FreeMemory();
	/*! Initialize the object */
	void Init();
	/*! Returns the open flag */
	bool GetOpen() { return m_bOpen; }
	/*!
	Set flag indicating if tag needs to be kept open to process subtags
 	\param o true = tag reamins open, false = tag will be closed
     */
	void SetOpen(bool o) { m_bOpen = o; }		
	/*!
	Deletes all properties from tag object
    */
	void CleanupPropertyList();
	/*!	Returns a pointer to the HUtilityXMLParser object associated to this tag object */
	HUtilityXMLParser *GetXMLParser() { return m_pXMLParser; }	
	HUtilityXMLTag* Clone();

	void AddComment(char *comment);
	
	struct vlist_s *GetCommentList() { return m_pCommentList; }
	void CleanListItems();

private:
	struct vhash_s *		m_pPropertyHash;
	struct vlist_s *		m_pPropertyList;
	struct vlist_s *		m_pItemList;
	struct vlist_s *		m_pCommentList;

	HUtilityXMLParser *		m_pXMLParser;
	char *					m_pTagName;
	bool					m_bOpen;
};


//! The HUtilityXMLTagCallback class holds a pointer to the XML Tag Callback and is used internally
class HUtilityXMLTagCallback
{
public:
 	void*					(*m_pTagCallback)(HUtilityXMLTag *, bool open, void *m_pExtraData); 	/*!< Callback pointer */
	void					*m_pExtraData;		/*!< Extra Callback Data */
};



//! The HUtilityXMLParser class decodes arbitrary XML data
/*!	
	HUtilityXMLParser class implements a simple non-validating XML Parser. It uses a callback 
	architecture to register functions that handle specific tags 
	and these functions in turn query the parsing toolkit for their parameters.
*/ 
class MVO_API HUtilityXMLParser  
{
public:

	/*!	Constructs an HUtilityXMLParser object */
	HUtilityXMLParser();
	virtual ~HUtilityXMLParser();

	/*! Resets the reader for processing of data */
 	void Restart();

	/*!
	Takes complete chunk of XML data and processes it. 
	\return false = data is invalid
   	*/
 	bool ProcessXMLData();

 	/*!
	Associate user defined callback to tagname
	\param tagname Name of tag to associate with callback
	\param callbackfunction  Pointer to callback function. 
	\param extradata Arbitrary pointer that will be passed to callback
 	\return true = callback has not been set, false = callback already set
   	*/
 	bool	SetTagCallback(const char *tagname, void *(*callbackfunction)(HUtilityXMLTag *, bool open, void *extradata), void *extradata);
 
	/*!
	Prepares memory buffer for parsing
	\param buffer memory buffer to parse
   	*/
	void SetupParseBuffer(const char *buffer);
	/*!
	\return Stack data which can be retrieved during parsing
   	*/
	void *GetStackData();
	/*!
	Returns document type information
	\param doctype Document Type (returned)
	\param parsebuffer Memory buffer to parse for buffer (or if 0 use already prepared buffer)
	\return true = doctype found, false = no doctype found
    	*/
	bool FindDocType(char *doctype, const char *parsebuffer = 0);
	/*! 
	Extracts point from string
	\param tp Pointer to text parser object
	\param point Returns extracted point
 	*/
	static void GetFloatPoint(HUtilityTextParser *tp, HPoint &point);

	/*! 
	Extracts point from string
	\param text Text to extract point info from
	\param point Returns extracted point
 	*/
	static void GetFloatPoint(const char *text, HPoint &point);
	static void GetRBGA(const char *text, float &r, float &g, float &b, float &a);
	

private:	
	HUtilityTextParser *m_pTextParser;
	void AddListItems(HUtilityXMLTag *xmltag);
	int  ProcessSubtags(HUtilityXMLTag *xmltag);
	int AddProperties(HUtilityXMLTag *xmltag);
	int SkipSpecialTags(HUtilityXMLTag *xmltag);
	void DoCallback(HUtilityXMLTag *xmltag, bool openclose);
 
	struct vhash_s * tagcallback_hash;
	struct vlist_s * stackdata_list;
	HUtilityXMLTag * m_pInvalidTag;

};



//! 	HUtilityXMLGenerator allows the creation of valid XML blocks. 
class MVO_API HUtilityXMLGenerator
{
public:	
	
	/*!	Constructs an HUtilityXMLGenerator object */
	HUtilityXMLGenerator();
	virtual ~HUtilityXMLGenerator();

	/*!
	Encode XML Tag
 	\param xmltag tag object to encode
   	*/
	void AddTag(HUtilityXMLTag *xmltag);
	/*!	Reset XML Generator */	
	void Reset();	

	/*!
	Close specified tag (ends subtag processing)
	\param xmltag tag to close
   	*/
	void CloseTag(HUtilityXMLTag *xmltag);
	/*! Returns a pointer to the buffer containing encoded XML data */
	char *GetBuffer() { return m_pTextBuffer; }
	/*!	Returns the size of XML Buffer */
	unsigned int GetBufferSize() { return m_BufferPos; }

	/*!
	Finalize encoding of XML data
    */
	void Finalize();
	void WriteToBuffer(const char *text);

private:

 	void WriteToBufferInternal(const char *text, int len);
 	void WriteTagname(const char *name);
	void WriteComment(const char *name);
	void WriteProperty(const char *name, HUtilityXMLProperty *xmlprop);
	void WriteListItems(HUtilityXMLTag *xmltag);
	void WriteEndtag(const char *name);
	void WriteNewline(int extra = 0);


	bool				m_bFirstTag;
	int					m_Intend;
	char *				m_pTextBuffer;
	unsigned int		m_BufferSize;
	unsigned int		m_BufferPos;



 
};

 


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif


