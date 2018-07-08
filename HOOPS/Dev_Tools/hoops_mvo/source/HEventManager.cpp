// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HEventManager.h"
#include "HEventListener.h"
#include <math.h>
#include "string.h"
#include "stdio.h"

#include "vhash.h"
#include "vlist.h"
#include "HEventInfo.h"



HEventListenerManager::HEventListenerManager()
{
	m_pEventListenerList = new_vlist(malloc, free);
	m_bQueryInProgress = false;
}

HEventListenerManager::~HEventListenerManager()
{
	START_LIST_ITERATION(HEventListenerItem, m_pEventListenerList);{
		delete temp;
	}END_LIST_ITERATION(m_pEventListenerList);
	delete_vlist(m_pEventListenerList);
}


void HEventListenerManager::Reset()
{
	vlist_reset_cursor(m_pEventListenerList);
	m_bQueryInProgress = false;
}
void HEventListenerManager::StartQuery()
{
//	assert(m_bQueryInProgress == false);
	Reset();
	m_bQueryInProgress = true;
}

HEventListener * HEventListenerManager::GetEventListener()
{
	if (!m_bQueryInProgress)
		return 0;
	HEventListenerItem * temp = (HEventListenerItem *)vlist_peek_cursor(m_pEventListenerList);
 	if (!temp)
	{
		Reset();
		return 0;
	}
	else
	{
		vlist_advance_cursor(m_pEventListenerList);
		return temp->m_pEventListener;
	}
}


bool HEventListenerManager::RemoveEventListener(HEventListener *el)
{
 	START_LIST_ITERATION(HEventListenerItem, m_pEventListenerList);
	if (temp->m_pEventListener == el)
	{
		vlist_remove(m_pEventListenerList, temp);
		delete temp;
 		return true;
	}
	END_LIST_ITERATION(m_pEventListenerList);
	return false;
}

void HEventListenerManager::AddEventListener(HEventListener *el, int priority)
{

	HEventListenerItem *listeneritem = 0;
	START_LIST_ITERATION(HEventListenerItem, m_pEventListenerList);
	if (temp->m_pEventListener == el)
	{
		vlist_remove(m_pEventListenerList, temp);
		listeneritem = temp;
		break;
	}
	END_LIST_ITERATION(m_pEventListenerList);

	if (!listeneritem)
	{
		listeneritem = new HEventListenerItem();
	}
	listeneritem->m_pEventListener = el;
	listeneritem->m_Priority = priority;

	bool inserted = false;
 	
	HEventListenerItem * item = (HEventListenerItem *)vlist_peek_first(m_pEventListenerList);	
	if (item && priority >= item->m_Priority)
	{
	    inserted = true;
	    vlist_add_first(m_pEventListenerList, listeneritem);
	}
	else{	
	
	START_LIST_ITERATION(HEventListenerItem, m_pEventListenerList);
		HEventListenerItem * nextitem = (HEventListenerItem *)vlist_peek_cursor_next(m_pEventListenerList);	
		if (nextitem && priority>=nextitem->m_Priority)
		{
			vlist_add_after_cursor(m_pEventListenerList, listeneritem);
			inserted = true;
			break;
		}
 	END_LIST_ITERATION(m_pEventListenerList);
	}
	if (!inserted)
		vlist_add_last(m_pEventListenerList, listeneritem);
}


static void delete_eventlistenerlisthashstring(
	void * tagcallback_struct,
	const char * tag_name,
	void * user_data)
{
	struct vhash_s * elhash = (struct vhash_s *) user_data;
	HEventListenerManager * pstruct = (HEventListenerManager *) tagcallback_struct;

	delete(pstruct);
	vhash_remove_string_key(elhash, tag_name);
}
static void delete_eventlistenerlisthash(
	void * tagcallback_struct,
	void * tag_name,
	void * user_data)
{
	struct vhash_s * elhash = (struct vhash_s *) user_data;
	HEventListener * pstruct = (HEventListener *) tagcallback_struct;

	delete(pstruct);
	vhash_remove(elhash, tag_name);
}




HEventManager::HEventManager()
{
	m_pEventListenerListHash = new_vhash(1, malloc, free);
	m_pEventListenerListHashString = new_vhash(1, malloc, free);
}


HEventManager::~HEventManager()
{
 	vhash_string_key_map_function(m_pEventListenerListHashString, delete_eventlistenerlisthashstring,(void*)m_pEventListenerListHashString);
	delete_vhash(m_pEventListenerListHashString);
 	vhash_map_function(m_pEventListenerListHash, delete_eventlistenerlisthash,(void*)m_pEventListenerListHash);
	delete_vhash(m_pEventListenerListHash);
}


void HEventManager::RegisterHandler(HEventListener *listener, const char * et, int priority)
{
	if (listener->GetRequestedPriority() !=-1)
		priority = listener->GetRequestedPriority();
	HEventListener * eventlistenerlist = 0;
  	if (!(vhash_lookup_string_key_item(m_pEventListenerListHashString, et , (void **)&eventlistenerlist) == VHASH_STATUS_SUCCESS))
	{
		eventlistenerlist = listener->CreateListenerManager();	 
		vhash_replace_string_key_item(m_pEventListenerListHashString, et, (void *)eventlistenerlist, 0);
	}
	eventlistenerlist->GetEventListenerManager()->AddEventListener(listener, priority);
}

void HEventManager::RegisterHandler(HEventListener *listener, HEventListenerType et, int priority)
{
	if (listener->GetRequestedPriority() !=-1)
		priority = listener->GetRequestedPriority();

	HEventListener * eventlistenerlist = 0;
  	if (!(vhash_lookup_item(m_pEventListenerListHash, (void *)et , (void **)&eventlistenerlist) == VHASH_STATUS_SUCCESS))
	{
		eventlistenerlist = listener->CreateListenerManager();
 		vhash_replace_item(m_pEventListenerListHash, (void *)et, (void *)eventlistenerlist, 0);
	}
	eventlistenerlist->GetEventListenerManager()->AddEventListener(listener, priority);
}


HEventListener * HEventManager::GetEventListenerManager(HEventListenerType eventtype)
{
	HEventListener * eventlistenerlist = 0;
  	if (!(vhash_lookup_item(m_pEventListenerListHash, (void *)eventtype , (void **)&eventlistenerlist) == VHASH_STATUS_SUCCESS))
		return 0;
 	return eventlistenerlist;
}

HEventListener * HEventManager::GetEventListenerManager(const char * eventtype)
{
	HEventListener * eventlistenerlist = 0;
  	if (!(vhash_lookup_string_key_item(m_pEventListenerListHashString, eventtype , (void **)&eventlistenerlist) == VHASH_STATUS_SUCCESS))
		return 0;
 	return eventlistenerlist;
}
 

bool HEventManager::UnRegisterHandler(HEventListener * listener, const char * et)
{

	HEventListener *eventlistenerlist = 0;
  	if (!(vhash_lookup_string_key_item(m_pEventListenerListHashString, et , (void **)&eventlistenerlist) == VHASH_STATUS_SUCCESS))
		return false;
	return eventlistenerlist->GetEventListenerManager()->RemoveEventListener(listener);
}

bool HEventManager::UnRegisterHandler(HEventListener *listener, HEventListenerType et)
{

	HEventListener *eventlistenerlist = 0;
  	if (!(vhash_lookup_item(m_pEventListenerListHash, (void *)et , (void **)&eventlistenerlist) == VHASH_STATUS_SUCCESS))
		return false;
	return eventlistenerlist->GetEventListenerManager()->RemoveEventListener(listener);
}

void HEventManager::UnRegisterHandler(HEventListener *listener)
{
	UNREFERENCED(listener);
}



static void delete_objecthash(
	void * tagcallback_struct,
	const char * tag_name,
	void * user_data)
{
	struct vhash_s * elhash = (struct vhash_s *) user_data;
	HObject * pstruct = (HObject *) tagcallback_struct;

	delete(pstruct);
	vhash_remove_string_key(elhash, tag_name);
}


long HObjectManager::object_segment_count=0;

HObjectManager::HObjectManager()
{
	m_pObjectHash = new_vhash(1, malloc, free);
}

	
HObjectManager::~HObjectManager()
{
 	vhash_string_key_map_function(m_pObjectHash, delete_objecthash,(void*)m_pObjectHash);
	delete_vhash(m_pObjectHash);
}

HObject * HObjectManager::AddHObject(HObject *object)
{

	HObject  *retobject = 0;
 	vhash_replace_string_key_item(m_pObjectHash, object->GetName(), (void *)object, (void **)&retobject);

    return retobject;
}

HObject * HObjectManager::GetHObject(const char *name)
{

	HObject  *retobject = 0;
 	vhash_lookup_string_key_item(m_pObjectHash, name, (void **)&retobject);
	return retobject;
}

HObject * HObjectManager::RemoveHObject(const char * et)
{
	HObject  *retobject = 0;
 	vhash_remove_string_key_item(m_pObjectHash, et, (void **)&retobject);
	return retobject;
}

HC_KEY HObjectManager::FindHObjectSegment(HC_KEY segkey, char *res, char *infores)
{

	if (res)
		*res = '\0';

	if (infores)
		*infores = '\0';

    if (segkey == INVALID_KEY)
		return INVALID_KEY;

    HC_KEY oldreskey = segkey;
	
	if (object_segment_count == 0)
		return oldreskey;

    HC_KEY reskey = HUtility::GrabSegment(segkey);

    HUserOptions temp;
	char otype[MVO_BUFFER_SIZE];
	strcpy(otype,"");

    while (1)
    {
		HC_Open_Segment_By_Key(reskey);

			temp.Show_One_Net_User_Option("hobject");

			if (!temp.IsValid() || streq(temp.m_data,"skip"))	
			{
				HC_Close_Segment();
				if (temp.IsValid() && streq(temp.m_data,"skip") || 
					!temp.IsValid() && streq(otype,"skip"))
					return INVALID_KEY;
				else
					return oldreskey;
			}
	
			if (HC_Show_Existence("user options = hobject"))
			{	
				temp.Show_One_User_Option("hobject");
				HUtility::ShowOneUserOption("hobject_type", otype);
				if (strcmp(otype, "skip") != 0)
					{							
						if (res)
							strcpy(res, temp.m_data);
						if (infores)
							strcpy(infores, otype);
						
 						HC_Close_Segment();
						return reskey;
					}
			}

		HC_Close_Segment();


		oldreskey = reskey;
		reskey = HC_KShow_Owner_By_Key(reskey);
    }

    return INVALID_KEY;
}

void HObjectManager::FileLoadHObjectCheck(HC_KEY segkey)
{
	/*defined static to save stack space*/
	static char type[128];
	static char full_path_name[MVO_BUFFER_SIZE];
	static HC_KEY key;
	static char	key_type[32];
	
	if (!HC_Show_Existence_By_Key(segkey, "self"))
		return;
	
	HC_Open_Segment_By_Key(segkey);
		if (HC_Show_Existence("user options = hobject"))
		{	
			object_segment_count++;
			HC_Close_Segment();
			return;
		}
		
		HC_Begin_Contents_Search(".", "segments, includes");
			while(HC_Find_Contents(type, &key))
			{
				if(streq(type, "include"))
				{
					HC_Show_Include_Segment(key, full_path_name);
					key=HC_KCreate_Segment(full_path_name);
					if (key >= 0) {		// cant trust, may be a local key, which won't work in the current 'segkey' context
						HC_KEY	save = key;
						HC_Open_Segment (full_path_name);
							HC_Open_Segment ("..");
								HC_Show_Key_Status (key, key_type);
								key = HC_Renumber_Key (key, -1, "");	// get original
								HC_Renumber_Key (key, save, key_type);	// and restore
							HC_Close_Segment();
						HC_Close_Segment();
					}
				}
				
				FileLoadHObjectCheck(key);				
			}
		HC_End_Contents_Search();
		
	HC_Close_Segment();
}

void HObjectManager::SetHObjectSegment(const char *objecttype)
{
    char text[MVO_BUFFER_SIZE];
    sprintf(text, "hobject = %s", objecttype);
    HC_Set_User_Options(text);
	object_segment_count++;
}

void HObjectManager::SetHObjectInfo(const char *objecttype)
{
    char text[MVO_BUFFER_SIZE];
    if (!HUtility::ShowOneUserOption("hobject",0))
		SetHObjectSegment("generic");
    
    sprintf(text, "hobject_type = %s", objecttype);
    HC_Set_User_Options(text);
}
