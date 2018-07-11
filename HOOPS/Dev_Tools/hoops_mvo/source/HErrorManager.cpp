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
#include "vlist.h"
#include "HErrorManager.h"

 
struct vlist_s *	HErrorManager::m_ErrorList = 0;
struct vlist_s *	HErrorManager::m_UpdateErrorList = 0;
struct vlist_s *	HErrorManager::m_SuppressList = 0;
void			(* HErrorManager::m_pfError)(HErrorNode *, void *) = 0;
void			*HErrorManager::m_UserData = 0;
bool				*HErrorManager::m_ErrorClass = 0;
bool				HErrorManager::m_bShowDialog = true;
bool				HErrorManager::m_bDialogRunning = false;
bool				HErrorManager::m_bBlocked = false;
bool				HErrorManager::m_bInterrupted = false;
bool				HErrorManager::m_bSpoolUpdateErrors = true;

void HErrorManager::CallbackEntryPoint (
	int category,
	int specific,
	int severity,
	int msgc,
	char const ** msgv,
	int stackc,
	char const ** stackv) 
{
	

	m_bDialogRunning = true;
	if (!m_bBlocked)
	{	
		double ct;
		HIC_Show_Time(&ct);
		HErrorNode *node = new HErrorNode(category, specific, severity, msgc, msgv, stackc, stackv,ct);
/* with full threading it's never safe to report the error directly */
// 		if (strstr(node->m_stack[0],"Update_Display"))
			vlist_add_last(m_UpdateErrorList, node);
// 		else
// 		{
// 			vlist_add_first(m_ErrorList, node);
// 			if (m_pfError)
// 				m_pfError(node, m_UserData);
// 		}
		
	}
	m_bDialogRunning = false;
} 

void HErrorManager::SpoolUpdateErrors()
{
	if (m_UpdateErrorList)
	{	
		if (m_bDialogRunning == false)
		{	
			m_bDialogRunning = true;
			if (m_bBlocked == false)
			{	
				
				m_bBlocked = true;
				HErrorNode * temp;
				while ((temp = (HErrorNode *)vlist_remove_first(m_UpdateErrorList)) != 0) {

					HErrorNode *node = new HErrorNode(temp->m_category, temp->m_specific, temp->m_severity, temp->m_messagelength,
					temp->m_message, temp->m_stacklength, temp->m_stack,0);
					bool suppress = false;
					HErrorNode *cerror = temp;
					START_LIST_ITERATION(HSuppressInfo, m_SuppressList);{
						if ((temp->category == cerror->m_category && temp->specific == cerror->m_specific && temp->severity == cerror->m_severity) || (m_ErrorClass[cerror->m_severity]))
						{
							suppress = true;
							break;
						}
					}END_LIST_ITERATION(m_SuppressList);

					if (!suppress)
					{		
						vlist_add_first(m_ErrorList, node);

						if (m_pfError)
							m_pfError(node, m_UserData);
					}
					else
						delete node;

					delete temp;

					if (m_bInterrupted) {
						while ((temp = (HErrorNode *)vlist_remove_first(m_UpdateErrorList)) != 0) {
							delete temp;
						}
						break;
					}
				}

				m_bBlocked = false;
			}	
			m_bDialogRunning = false;
		}
	}
}

void HErrorManager::Init()
{
 
	m_ErrorList = new_vlist(malloc,free);
	m_UpdateErrorList = new_vlist(malloc,free);
	m_SuppressList = new_vlist(malloc,free);
	HC_UnDefine_Error_Handler ( (void (HC_CDECL*)(HC_ANY_ARGS)) HC_Report_Error );
	HC_Define_Error_Handler ( (void (HC_CDECL*)(HC_ANY_ARGS)) HErrorManager::CallbackEntryPoint );
	m_ErrorClass = new bool[4];
	for (int i=0;i<4;i++)
		m_ErrorClass[i] = false;

}
void HErrorManager::Cleanup()
{

	if (!m_bBlocked) {

		m_bInterrupted = false;

		HC_UnDefine_Error_Handler ( (void (HC_CDECL*)(HC_ANY_ARGS)) HErrorManager::CallbackEntryPoint );
		HC_Define_Error_Handler ( (void (HC_CDECL*)(HC_ANY_ARGS)) abort );

		if (m_ErrorList)
		{
			START_LIST_ITERATION(HErrorNode, m_ErrorList);{
				delete temp;
			}END_LIST_ITERATION(m_ErrorList);
			delete_vlist(m_ErrorList);
			m_ErrorList = 0;
		}

		if (m_UpdateErrorList)
		{
			START_LIST_ITERATION(HErrorNode, m_UpdateErrorList);{
				delete temp;
			}END_LIST_ITERATION(m_UpdateErrorList);
			delete_vlist(m_UpdateErrorList);
			m_UpdateErrorList = 0;
		}

		if (m_SuppressList)
		{

			START_LIST_ITERATION(HSuppressInfo, m_SuppressList);{
				delete temp;
			}END_LIST_ITERATION(m_SuppressList);
			delete_vlist(m_SuppressList);
			m_SuppressList = 0;	
		}
	
		delete [] m_ErrorClass;
		m_ErrorClass = 0;
	}
	else 
		m_bInterrupted = true;
}


void HErrorManager::BeginErrorIteration()
{
	vlist_reset_cursor(m_ErrorList);
}

HErrorNode * HErrorManager::GetNextError()
{
	HErrorNode *temp = (HErrorNode *)vlist_peek_cursor(m_ErrorList);
	vlist_advance_cursor(m_ErrorList);
	return temp;
}

