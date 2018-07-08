#ifndef __BULLETIN_H_
#define __BULLETIN_H_
#include "parasolid_kernel.h"
#include "parafrustdefs.h"



class HP_BulletinBoard
{
public:
	HP_BulletinBoard();
	~HP_BulletinBoard();

	bool FilterEvent(
		PK_ENTITY_t bbEntity,
		PK_BB_event_t bbEvent_type,
		unsigned long num_entities,
		PK_ENTITY_t *created, unsigned long *num_created,
		PK_ENTITY_t *deleted, unsigned long *num_deleted,
		PK_ENTITY_t *altered, unsigned long *num_altered,
		PK_ENTITY_t *transformed, unsigned long *num_transformed);

//	bool FilterEvents(PK_ENTITY_t bbEntity,
//							PK_BB_event_t bbEvent_type,
//							PK_ENTITY_t *notable_entities,
//							PK_BB_event_t* notable_event_types,
//							unsigned long* num_notable_events,
//							unsigned long max_num_notable_events);
	bool GetChangedEntities(
		PK_ENTITY_t **created, unsigned long *num_created,
		PK_ENTITY_t **deleted, unsigned long *num_deleted,
		PK_ENTITY_t **altered, unsigned long *num_altered,
		PK_ENTITY_t **tranformed, unsigned long *num_transformed);

	bool CompressAndAllocEventArray(
		PK_ENTITY_t* from_array, 
		unsigned long num_from_entries, 
		PK_ENTITY_t** to_array, 
		unsigned long* num_to_entries); 

	void UpdateBulletins(); PK_BB_t GetBB( void ) { return m_BB; }

private:
	void CreateBB();
	PK_BB_t  m_BB;
};



#endif




