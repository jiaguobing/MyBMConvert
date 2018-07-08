// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "HTools.h"
#include "hc.h" 
#include "hic.h"
#include "hpserror.h"
#include "HImErrorHandler.h"

HErrorDescriptorNode *HImErrorHandler::m_Block = 0;
HErrorDescriptorNode *HImErrorHandler::m_Pass = 0;
bool HImErrorHandler::m_BlockByDefault = true;

/* this helper class forms a node in a linked list.  It provides the
 * programmatic description of a single error message. */
class HErrorDescriptorNode {
private:
    int category, specific, severity;
public:
    HErrorDescriptorNode( int cat, int spec, int sev ) {
		category = cat; 
		specific = spec;
		severity = sev;
		next = prev = 0;
    }
    ~HErrorDescriptorNode() { delete next; next = 0; }

    bool Match( int cat, int spec, int sev ) {
		if( category >= 0 && category != cat ) return false;
		if( specific >= 0 && specific != spec ) return false;
		if( severity >= 0 && severity != sev ) return false;
		return true;
    }
    //publicly accessible next and prev pointers so we can have a
    //quick and dirty doubly-linked list (0-terminated on both ends)
    HErrorDescriptorNode *next, *prev;
};


HImErrorHandler::HImErrorHandler( bool BlockByDefault ) { 
	m_Block = m_Pass = 0; 
	m_BlockByDefault = BlockByDefault; 
}


HImErrorHandler::~HImErrorHandler() 
{
	H_SAFE_DELETE(m_Block);
	H_SAFE_DELETE(m_Pass);
}


void HImErrorHandler::RegisterCallbacks()
{
	//uncomment the following lines to enable this class.
	//HC_UnDefine_Error_Handler ( (void (HC_CDECL*)(HC_ANY_ARGS)) HC_Report_Error );
	//HC_Define_Error_Handler ( (void (HC_CDECL*)(HC_ANY_ARGS)) HImErrorHandler::CallbackEntryPoint );
}


void HImErrorHandler::Block( int cat, int spec, int sev ) { 
	HErrorDescriptorNode *newnode = new HErrorDescriptorNode( cat, spec, sev );
	newnode->next = m_Block;
	if( m_Block != 0 )
		m_Block->prev = newnode;
	m_Block = newnode;
}


void HImErrorHandler::Pass( int cat, int spec, int sev ) { 
	HErrorDescriptorNode *newnode = new HErrorDescriptorNode( cat, spec, sev );
	newnode->next = m_Pass;
	if( m_Pass != 0 )
		m_Pass->prev = newnode;
	m_Pass = newnode;
}


/* private */
bool HImErrorHandler::IsBlocked( int category, int specific, int severity )
{
    HErrorDescriptorNode *ptr;
    for( ptr = m_Block ; ptr != 0 ; ptr = ptr->next ) {
		if( ptr->Match( category, specific, severity ) ) {
			if( m_Block != ptr ) {
				//move to the front so list is sorted by most-recently-matched
				if( ptr->prev != 0 )
					ptr->prev->next = ptr->next;
				if( ptr->next != 0 )
					ptr->next->prev = ptr->prev;
				m_Block->prev = ptr;
				ptr->next = m_Block;
				m_Block = ptr;
			}
			return true;
		}
    }
    return false;
} 


/* private */
bool HImErrorHandler::IsPassed( int category, int specific, int severity )
{
    HErrorDescriptorNode *ptr;
    for( ptr = m_Pass ; ptr != 0 ; ptr = ptr->next ) {
		if( ptr->Match( category, specific, severity ) ) {
			if( m_Pass != ptr ) {
				//move to the front so list is sorted by most-recently-matched
				if( ptr->prev != 0 )
					ptr->prev->next = ptr->next;
				if( ptr->next != 0 )
					ptr->next->prev = ptr->prev;
				m_Pass->prev = ptr;
				ptr->next = m_Pass;
				m_Pass = ptr;
			}
			return true;
		}
    }
    return false;
} 


void HImErrorHandler::CallbackEntryPoint (
	int category,
	int specific,
	int severity,
	int msgc,
	char **msgv,
	int stackc,
	char **stackv) 
{
    if( IsPassed( category, specific, severity ) ) {
		HC_Report_Error (category, specific, severity, msgc, msgv, stackc, stackv);
		return;
    } 
    else if( IsBlocked( category, specific, severity ) ) {
		return;
    }
    else if( !BlockByDefault() ) {
		HC_Report_Error (category, specific, severity, msgc, msgv, stackc, stackv);
		//things that aren't explicitly passed are blocked after the first warning
		Block(category, specific, severity);
    }
} 