HErrorNode * HErrorManager::GetLastError()
{
	HErrorNode *temp = (HErrorNode *)vlist_peek_first (m_ErrorList);
	return temp;
}
 
void HErrorManager::DeleteLastError()
{
	HErrorNode *temp = 0;
	/*
 	vlist_reset_cursor(m_ErrorList);
	while ((temp = (HErrorNode *)vlist_peek_cursor(m_ErrorList)) != vlist_peek_last(m_ErrorList) && temp)
		vlist_advance_cursor(m_ErrorList);
	vlist_remove_at_cursor(m_ErrorList);
	if (temp)
		delete temp;
		*/
	temp = (HErrorNode *)vlist_remove_first (m_ErrorList);
	if (temp)
		delete temp;


}

void HErrorManager::ShowAllErrors()
{
	HErrorNode *node;
	BeginErrorIteration();
	while ((node = GetNextError()) != 0)
	{
		node->Report();
	}
}

 
void HErrorManager::AllowAllErrors()
{
	HC_Define_System_Options("warnings = on, errors = on, fatal errors = on, info = on");
	for (int i=0;i<4;i++)
		m_ErrorClass[i] = false;
	START_LIST_ITERATION(HSuppressInfo, m_SuppressList);{
		delete temp;
	}END_LIST_ITERATION(m_SuppressList);
	vlist_flush(m_SuppressList);

}
void HErrorManager::SuppressError(int severity, int category, int specific)
{
	if (!category && !specific)
		m_ErrorClass[severity] = true;
	HSuppressInfo *si = new HSuppressInfo(severity, category, specific);	
	vlist_add_last(m_SuppressList, si);

	char ** errors = new char *[4];
	for (int i = 0; i < 4; i++)
		errors[i] = new char[MVO_BUFFER_SIZE];

	char res[MVO_BUFFER_SIZE];
	HC_Show_One_System_Option("fatal errors", res);
	if (strcmp(res,"off") == 0)
		m_ErrorClass[3] = true;
	HC_Show_One_System_Option("errors", res);
	if (strcmp(res,"off") == 0)
		m_ErrorClass[2] = true;
	HC_Show_One_System_Option("warnings", res);
	if (strcmp(res,"off") == 0)
		m_ErrorClass[1] = true;
	HC_Show_One_System_Option("info", res);
	if (strcmp(res,"off") == 0)
		m_ErrorClass[0] = true;

	if (!m_ErrorClass[3])
		strcpy(errors[3], "fatal errors = ");
	else
		strcpy(errors[3], "fatal errors = off");

	if (!m_ErrorClass[2])
		strcpy(errors[2], "errors = ");
	else
		strcpy(errors[2], "errors = off");

	if (!m_ErrorClass[1])
		strcpy(errors[1], "warnings = ");
	else
		strcpy(errors[1], "warnings = off");

	if (!m_ErrorClass[0])
		strcpy(errors[0], "info = ");
	else
		strcpy(errors[0], "info = off");
 
	char text[MVO_BUFFER_SIZE];
	START_LIST_ITERATION(HSuppressInfo, m_SuppressList);{
		if (!m_ErrorClass[temp->severity])
		{
			if (!strstr(errors[temp->severity], "disable"))
 				strcat(errors[temp->severity], "(on, disable = (");
 
 			sprintf(text, "%s%d/%d,",errors[temp->severity], temp->category, temp->specific);
			strcpy(errors[temp->severity],text);
		}
	}END_LIST_ITERATION(m_SuppressList);

	for (int i = 0; i < 4; i++)
	{
		if (!m_ErrorClass[i])
		{
			if (!strstr(errors[i], "disable"))
				strcat(errors[i], "on");
			else
			{			
				errors[i][strlen(errors[i])-1]= ')';
				strcat(errors[i], ")");
			}
		}
		HC_Define_System_Options(errors[i]);
		delete [] errors[i];
	}

	delete [] errors;

 
}

HErrorNode::HErrorNode(int category, int specific, int severity, int msgc, char const ** msgv, int stackc, char const ** stackv, double t)
{
	m_category = category;
	m_specific = specific;
	m_severity = severity;
	m_messagelength = msgc;
	m_time = t;
	m_message = new char const * [m_messagelength];
	for (int i = 0; i < m_messagelength; i++)
	{
		char * tmp = new char[strlen(msgv[i])+1];	
		strcpy(tmp, msgv[i]);
		m_message[i] = tmp;
	}	
	
	m_stacklength = stackc;
	m_stack = new char const * [m_stacklength];

	for (int i = 0; i < m_stacklength; i++)
	{
		char * tmp = new char[strlen(stackv[i])+1];	
		strcpy(tmp, stackv[i]);
		m_stack[i] = tmp;
	}
}


HErrorNode::~HErrorNode()
{
	int i;
	for (i=0;i<m_messagelength;i++)
		delete [] m_message[i];
	delete [] m_message;
	for (i=0;i<m_stacklength;i++)
		delete [] m_stack[i];
	delete [] m_stack;
}

void HErrorNode::Report()
{
	HC_Report_Error(m_category, m_specific, m_severity, m_messagelength, m_message, m_stacklength, m_stack);

}


void HErrorManager::Flush()
{
	START_LIST_ITERATION(HErrorNode, m_ErrorList);{
		delete temp;
	}END_LIST_ITERATION(m_ErrorList);
	vlist_flush(m_ErrorList);
 
	START_LIST_ITERATION(HErrorNode, m_UpdateErrorList);{
		delete temp;
	}END_LIST_ITERATION(m_UpdateErrorList);
	vlist_flush(m_UpdateErrorList);
	
}
