// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HUTILITYTEXTPARSER_H
#define _HUTILITYTEXTPARSER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"


//! This structure holds the delimiter information for text parsing
typedef struct delimiter_s{
	const char *sd;		//!< Start Delimiter
 	bool sd_exclude;	//!< Exclude Start Delimiter
	const char *ed;		//!< End Delimiter
 	bool ed_exclude;	//!< Exclude End Delimiter
	bool checkopenclose;	//!< Parse Open/Close Brackets
}delimiter_t;

typedef struct delimiter_full_s{
 	char mc1[32];
	char mc2[32];
	int  i1;
	int  i2;
	int  len;
}delimiter_full_t;





//! The HUtilityTextParser is used for parsing of text using one or more delimiter lists
class MVO_API HUtilityTextParser  
{
public:
	/*!
	Constructs an HUtilityXMLTag object
   	*/

	HUtilityTextParser();
	virtual ~HUtilityTextParser();
	
	/*!
	Sets the list of delimiters used for parsing
 	\param dlist List of Delimiter Objects
	\param length Length of Delimiter List
  	*/
	void SetDelimiterList(const delimiter_t dlist[], unsigned int length);

	/*!
	\return Start delimiter of currently parsed string
   	*/
	char GetStartDelimiter() { return m_StartChar; }
	/*!
	\return End delimiter of currently parsed string
   	*/
	char GetEndDelimiter() { return m_EndChar; }
	
	/*!	
 	\param i Character Position to Peek
	\return Character at requested position in currently parsed string
  	*/
	char PeekAt(unsigned int i=0) { return m_pTextBuffer[m_BufferPos + i]; }
	
	/*!	
	Restarts Parsing at previous position
   	*/
	void Rewind() { m_BufferPos = m_OldBufferPos; }
	/*!	
	Skip x number of characters ahead in currently parsed string
 	\param snum Amount of Characters to Skip Ahead
   	*/
	void SkipAhead(int snum) { m_BufferPos += snum; }
	/*!	
	Parse String using delimiter list
	\param tempbuf Returns next string token
	\param dnum Number of Delimiter Object
	\param dlist Delimiter List Used for Parsing
	\param length Length of Delimiter List
	\param checkforquote True: Keeps track of quotes in string
 	\return True: String not parsed to end
   	*/
	bool ParseBuffer(char *tempbuf, int &dnum, const delimiter_t dlist[], unsigned int length, bool checkforquote = true);
	/*!	
	Parse String using delimiter list
	\param searchbuffer String to Parse
	\param tempbuf Returns next string token
	\param dnum Number of Delimiter Object
	\param dlist Delimiter List Used for Parsing
	\param length Length of Delimiter List
	\param checkforquote True: Keeps track of quotes in string
 	\return True: String not parsed to end
   	*/
	bool ParseBuffer(const char *searchbuffer, char *tempbuf, int &dnum, const delimiter_t dlist[], unsigned int length, bool checkforquote = true);
	
	/*!	
	Pass new string buffer for parsing
 	\param buffer String buffer to parse
	\param bufferlength Length of buffer
    	*/
	void InsertBuffer(const char *buffer, unsigned int bufferlength);
 	
private:
	bool CheckDelimiter(bool startdelimiter,  char *buf, unsigned int len, int dnum);
	bool CheckAllDelimiters(bool startdelimiter,  char *buf, unsigned int len, int &dnum);
 	bool MultiCompare(char const * const text, char const * const ctext, delimiter_full_t *ft);
	void SetFullDelimiter(const char *ctext, delimiter_full_t *ft);

	static unsigned long FindNextChar(char const * const text, char const c, int start);
	static unsigned long FindNextChar(char const * const text, int, char const c, int start);
	
	const char*			m_pTextBuffer;
	const delimiter_t*	m_pDelimiterList;
	delimiter_full_t*	m_pDelimiterFullList1;
	delimiter_full_t*	m_pDelimiterFullList2;
	unsigned int		m_BufferLength;
	unsigned int		m_BufferPos;
	unsigned int		m_OldBufferPos;
	unsigned int		m_DelimterListLength;
	char				m_StartChar;
	char				m_EndChar;

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif


