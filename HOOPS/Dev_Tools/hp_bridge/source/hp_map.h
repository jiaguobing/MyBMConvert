#ifndef __HP_MAP_H_
#define __HP_MAP_H_
#include "hc.h"
//#include "HP_MapList.h"
#include "parasolid_kernel.h"
#include "parafrustdefs.h" // for conditional bool def

class HXHashElt;

class HP_Map {
	
public:
	HP_Map();
	~HP_Map();
	bool AddMapping(HC_KEY key, PK_ENTITY_t entity);

	// delete functions return false if item to be deleted could not be found.
	bool DeleteMapping(HC_KEY key);
	bool DeleteMapping(PK_ENTITY_t entity);
	bool DeleteMapping(HC_KEY key, PK_ENTITY_t entity);

	// returns zero if entity not found
	HC_KEY FindMapping(PK_ENTITY_t entity);

	// returns 0 if key not found
	PK_ENTITY_t FindMapping(HC_KEY key);

	// returns number of keys found (for multiple mappings)
	unsigned long FindMapping(PK_ENTITY_t entity, HC_KEY* keys, unsigned long count);

	unsigned long Hash(HC_KEY key);
	unsigned long Hash(PK_ENTITY_t entity);

	int CheckConsistency();

private:
	void ExpandHashTable( unsigned long newTableSize );

	HXHashElt** m_pHP_list;// hash table for hoops-to-acis
	HXHashElt** m_pPH_list;// hash table for acis-to-hoops
	unsigned long m_TableSize;// size of hash table
	unsigned long m_NumEntries; // number of entries in hash table
};


#endif // __HP_MAP_H_



