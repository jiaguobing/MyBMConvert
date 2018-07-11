// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <memory.h>
#include "hc.h"
#include "HIndexManager.h"
#define EXCLUDE_VLIST //avoid creating a dependency on vlist via the vhash_to_vlist functions
#include "vhash.h"

#ifdef _DEBUG
#  include <assert.h>
#  define ASSERT(x) assert(x);
#else
#  define ASSERT(x)
#endif



HIndexManager::HIndexManager (bool needPairToIndex, bool needIndexToPair) 
{
	m_p2i = needPairToIndex;
	m_i2p = needIndexToPair;
	m_IndexToPairs = 0;
	m_SharedVertices = 0;
	m_ShellLookup = 0;
	m_PairToIndex = 0;
	m_IndexToPairsAllocated = 0;
	m_IndexToPairsUsed = 0;
	m_SharedVerticesAllocated = 0;
	m_SharedVerticesUsed = 0;
	m_ShellLookupAllocated = 0;
	m_ShellLookupUsed = 0;
	m_PairToIndexAllocated = 0;
	m_PairToIndexUsed = 0;
}


HIndexManager::~HIndexManager () 
{
	int i;

	for (i = 0 ; i < m_SharedVerticesUsed ; i++)
		delete [] m_SharedVertices[i].IndexToPair;
	for (i = 0 ; i < m_PairToIndexUsed ; i++)
		delete [] m_PairToIndex[i].bigIndices;
	delete [] m_IndexToPairs;
	delete [] m_SharedVertices;
	delete [] m_ShellLookup;
	delete [] m_PairToIndex;
}


void HIndexManager::ExpandSharedVertices()
{
	while (m_SharedVerticesUsed == m_SharedVerticesAllocated)
		if (m_SharedVerticesAllocated < 32)
			m_SharedVerticesAllocated = 32;
		else
			m_SharedVerticesAllocated = (int)(m_SharedVerticesAllocated * 1.2);
	SharedVertex *temp = new SharedVertex[m_SharedVerticesAllocated];
	if (m_SharedVertices) {
		memcpy (temp, m_SharedVertices, m_SharedVerticesUsed*sizeof(SharedVertex));
		delete [] m_SharedVertices;
	}
	m_SharedVertices = temp;
}


void HIndexManager::ExpandShellLookup(int range_count)
{
	while (m_ShellLookupUsed + range_count > m_ShellLookupAllocated)
		if (m_ShellLookupAllocated < 32)
			m_ShellLookupAllocated = 32;
		else
			m_ShellLookupAllocated = (int)(m_ShellLookupAllocated * 1.2);
	ShellInfo *temp = new ShellInfo[m_ShellLookupAllocated];
	if (m_ShellLookup) {
		memcpy (temp, m_ShellLookup, m_ShellLookupUsed*sizeof(ShellInfo));
		delete [] m_ShellLookup;
	}
	m_ShellLookup = temp;
}


void HIndexManager::	ExpandPairToIndices()
{
	while (m_PairToIndexUsed == m_PairToIndexAllocated)
		if (m_PairToIndexAllocated < 32)
			m_PairToIndexAllocated = 32;
		else
			m_PairToIndexAllocated = (int)(m_PairToIndexAllocated * 1.2);
	PairToIndex *temp = new PairToIndex[m_PairToIndexAllocated];
	if (m_PairToIndex) {
		memcpy (temp, m_PairToIndex, m_PairToIndexUsed*sizeof(PairToIndex));
		delete [] m_PairToIndex;
	}
	m_PairToIndex = temp;
}


#undef HIGHBIT
#define HIGHBIT 0x80000000

void HIndexManager::AddOneIndexToPair (int bigIndex, int littleIndex)
{
	if (m_i2p) {
		int si_index = m_ShellLookupUsed + (littleIndex >> VI_BITS); 
		ShellInfo const *si;
		unsigned int vertexref;
		SharedVertex *sv = 0;
		ASSERT (si_index < MAX_SI_INDEX);
		si = &m_ShellLookup[si_index];
		vertexref = (si_index << VI_BITS) | (littleIndex - si->offset);

		if (m_IndexToPairs[bigIndex] == UNUSED_INDEX_TO_PAIR) {
			/* this is the first pair to inherit from bigIndex */
			m_IndexToPairs[bigIndex] = vertexref;
			m_IndexToPairsUsed++;
		}
		else if (m_IndexToPairs[bigIndex] & HIGHBIT) {
			/* bigIndex already had 2 or more pairs inheriting from bigIndex */
			sv = &m_SharedVertices[ m_IndexToPairs[bigIndex] & ~HIGHBIT ];
			int oldcount = sv->IndexToPair[0];
			unsigned int *temp = new unsigned int[oldcount+2];
			memcpy (temp, sv->IndexToPair, (oldcount+1) * sizeof(int));
			delete [] sv->IndexToPair;
			sv->IndexToPair = temp;
			sv->IndexToPair[0] = oldcount+1;
			sv->IndexToPair[oldcount+1] = vertexref;
		}
		else {
			/* this is the second pair to inherit from bigIndex */
			if (m_SharedVerticesUsed == m_SharedVerticesAllocated) {
				ExpandSharedVertices();
			}
			sv =&m_SharedVertices[ m_SharedVerticesUsed ];
			sv->IndexToPair = new unsigned int[3];
			sv->IndexToPair[0] = 2;
			sv->IndexToPair[1] = m_IndexToPairs[bigIndex];
			sv->IndexToPair[2] = vertexref;
			m_IndexToPairs[bigIndex] = (HIGHBIT | m_SharedVerticesUsed);
			m_SharedVerticesUsed++;
		}
	}
	if (m_p2i) {
		m_PairToIndex[m_PairToIndexUsed].bigIndices[littleIndex] = bigIndex;
	}
}

void HIndexManager::RemoveOneTranslation (HC_KEY key, int littleIndex)
{
	int i, oldcount, bigIndex;

	if (!GetIndex (key, littleIndex, &bigIndex))
		return;

	if (m_i2p) {
		if (m_IndexToPairs[bigIndex] == UNUSED_INDEX_TO_PAIR) {
			//nothing to do
			return;
		}
		else if (m_IndexToPairs[bigIndex] & HIGHBIT) {
			SharedVertex *sv = 0;

			//bigIndex already had (at one point, anyways) 2 or more key/index 
			//  pairs inheriting from bigIndex
			sv = &m_SharedVertices[ m_IndexToPairs[bigIndex] & ~HIGHBIT ];
			oldcount = sv->IndexToPair[0];
			if (oldcount == 1) {
				delete [] sv->IndexToPair;
				m_IndexToPairs[bigIndex] = UNUSED_INDEX_TO_PAIR;
			}
			else {
				//downsize the array
				unsigned int *temp;
				//search for the item getting killed, and overwrite it with the tail
				for (i = 1 ;  ; i++) {
					int si_index = sv->IndexToPair[i] >> VI_BITS;
					ShellInfo const *si = &m_ShellLookup[si_index];
					
					if (si->key == key) {
						sv->IndexToPair[i] = sv->IndexToPair[oldcount];
						break;
					}
					if (i >= oldcount)
						return;
				}
				temp = new unsigned int[oldcount];
				memcpy (temp, sv->IndexToPair, (oldcount) * sizeof(int));
				sv->IndexToPair = temp;
				sv->IndexToPair[0] = oldcount-1;
			}
		}
		else {
			//it was a singleton.  simply reassign.
			m_IndexToPairs[bigIndex] = UNUSED_INDEX_TO_PAIR;
		}
	}
	if (m_p2i) {
		// GetIndex swaps the relevant m_PairToIndex to the front
		m_PairToIndex[0].bigIndices[littleIndex] = -1;
	}
}


int HIndexManager::GetPairCount (int bigIndex) const
{
	SharedVertex const *sv = 0;

	if (m_IndexToPairs[bigIndex] == UNUSED_INDEX_TO_PAIR) {
		return 0;
	}
	else if (m_IndexToPairs[bigIndex] & HIGHBIT) {
		/* bigIndex has 2 or more pairs inheriting from bigIndex */
		sv = &m_SharedVertices[ m_IndexToPairs[bigIndex] & ~HIGHBIT ];
		return sv->IndexToPair[0];
	}
	else {
		/* bigIndex has exactly one key/index pair inheriting from bigIndex */
		return 1;
	}
}


bool HIndexManager::GetPair (int bigIndex, int n, HC_KEY *key, int *littleIndex) const
{
	unsigned int vertexref = UNUSED_INDEX_TO_PAIR;
	SharedVertex const *sv = 0;
	ShellInfo const *si = 0;

	if (m_IndexToPairs[bigIndex] == UNUSED_INDEX_TO_PAIR) {
		return false;
	}
	else if (m_IndexToPairs[bigIndex] & HIGHBIT) {
		/* bigIndex has 2 or more pairs inheriting from bigIndex */
		sv = &m_SharedVertices[ m_IndexToPairs[bigIndex] & ~HIGHBIT ];
		int count = sv->IndexToPair[0];
		if (n > count)
			return false;
		else
			vertexref = sv->IndexToPair[n+1];
	}
	else {
		/* bigIndex has exactly one key/index pair inheriting from bigIndex */
		if (n > 1)
			return false;
		vertexref = m_IndexToPairs[bigIndex];
	}
	ASSERT ((vertexref & HIGHBIT) == 0);
	si = &m_ShellLookup[vertexref >> VI_BITS];
	if (key)
		*key = si->key;
	if (littleIndex)
		*littleIndex = si->offset + (vertexref & VI_MASK);
	return true;
}


bool HIndexManager::GetPairs (int bigIndex, HC_KEY *keys, int *littleIndices) const 
{
	int i, count;

	count = GetPairCount (bigIndex);
	for (i = 0 ; i < count ; i++) {
		if (!GetPair (bigIndex, i, &keys[i], &littleIndices[i]))
			return false;
	}
	return true;
}


bool HIndexManager::GetIndex (HC_KEY key, int littleIndex, int *bigIndex) const
{
	int i;

	//linear search for the key in the m_PairToIndex array
	for (i = 0 ; i < m_PairToIndexUsed ; i++) {
		if (key == m_PairToIndex[i].key) {
			if (i != 0) {
				/* swap the PairToIndex that we found with the first entry so
					that if we look for it again, we'll find it faster. */
				PairToIndex temp;
				memcpy (&temp, &m_PairToIndex[i], sizeof(PairToIndex));
				memcpy (&m_PairToIndex[i], &m_PairToIndex[0], sizeof(PairToIndex));
				memcpy (&m_PairToIndex[0], &temp, sizeof(PairToIndex));
			}
			if (bigIndex)
				*bigIndex = m_PairToIndex[0].bigIndices[littleIndex];
			return true;
		}
	}
	return false;
}


#undef I2V
#define I2V(x) ((void *)(MVO_POINTER_SIZED_INT)(x))

void HIndexManager::AddOneTranslation (void *arg1, void *arg2, void *arg3)
{
	MVO_POINTER_SIZED_INT littleIndex = (MVO_POINTER_SIZED_INT)arg1;
	MVO_POINTER_SIZED_INT bigIndex = (MVO_POINTER_SIZED_INT)arg2;
	HIndexManager *im = (HIndexManager *)arg3;

	memcpy (&im->m_NewPoints[littleIndex], &im->m_OldPoints[bigIndex], sizeof(HPoint));
	im->AddOneIndexToPair (bigIndex, littleIndex);
}


