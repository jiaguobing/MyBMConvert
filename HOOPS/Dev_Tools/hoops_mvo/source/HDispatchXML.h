// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HDispatchXML.h
	Defines datatypes associated with the TK_Dispatch_XML class  
*/
#ifndef _HDISPATCHXML_H
#define _HDISPATCHXML_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"


//! The TK_Dispatch_XML class stores a list of XML Handlers.  
/*! This class allows more than one handler to be registered at a time so that 
multiple chunks of XML data can be read in and written out. 
*/

class MVO_API TK_Dispatch_XML : public TK_XML
{

 
public:

	/*!
	Constructs a TK_Dispatch_XML object.
	\param model A pointer to HBaseModel object.
 	*/
	TK_Dispatch_XML(HBaseModel * model);
	~TK_Dispatch_XML();
 
	/*! This method iterates through the list of registers handlers invokes their Interpret method.
	\param tk A reference to the BStreamFileToolkit object. 
	\param key The key that you want to extract information from.
	\param variant Optional information specific to the object specified by the key.
	*/
 	TK_Status	Interpret(BStreamFileToolkit &tk, ID_Key key, int variant);

	/*! This method iterates through the list of registers handlers invokes their Execute method. 
	\param tk A reference to the BStreamFileToolkit object. 
	*/
	TK_Status   Execute (BStreamFileToolkit & tk);

	/*! This method iterates through the list of registers handlers invokes their Read method. 
	\param tk A reference to the BStreamFileToolkit object. 
	*/
	TK_Status   Read (BStreamFileToolkit & tk);

	/*! This method lets you add a handler so it will be on the list that will be iterated when 
	#Interpret is invoked.
	\param handler A pointer to the handler that you want to register.
	*/
	void AddHandler(TK_XML *handler);

	/*! This method stops the dispatcher from iterating through the handlers list.*/
	void StopIteration() { m_bStopIteration = true; }

	/*! This method makes an exact copy of itself.
	\return A pointer to the newly created copy of this object.
	*/
	TK_Dispatch_XML *MakeCopy();

protected:	
	struct vlist_s*			m_XMLHandlerList;					/*!< A pointer to the list of XML handlers. */

private:
	HBaseModel *	m_pModel;
 	bool m_bStopIteration;
	bool m_bCloned;

	
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif






