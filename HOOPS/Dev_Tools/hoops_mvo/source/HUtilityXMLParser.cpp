// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HUtilityXMLParser.h"
#include "HUtilityTextParser.h"
#include "vhash.h"
#include "vlist.h"


#define STATUS_DELIMITER_NOT_FOUND 0
#define STATUS_TAG_FINISHED 1
#define STATUS_TAG_NOT_FINISHED 2
#define STATUS_SPECIAL_TAG_SKIPPED 3
#define STATUS_SPECIAL_TAG_NOT_SKIPPED 4
#define STATUS_NO_SUBTAGS 5
#define STATUS_SUBTAGS_FOUND 6
#define UINT2bool(__uint__val)  (__uint__val > 0) ?  true: false


static delimiter_t find_tagname_td[] = {{"\x21-\x7E|", true, "/|>| |", true, false}};
static delimiter_t skip_tag_td[] = {{"<", true, ">", true, true}};
static delimiter_t find_properites_td[] = {{">", true, 0, true, true},
											{"/", true, 0, true, true},
											{"\x21-\x7E|", false, " |=", true, false}};
static	delimiter_t find_properites_value_td[] = {{"\"", true, "\"", true, false},
													{"\x21-\x7E|", false," |>|/|", true, false}};
	
static	delimiter_t find_equal_td[] = {{"=", true, 0, true, true},
										{"\x21-\x7E|", true, 0, true, true}};
static	delimiter_t check_for_start_tag_td[] = {{"\x21-\x7E|", false, 0, true, false}};	
static	delimiter_t find_item_value_td[] = { {"<", false,0, true, false},
												{"\"", false, "\"", false, false},
												{"[", false,"]", false, true},
												{"\"", false,"\"", true, false},
												{"\x21-\x7E|", false," |<|\n|\x09|", true, false}};
static delimiter_t find_doctype_td[] = {{"\"", true, "\"", true, false},
										{"\x21-\x7E|", false, " |>", true, false},
										{"[", false, 0, true, false}};
static delimiter_t skip_bracket_td[] = {{"[", true, "]", true, true}};

 
static	int dlum;
static	char result[MVO_BUFFER_SIZE];	
 
static void delete_property_struct(
	void * property_struct,
	const char * property_name,
	void * user_data)
{
	struct vhash_s * m_pPropertyHash = (struct vhash_s *) user_data;
	HUtilityXMLProperty * pstruct = (HUtilityXMLProperty *) property_struct;

	delete(pstruct);
	vhash_remove_string_key(m_pPropertyHash, property_name);
}



static void delete_tagcallback_struct(
	void * tagcallback_struct,
	const char * tag_name,
	void * user_data)
{
	struct vhash_s * tagcallback_hash = (struct vhash_s *) user_data;
	HUtilityXMLTagCallback * pstruct = (HUtilityXMLTagCallback *) tagcallback_struct;

	delete(pstruct);
	vhash_remove_string_key(tagcallback_hash, tag_name);
}


HUtilityXMLTag::HUtilityXMLTag(HUtilityXMLParser *xmlparser)
{
	m_pPropertyHash = 0;
	m_pItemList = 0;
	m_pTagName = 0;
	m_pPropertyList = 0;
	m_pCommentList = 0;
	m_bOpen = false;
	m_pXMLParser = xmlparser;
	Init();
}

HUtilityXMLTag::~HUtilityXMLTag()
{
	FreeMemory();
}

void HUtilityXMLTag::Init()
{
	FreeMemory();
	m_pPropertyHash = new_vhash(1, malloc, free);
	m_pItemList = new_vlist(malloc, free);
	m_pCommentList = new_vlist(malloc, free);
}

void HUtilityXMLTag::AddComment(char *comment)
{
    char * text = new char[strlen(comment)+1];
    strcpy(text, comment);
    vlist_add_last(m_pCommentList, text);
}
void HUtilityXMLTag::CleanupPropertyList()
{
	if (m_pPropertyList)
	{
		vhash_string_key_pair_t *temp;

		while ((temp = (vhash_string_key_pair_t *)vlist_remove_first(m_pPropertyList)) != 0) {
			//this one was allocated by malloc via function pointer passed to 
                        //vhash_string_keys_to_vlist, so it needs to be matched with free, not delete
                        free( temp ); 
		}
		delete_vlist(m_pPropertyList);
		m_pPropertyList = 0;
	}
}


void HUtilityXMLTag::FreeMemory()
{
	if (m_pPropertyHash)
	{
		if (vhash_count(m_pPropertyHash) > 0)
			vhash_string_key_map_function(m_pPropertyHash, delete_property_struct,(void*)m_pPropertyHash);
		delete_vhash(m_pPropertyHash);
		m_pPropertyHash = 0;
	}
	
	CleanupPropertyList();

	if (m_pItemList)
	{
		char *temp;

		while ((temp = (char *)vlist_remove_first(m_pItemList)) != 0) {
			delete [] temp;
		}
		delete_vlist(m_pItemList);
		m_pItemList = 0;
	}
	if (m_pTagName)
	{
		delete [] m_pTagName;
		m_pTagName = 0;
	}
	if (m_pCommentList)
	{
	    START_LIST_ITERATION(char, m_pCommentList);
		delete [] temp;
	    END_LIST_ITERATION(m_pCommentList);
	    delete_vlist(m_pCommentList);
	    m_pCommentList = 0;
	}

}
 
void HUtilityXMLTag::SetTagname(const char *tagname)
{

	if (m_pTagName)
		delete [] m_pTagName;
	m_pTagName = new char[strlen(tagname) + 1];
	strcpy(m_pTagName, tagname);
}


bool HUtilityXMLTag::AddProperty(const char *name, const char *text, bool hasQuotes)
{
	HUtilityXMLProperty *preturn = 0;
	HUtilityXMLProperty *ptype = new HUtilityXMLProperty;
	ptype->m_pText = new char[strlen(text) + 1];
	strcpy(ptype->m_pText, text);
	ptype->m_HasQuotes = hasQuotes;

	if (vhash_replace_string_key_item(m_pPropertyHash, name, (void *)ptype, (void **)&preturn) == VHASH_STATUS_SUCCESS)
	{
		if (preturn)
		{
			delete preturn;
			return false;
		}
	}
	return true;

}

 
bool HUtilityXMLTag::GetProperty(const char *name, HUtilityXMLProperty **prop)
{
	HUtilityXMLProperty *preturn = 0;
  	if (vhash_lookup_string_key_item(m_pPropertyHash, name , (void **)&preturn) == VHASH_STATUS_SUCCESS)
	{
		*prop = preturn;
		return true;
	}
	return false;

}

bool HUtilityXMLTag::GetProperty(const char *name, char *pcont)
{
	HUtilityXMLProperty *p;
	if (GetProperty(name, &p))
	{
		strcpy(pcont, p->m_pText);
		return true;
	}
	else
		return false; 
}

bool HUtilityXMLTag::GetProperty(const char *name, int &ii)
{
	HUtilityXMLProperty *p;
	if (GetProperty(name, &p))
	{
 		ii = atoi(p->m_pText);
		return true;
	}
	else
		return false;

 
}

bool HUtilityXMLTag::GetProperty(const char *name, bool &ii)
{
	HUtilityXMLProperty *p;
	if (GetProperty(name, &p))
	{
 		ii = UINT2bool(atoi(p->m_pText));
		return true;
	}
	else
		return false;

 
}




void HUtilityXMLTag::AddListItem(const char *text)
{
	char *itemtext = new char[strlen(text) + 1];
	strcpy(itemtext, text);
	vlist_add_last(m_pItemList, (void *)itemtext);
}

void HUtilityXMLTag::StartListItemsQuery()
{
	vlist_reset_cursor(m_pItemList);
}

void HUtilityXMLTag::CleanListItems()
{
	START_LIST_ITERATION(char, m_pItemList);{
		delete [] temp;
	}END_LIST_ITERATION(m_pItemList);
    vlist_flush(m_pItemList);

}
bool HUtilityXMLTag::GetNextListItem(char **text)
{

	void *temp;
  	if ((temp = vlist_peek_cursor(m_pItemList)) == 0)
				return false;
	*text = (char *)temp;
 	vlist_advance_cursor(m_pItemList);	
	return true;
}

bool HUtilityXMLTag::HasMoreItems()
{
   	if (!vlist_peek_cursor(m_pItemList))
				return false;
	else
		return true;
}


