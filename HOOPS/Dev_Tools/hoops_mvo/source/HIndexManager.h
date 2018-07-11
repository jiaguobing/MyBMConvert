// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIO_INDEX_MANAGER_
#define _HIO_INDEX_MANAGER_

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

#define VI_BITS			(16) //number of bits per section of vertex indices
#define VI_MASK			((1<<VI_BITS)-1)
#define MAX_SI_INDEX	((0x1 << (31-VI_BITS))-1)


typedef struct {
	HC_KEY key;
	MVO_POINTER_SIZED_INT offset; //pointer-sized to maintain alignment
} ShellInfo;


typedef struct {
	HC_KEY key;
	int *bigIndices;
} PairToIndex;


#define UNUSED_INDEX_TO_PAIR 0xffffffff
/* interpretation of IndexToPair (stored as unsigned int) values:
    if (HIGHBIT)
	    if (0xffffffff)
		    unused entry
		else
		    use the remaining 31 bits as an index into m_SharedVertices
	else
		use the remaining upper 31-VI_INDEX bits as in index into the m_ShellLookup
		add the ShellInfo.offset to the lower VI_INDEX bits to form the shell's vertex index
		grab the shell's key out of the ShellInfo
*/


typedef struct {
	//first int is the count, subsequent ints are interpreted as IndexToPair
	unsigned int *IndexToPair;
} SharedVertex;

/* STORAGE REQUIREMENTS:
	IndexToPair: 
	1 int per unshared vertex
	1 ptr + (2+N) ints for a vertex shared N times
	1 ptr + 1 int per shell

	PairToIndex
	1 ptr per shell
	1 int per shell vertex

   RESTRICTIONS:
    there can be no more than 2^15 ShellInfo structures.  Thus there can be 
	no more than 32k shells.  For these purposes, a shell with more than 2^VI_BITS 
	vertices counts as point_count >> VI_BITS.
*/


class MVO_API HIndexManager
{
  private:
	void ExpandSharedVertices();
	void ExpandShellLookup(int count);
	void ExpandPairToIndices();

	HPoint const *m_OldPoints; //needed for AddOneTrnslation
	HPoint * m_NewPoints;       //temporary scratch space

  protected:
    bool m_p2i;
	bool m_i2p;
    
    unsigned int *m_IndexToPairs;
	int m_IndexToPairsAllocated;
	int m_IndexToPairsUsed;

    SharedVertex *m_SharedVertices;
	int m_SharedVerticesAllocated;
	int m_SharedVerticesUsed;

    ShellInfo *m_ShellLookup;
	int m_ShellLookupAllocated;
	int m_ShellLookupUsed;

	PairToIndex *m_PairToIndex;
	int m_PairToIndexAllocated;
	int m_PairToIndexUsed;

	void AddOneIndexToPair (int bigIndex, int littleIndex);
	void RemoveOneTranslation (HC_KEY key, int littleIndex);
	void RemoveTranslations (HC_KEY key, int face_list_length, const int *face_list);

	static void AddOneTranslation (void *arg1, void *arg2, void *arg3); 
  public: 
	HIndexManager (bool needPairToIndex, bool needIndexToPair);
	virtual ~HIndexManager ();

	/*! Compacts a sparsely-referenced points array into its minimal subset,
	    records the translation information from the original specification.
		Requires that a HOOPS segment be open.
		\param point_count the number of points in the master points list.
		\param points the master points list.
		\param face_list_length the length of the face_list array.
		\param face_list the vertex indices that connect to form faces (see docs for HC_Insert_Shell.
		\returns the key to the HOOPS shell that was inserted.
 	*/	
	HC_KEY KInsertShell (int point_count, const HPoint *points, int face_list_length, const int *face_list);

	/*! Calls through to HOOPS to remove the item.  If the item was inserted by 
		HIndexManager::KInsertShell, this function additionally removes the 
		translation information
		\param key the key to the item to be deleted, as returned from this->KInsertShell. */	
	void DeleteByKey (HC_KEY key);

	void EditShellPoints (HC_KEY key, int littleIndexOffset, int ndelete, int insert, const HPoint *points);

	/*! just a simple call through to HC_Edit_Shell_Faces.  Nothing needs to be done here, but it provides consistency in the calling conventions.
	\param key the key to the item to be deleted, as returned from this->KInsertShell. */	
	void EditShellFaces (HC_KEY key, int ioffset, int ndelete, int insert_list_length, const int *insert_list) {
		HC_Edit_Shell_Faces (key, ioffset, ndelete, insert_list_length, insert_list);
	}

	//! returns the number of times vertex bigIndex is shared into littleIndex values
	int GetPairCount (int bigIndex) const;
	/*! retrieves the nth key/littleIndex pair that inherits from bigIndex
		\param bigIndex the index into the master points array
		\param n which of the shared vertices to return (e.g. the i in a for(i=0;;i++) loop)
		\param key returned to user
		\param littleIndex returned to user
		\returns true if successful */
	bool GetPair (int bigIndex, int n, HC_KEY *key, int *littleIndex) const;
	/*! retrieves all key/littleIndex pairs that inherit from bigIndex
		\param bigIndex the index into the master points array
		\param keys returned to user. Caller must be allocate to length GetPairCount()
		\param littleIndices returned to user. Caller must be allocate to length GetPairCount()
		\returns true if successful */
	bool GetPairs (int bigIndex, HC_KEY *keys, int *littleIndices) const;
	//! returns the bigIndex from which the key/littleIndex pair came
	bool GetIndex (HC_KEY key, int littleIndex, int *bigIndex) const;
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif






