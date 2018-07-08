// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIM_ERROR_HANDLER_H
#define _HIM_ERROR_HANDLER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

/* forward declaration.  
 * private helper class */
class HErrorDescriptorNode;


//! The HImErrorHandler class implements a set of rules for when info/warnings/errors should be passed to the user.
/*!
	The HImErrorHandler allows for a more sophisticated scheme of prompting the user when necessary.  For most
	warnings and errors, HOOPS does not retain the information that the user has already seen that message 
	or one like it.  The result is that the error messages can be duplicated to the point of making an application
	unusable.  The alternative is to turn info/warnings/errors off (see #HC_Define_System_Options), but that is
	too broad a brush as it can cause potentially serious problems to be missed.

	In addition to the information string that the user sees, HOOPS error messages are identified by 3 integers:
	category, specific, and severity.  The default behavior is for the HImErrorHandler to pass each combination of
	3 integers exactly once during the execution of the application.  Memory of what errors have already been 
	given is kept in a linked list so that duplicates can be rejected.  Additionally, Pass() can be used to 
	specify that a certain error message always be passed; and Block() can be used to block even the first instance
	of a message.  Both Pass() and Block() take the 3 identifying integers that describe an error, '-1' is a special
	value interpreted as a wild card that matches anything.  For example, Pass( HEC_CALLBACK, -1, -1 ) would 
	always pass any callback related bugs.  Info/warnings/errors matched by both Block() and Pass() will be passed.

	The most common usage is expected to be to simply register it with default settings just to avoid duplicates.
*/ 
class MVO_API HImErrorHandler 
{
private:
    static bool IsBlocked( int category, int specific, int severity );
    static bool IsPassed( int category, int specific, int severity );

    static HErrorDescriptorNode *m_Block;
    static HErrorDescriptorNode *m_Pass;
    static bool m_BlockByDefault;

public:
	/*! Constructor an HImErrorHandler object. 
	\param BlockByDefault Pass true if you want to block all error messages by default if they haven't been identified as
	blocked or passed.*/
    HImErrorHandler( bool BlockByDefault = false );
    ~HImErrorHandler();
	/*! This method gives HOOPS pointers to the functions want to intercept.  
		This function should be called just once, at the beginning of execution. */
	static void RegisterCallbacks();
    
    /*! This method specifies the errors to always block.  This includes errors that we may not have 
	even seen before.
	\param cat The category number.
	\param spec The specific number.
	\param sev The severity number.
	*/
    static void Block( int cat, int spec, int sev );

	/*! This method specifies the errors to pass on.  This includes errors that we may not have seen before.
	\param cat The category number.
	\param spec The specific number.
	\param sev The severity number.
	*/
    static void Pass( int cat, int spec, int sev );

    /*! This method sets the behavior of the all errors not otherwise specified.
	\param bbd Pass true to block by default or false to pass by default.
	*/
    void SetBlockByDefault( bool bbd ) { m_BlockByDefault = bbd; }

    /*! \return True if error messages are block by default unless specified otherwise or false if they are passed.*/
	static bool BlockByDefault() { return m_BlockByDefault; }

    /*! This is the callback that is registered with HOOPS and handles the passing and blocking of errors.  It must 
	be static to be registered as a callback.
	*/
    static void CallbackEntryPoint( 
		int category, int specific, int severity,
		int msgc, char *msgv[], int stackc, char *stackv[]);
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