bool HUtilityXMLTag::HasListItems()
{
	vlist_reset_cursor(m_pItemList);
	if (!vlist_peek_cursor(m_pItemList))
		return false;
	else 
		return true;
}



void HUtilityXMLTag::StartPropertyQuery()
{
	CleanupPropertyList();
 
	m_pPropertyList = new_vlist(malloc, free);
 
 	vhash_string_keys_to_vlist(m_pPropertyHash, m_pPropertyList, malloc);
	vlist_reset_cursor(m_pPropertyList);
}


bool HUtilityXMLTag::GetNextPropertyItem(char **propname, HUtilityXMLProperty **prop)
{
	void *temp;
 	
  	if ((temp = vlist_peek_cursor(m_pPropertyList)) == 0)
			return false;
	vhash_string_key_pair_t *pair = (vhash_string_key_pair_t *)temp;

	*propname = (char *)pair->key;
	*prop = (HUtilityXMLProperty *)pair->item;
 	vlist_advance_cursor(m_pPropertyList);	
	return true;

}

HUtilityXMLTag* HUtilityXMLTag::Clone()
{
    HUtilityXMLTag *tag = new HUtilityXMLTag(m_pXMLParser);
    tag->SetTagname(GetTagname());
    StartPropertyQuery();
    while (1)
    {
	char *name;
	HUtilityXMLProperty *prop;
	if (!GetNextPropertyItem(&name, &prop))
	    break;
	tag->AddProperty(name, prop->m_pText, prop->m_HasQuotes);
    }

    StartListItemsQuery();

    while (1)
    {
	char *text;
 	if (!GetNextListItem(&text))
	    break;
	tag->AddListItem(text);
    }

	START_LIST_ITERATION(char, m_pCommentList);{
		tag->AddComment(temp);
	}END_LIST_ITERATION(m_pCommentList);
    
    return tag;
}
void HUtilityXMLParser::GetFloatPoint(const char *text, HPoint &point)
{
	HUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	GetFloatPoint(&tp, point);
}