HC_KEY HIndexManager::KInsertShell (
	   int point_count, 
	   const HPoint *points, 
	   int face_list_length, 
	   const int *face_list)
{
	vhash_t *hash;
	const int *face, *end;
	int *new_face_list, *newface;
	int i, length, status;
	int new_point_count = 0;
	MVO_POINTER_SIZED_INT index;
	HC_KEY key;

	hash = new_vhash (2 * face_list_length, malloc, free);

	//get a count of the unique vertex indices and set up new_face_list
	face = face_list;
	end = face_list + face_list_length;
	new_face_list = new int[face_list_length];
	newface = new_face_list;
	while (face < end) {
		length = *newface = *face;
		face++;
		newface++;
		for (i = 0 ; i < length ; i++) {
			status = vhash_lookup_item (hash, I2V(face[i]), (void **)&index);
			if (status != VHASH_STATUS_SUCCESS) {
				index = new_point_count++;
				vhash_insert_item (hash, I2V(face[i]), I2V(index));
			}
			newface[i] = index;
		}
		face += length;
		newface += length;
	}

	int range_count = 1 + ((new_point_count-1) >> VI_BITS);
	if (m_i2p) {
		//expand the m_ShellLookup array if necessary
		if (m_ShellLookupUsed + range_count > m_ShellLookupAllocated)
			ExpandShellLookup(range_count);
		for (i = 0 ; i < range_count ; i++) {
			m_ShellLookup [m_ShellLookupUsed + i].key = -1; //not known yet
			m_ShellLookup [m_ShellLookupUsed + i].offset = i << VI_BITS;
		}
		//allocate the m_IndexToPairs array if necessary
		if (point_count > m_IndexToPairsAllocated) {
			//for now, the master points array is assumed to be a constant size
			ASSERT (m_IndexToPairsAllocated == 0);  
			m_IndexToPairs = new unsigned int [point_count];
			memset (m_IndexToPairs, 0xff, point_count * sizeof(unsigned int));
			ASSERT (m_IndexToPairs[0] == UNUSED_INDEX_TO_PAIR);
			m_IndexToPairsAllocated = point_count;
		}
	}
	//expand the m_PairToIndex array if necessary
	if (m_p2i) {
		if (m_PairToIndexUsed == m_PairToIndexAllocated)
			ExpandPairToIndices();
		m_PairToIndex[m_PairToIndexUsed].key = -1; //not known yet
		m_PairToIndex[m_PairToIndexUsed].bigIndices = new int[new_point_count];
	}
	m_NewPoints = new HPoint[new_point_count];
	m_OldPoints = points;

	//generate one 'AddOneTranslation' callback per littleIndex/bigIndex pair
	vhash_map_function (hash, AddOneTranslation, this);

	key = HC_KInsert_Shell (new_point_count, m_NewPoints, face_list_length, new_face_list);

	//now that we know the key, fill it in in the m_ShellLookup and
	// m_PairToIndex arrays, and increment the 'used' counters.
	if (m_i2p)
		for (i = 0 ; i < range_count ; i++)
			m_ShellLookup [m_ShellLookupUsed++].key = key;
	if (m_p2i)
		m_PairToIndex[m_PairToIndexUsed++].key = key;

	delete [] new_face_list;
	delete [] m_NewPoints;
	m_NewPoints = 0;
	m_OldPoints = 0;
	delete_vhash (hash);
	return key;
}

void HIndexManager::DeleteByKey (
	   HC_KEY key)
{
	int *face_list, *ptr, *end;
	int i, face_list_length, len;

	HC_Show_Shell_Size (key, 0, &face_list_length);
	if (face_list_length) {
		face_list = new int[face_list_length];
		HC_Show_Shell (key, 0, 0, &face_list_length, face_list);
		ptr = face_list;
		end = face_list + face_list_length;
		while (ptr < end) {
			len = *ptr++;
			if (len < 0)
				len = -len;
			for (i = 0 ; i < len ; i++) {
				RemoveOneTranslation (key, ptr[i]);
			}
			ptr += len;
		}
		delete [] face_list;
	}
	HC_Delete_By_Key (key);
}

void HIndexManager::EditShellPoints (
		HC_KEY key, 
		int littleIndexOffset, 
		int ndelete, 
		int insert, 
		const HPoint *points)
{
	int i, point_count, oldShellLookupUsed;

	HC_Show_Shell_Size (key, &point_count, NULL);
	HC_Edit_Shell_Points (key, littleIndexOffset, ndelete, insert, points);
	if (ndelete == insert)
		return;
	if (littleIndexOffset == -1)
		littleIndexOffset = point_count;
	else for (i = 0 ; i < ndelete ; i++)
		RemoveOneTranslation (key, littleIndexOffset+i);
	oldShellLookupUsed = m_ShellLookupUsed;
	for (i = 0 ; i < insert ; i++)
		AddOneIndexToPair (-1 /*???*/, littleIndexOffset+i);
	for (i = oldShellLookupUsed ; i < m_ShellLookupUsed ; i++) {
		m_ShellLookup [m_ShellLookupUsed + i].key = key; 
		m_ShellLookup [m_ShellLookupUsed + i].offset = i << VI_BITS; /*???*/
	}
}