void HUtilityXMLParser::GetFloatPoint(HUtilityTextParser *tp, HPoint &point)
{
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[MVO_BUFFER_SIZE];
	int dummy;
	tp->ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(point.x = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(point.y = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(point.z = (float)atof(result));
}

void HUtilityXMLParser::GetRBGA(const char *text, float &r, float &g, float &b, float &a)
{
	HUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[MVO_BUFFER_SIZE];
	int dummy;
	tp.ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(r = (float)atof(result));
	tp.ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(g = (float)atof(result));
	tp.ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(b = (float)atof(result));
	tp.ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(a = (float)atof(result));
}


HUtilityXMLParser::HUtilityXMLParser()
{
	m_pTextParser = new HUtilityTextParser();
	tagcallback_hash = new_vhash(1, malloc, free);
	stackdata_list = new_vlist(malloc, free);
	m_pInvalidTag = 0;
 
}

HUtilityXMLParser::~HUtilityXMLParser()
{
	if (tagcallback_hash)
	{
		vhash_string_key_map_function(tagcallback_hash, delete_tagcallback_struct,(void*)tagcallback_hash);
		delete_vhash(tagcallback_hash);
		tagcallback_hash = 0;
	}
	if (stackdata_list)
		delete_vlist(stackdata_list);

	delete m_pTextParser;
	
}

bool HUtilityXMLParser::SetTagCallback(const char *tagname, void *(*callbackfunction)(HUtilityXMLTag *, bool open, void *m_pExtraData), void *m_pExtraData)
{
	HUtilityXMLTagCallback *preturn = 0;
	HUtilityXMLTagCallback *ptype = new HUtilityXMLTagCallback;
	ptype->m_pTagCallback = callbackfunction;
	ptype->m_pExtraData = m_pExtraData;

	int result;
	if (!tagname)
		result = vhash_replace_string_key_item(tagcallback_hash, "@defaulttag", (void *)ptype, (void **)&preturn);
	else
		result = vhash_replace_string_key_item(tagcallback_hash, tagname, (void *)ptype, (void **)&preturn);

	if (result == VHASH_STATUS_SUCCESS)
	{
		if (preturn)
		{
			delete preturn;
			return false;
		}
	}
	return true;

}



void HUtilityXMLParser::SetupParseBuffer(const char *buffer)
{
	m_pTextParser->InsertBuffer((char *)buffer, (unsigned int)strlen(buffer));
}


int HUtilityXMLParser::AddProperties(HUtilityXMLTag *xmltag)
{
  	while (1)
	{
		m_pTextParser->ParseBuffer(result, dlum, find_properites_td, 3);
		if (dlum == 0)
			return STATUS_TAG_NOT_FINISHED;
		else if (dlum == 1)
		{
			m_pTextParser->SkipAhead(1);
			return STATUS_TAG_FINISHED;
		}
		else
		{
			char propname[MVO_BUFFER_SIZE];
			strcpy(propname, result);
			
			m_pTextParser->SkipAhead(-1);
			m_pTextParser->ParseBuffer(result, dlum, find_equal_td, 2);
			assert(dlum == 0);
			
			m_pTextParser->ParseBuffer(result, dlum, find_properites_value_td, 2, false);
			if (m_pTextParser->GetEndDelimiter() == '\"')
				xmltag->AddProperty(propname, result, true);
			else
				xmltag->AddProperty(propname, result, false);
			if (m_pTextParser->GetEndDelimiter() == '>')
				return STATUS_TAG_NOT_FINISHED;
			if (m_pTextParser->GetEndDelimiter() == '/')
				return STATUS_TAG_FINISHED;
			
		} 						
	}
																										
}

 
void HUtilityXMLParser::AddListItems(HUtilityXMLTag *xmltag)
{
	m_pTextParser->Rewind();
	while (1)
	{
 		m_pTextParser->ParseBuffer(result, dlum, find_item_value_td, 5, false);
		if (dlum == 0)
		{			
			m_pTextParser->SkipAhead(-1);							
			break;
		}
		else if (m_pTextParser->GetEndDelimiter() == '<')
		{
			xmltag->AddListItem(result);
			m_pTextParser->SkipAhead(-1);
			break;
		}
		else
			xmltag->AddListItem(result);		
	}
	DoCallback(xmltag, true);
}



int HUtilityXMLParser::SkipSpecialTags(HUtilityXMLTag *xmltag)
{

	if (result[0] == '?' || result[0] == '!')
	{
		m_pTextParser->Rewind();
		m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1);
		xmltag->AddComment(result);
		return STATUS_SPECIAL_TAG_SKIPPED;
	}
	return STATUS_SPECIAL_TAG_NOT_SKIPPED;
}

int HUtilityXMLParser::ProcessSubtags(HUtilityXMLTag *xmltag)
{	
	
	m_pTextParser->ParseBuffer(result, dlum, check_for_start_tag_td, 1, false);
	if (m_pTextParser->GetStartDelimiter() == '<' && m_pTextParser->PeekAt(0) != '/')
	{
		DoCallback(xmltag, true);
		m_pTextParser->Rewind();
 		ProcessXMLData();						
		return STATUS_SUBTAGS_FOUND;
	}
	else
		return STATUS_NO_SUBTAGS;
}


void HUtilityXMLParser::DoCallback(HUtilityXMLTag *xmltag, bool openclose)
{	
	
	
	
	HUtilityXMLTagCallback *preturn = 0;
	xmltag->SetOpen(openclose);
  	
	if (m_pInvalidTag == 0 || m_pInvalidTag == xmltag)
	{
	if (vhash_lookup_string_key_item(tagcallback_hash, xmltag->GetTagname() , (void **)&preturn) != VHASH_STATUS_SUCCESS)
	{
		if (vhash_lookup_string_key_item(tagcallback_hash, "@defaulttag" , (void **)&preturn) != VHASH_STATUS_SUCCESS)
		{
			if (openclose)
				m_pInvalidTag = xmltag;
			else
			{
				m_pInvalidTag = 0;
 				xmltag->Init();
			}
			return;
		}
	}
	void *stack_data = 0;

	if (preturn->m_pTagCallback)
	     stack_data = (preturn->m_pTagCallback)(xmltag, openclose, preturn->m_pExtraData);

	if (!openclose)
	{
		xmltag->Init();
		vlist_remove_first(stackdata_list);
	}
	else
		vlist_add_first(stackdata_list, (void *) stack_data);
	}
	else
		return;

}


bool HUtilityXMLParser::FindDocType(char *doctype, const char *buffer)
{
		
	if (buffer)
		SetupParseBuffer(buffer);
 	while (1)
	{		
 	if (m_pTextParser->ParseBuffer(result, dlum, find_tagname_td, 1))
	{

		if (m_pTextParser->GetStartDelimiter() != '<')
			return false;

		if (result[0] == '?')
		{
			m_pTextParser->Rewind();
			m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1);
		}
		else if (result[0] == '!')
		{
			if (strcmp(result, "!DOCTYPE") == 0)
			{
				m_pTextParser->ParseBuffer(result, dlum, find_doctype_td, 3, false);
				m_pTextParser->ParseBuffer(result, dlum, find_doctype_td, 3, false);
				if (m_pTextParser->GetStartDelimiter() == '[')
				{
					m_pTextParser->Rewind();
					m_pTextParser->ParseBuffer(result, dlum, skip_bracket_td, 1, false);
					strcpy(doctype, result);
				}
				else
				{
					m_pTextParser->ParseBuffer(result, dlum, find_doctype_td, 3, false);
					strcpy(doctype, result);
				}
				return true;			
			}
			else{
				m_pTextParser->Rewind();
				m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1);
			}
		}
		else
			return false;
	}
	}
	return false;
}


void *HUtilityXMLParser::GetStackData()
{
	return vlist_peek_first(stackdata_list);
}
  

bool HUtilityXMLParser::ProcessXMLData()
{
	HUtilityXMLTag xmltag(this);
		
 	while (1)
	{		
Start_Parsing:
	if (m_pTextParser->ParseBuffer(result, dlum, find_tagname_td, 1))
	{

		if (m_pTextParser->GetStartDelimiter() != '<')
			return false;

		if (SkipSpecialTags(&xmltag) == STATUS_SPECIAL_TAG_SKIPPED)
 			goto Start_Parsing;
		else
		{
			if (m_pTextParser->GetEndDelimiter() == '/' && result[0] == '\0')
			{
				m_pTextParser->Rewind();
				if (xmltag.GetOpen() == true)
				{
 					DoCallback(&xmltag, false);	
					if (!m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1))
						return false;
				}
				else
					return true;
			}
			else {
				if (m_pTextParser->GetEndDelimiter() == '/')
					m_pTextParser->SkipAhead(-1);
				
				if (m_pTextParser->GetEndDelimiter() == '>')
				{
					xmltag.SetTagname(result);
					xmltag.SetOpen(true);
				}
				else
				{
					xmltag.SetTagname(result);

					xmltag.SetOpen(true);
					if (AddProperties(&xmltag) == STATUS_TAG_FINISHED)
					{
							DoCallback(&xmltag, true);
 							DoCallback(&xmltag, false);
 							goto Start_Parsing;
					}				 
																														
				}

				if (ProcessSubtags(&xmltag) == STATUS_NO_SUBTAGS)
 						AddListItems(&xmltag);				 				
				
			}
			
			}
		}
		else
		{
			if (xmltag.GetOpen())
				return false;
			else
				return true;
		}
	}
	return true;
	
}

HUtilityXMLGenerator::HUtilityXMLGenerator()
{
	m_pTextBuffer = 0;
	m_BufferSize = 0;
	m_BufferPos = 0;
	m_Intend = 0;
	m_bFirstTag = true;
	
}


void HUtilityXMLGenerator::Reset()
{
	if (m_pTextBuffer)
		delete [] m_pTextBuffer;
	m_pTextBuffer = 0;
}

HUtilityXMLGenerator::~HUtilityXMLGenerator()
{
	Reset();
}


void HUtilityXMLGenerator::WriteToBufferInternal(const char *text, int len)
{
	if (m_BufferSize <= m_BufferPos + len + 1)
	{
		char *tbuf = new char[m_BufferSize];
		memcpy(tbuf, m_pTextBuffer, m_BufferSize);
		delete [] m_pTextBuffer;
		int newsize = (int)(m_BufferSize + strlen(text) + 200000);
		m_pTextBuffer = new char[newsize];
		memcpy(m_pTextBuffer, tbuf, m_BufferSize);
		m_BufferSize = newsize;
		delete [] tbuf;
	}
	memcpy(m_pTextBuffer + m_BufferPos, text, len);
	m_BufferPos +=len;
}

void HUtilityXMLGenerator::WriteToBuffer(const char *text)
{
	WriteToBufferInternal(text, (int)strlen(text));
}

void HUtilityXMLGenerator::Finalize()
{
	char temp = 0;
	WriteToBufferInternal(&temp, 1);
}


void HUtilityXMLGenerator::WriteTagname(const char *name)
{
	char temp[MVO_BUFFER_SIZE];
	sprintf(temp, "<%s", name);
	WriteToBuffer(temp);
}
void HUtilityXMLGenerator::WriteComment(const char *name)
{
	char temp[MVO_BUFFER_SIZE];
	sprintf(temp, "<%s>", name);
	WriteToBuffer(temp);
}


void HUtilityXMLGenerator::WriteEndtag(const char *name)
{
	char temp[MVO_BUFFER_SIZE];
	sprintf(temp, "</%s>", name);
	WriteToBuffer(temp);
}


void HUtilityXMLGenerator::WriteProperty(const char *name, HUtilityXMLProperty *xmlprop)
{
	char temp[MVO_BUFFER_SIZE];
	if (!xmlprop->m_HasQuotes)
		sprintf(temp, "%s = %s ", name, xmlprop->m_pText);
	else
		sprintf(temp, "%s = \"%s\" ", name, xmlprop->m_pText);

	WriteToBuffer(temp);
}

void HUtilityXMLGenerator::WriteListItems(HUtilityXMLTag *xmltag)
{
	char temp[MVO_BUFFER_SIZE];
	int linelen = 0;
	WriteNewline(1);
 	xmltag->StartListItemsQuery();
 	while (1)
	{			
		char *itemtext;
		if (!xmltag->GetNextListItem(&itemtext))
			break;
		linelen+=(int)strlen(itemtext);
		sprintf(temp, "%s ", itemtext);
		WriteToBuffer(temp);
		if (linelen>60 && xmltag->HasMoreItems())
		{
			WriteNewline(1);
			linelen = 0;
		}
	}
}


void HUtilityXMLGenerator::WriteNewline(int extra)
{
	WriteToBuffer("\n");
	for (int i=0;i<m_Intend + extra;i++)
		WriteToBuffer("\x09");
}

	
void HUtilityXMLGenerator::AddTag(HUtilityXMLTag *xmltag)
{
	if (m_bFirstTag)
		m_bFirstTag = false;
	else
	{
		m_Intend++;
		WriteNewline();
	}	
	
	
	vlist_t * commentlist = xmltag->GetCommentList();	
	START_LIST_ITERATION(char, commentlist);
	    WriteComment(temp);
	    WriteNewline();
	END_LIST_ITERATION(commentlist);


	WriteTagname(xmltag->GetTagname());


	

	xmltag->StartPropertyQuery();
	HUtilityXMLProperty *xmlprop;
	char *propname;
 	bool noproperty = true;
	while (1)
	{
		if (!xmltag->GetNextPropertyItem(&propname, &xmlprop))
			break;
		if (noproperty)
		{
			WriteToBuffer(" ");
			noproperty = false;
		}
		WriteProperty(propname, xmlprop);

	}
	if (xmltag->GetOpen() || xmltag->HasListItems())
	{
		WriteToBuffer(">");
		if (xmltag->HasListItems())
		{
			WriteListItems(xmltag);
			WriteNewline(0);
			WriteEndtag(xmltag->GetTagname());
			m_Intend--;
		}
	}
	else
	{
		WriteToBuffer("/>");
		m_Intend--;
	}
	xmltag->CleanupPropertyList();

 	
}

void HUtilityXMLGenerator::CloseTag(HUtilityXMLTag *xmltag)
{
		WriteNewline();
		WriteEndtag(xmltag->GetTagname());
		m_Intend--;
}


bool HUtilityXMLTag::AddPropertyFloat(const char *name, float f)
{
	char prop[MVO_BUFFER_SIZE];
	HCLOCALE(sprintf(prop,"%f", f));

	return AddProperty(name, prop, false);
}

bool HUtilityXMLTag::AddPropertyInt(const char *name, int i)
{
	char prop[MVO_BUFFER_SIZE];
	sprintf(prop,"%d", i);

	return AddProperty(name, prop, false);


}
