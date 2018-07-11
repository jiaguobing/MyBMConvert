// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! 
	\file vhash.h
	\brief A documented header file containing the vhash 
*/


#ifndef VHASH_H
#define VHASH_H

#include <stdlib.h>

#include "vlist.h"

#ifndef HOOPS_DEFINED
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif 



#ifdef HOOPS_DEFINED
#define VHASH_API HOOPS_API
#define VHASH_CDECL HC_CDECL
#else
	#ifdef LINUX_SYSTEM
		#define VHASH_API __attribute__ ((visibility ("default")))
	#else
		#define VHASH_API
	#endif
#define VHASH_CDECL

#ifdef _WIN32_WCE
#define VHASH_MEM_CDECL __cdecl
#else
#define VHASH_MEM_CDECL 
#endif

#if 0
/* use the one from vlist.h */
typedef void *(VHASH_MEM_CDECL *vmalloc_t) (size_t);
typedeef void (VHASH_MEM_CDECL *vfree_t) (void*);
#endif

#endif

#ifdef V_PREFIX

#define VNAME_TMP_2(_x2_,_y2_) _x2_##_y2_
#define VNAME_TMP_1(_x1_,_y1_) VNAME_TMP_2(_x1_,_y1_)
#define VNAME(_x_) VNAME_TMP_1(V_PREFIX,_x_)

#define new_vhash VNAME(new_vhash)
#define clone_vhash VNAME(clone_vhash)
#define vhash_flush VNAME(vhash_flush)
#define delete_vhash VNAME(delete_vhash)
#define vhash_memory_pool VNAME(vhash_memory_pool)
#define vhash_malloc VNAME(vhash_malloc)
#define vhash_free VNAME(vhash_free)
#define vhash_rebuild_table VNAME(vhash_rebuild_table)
#define vhash_count VNAME(vhash_count)
#define vhash_unique_keys VNAME(vhash_unique_keys)
#define vhash_table_size VNAME(vhash_table_size)
#define vhash_replace_item VNAME(vhash_replace_item)
#define vhash_insert_item VNAME(vhash_insert_item)
#define vhash_insert VNAME(vhash_insert)
#define vhash_remove_nth_string_key_item VNAME(vhash_remove_nth_string_key_item)
#define vhash_remove_nth_item VNAME(vhash_remove_nth_item)
#define vhash_remove_item VNAME(vhash_remove_item)
#define vhash_remove VNAME(vhash_remove)
#define vhash_lookup_nth_item VNAME(vhash_lookup_nth_item)
#define vhash_lookup_item VNAME(vhash_lookup_item)
#define vhash_lookup VNAME(vhash_lookup)
#define vhash_lookup_nth_string_key_item VNAME(vhash_lookup_nth_string_key_item)
#define vhash_lookup_string_key_item VNAME(vhash_lookup_string_key_item)
#define vhash_lookup_string_key VNAME(vhash_lookup_string_key)
#define vhash_insert_string_key_item VNAME(vhash_insert_string_key_item)
#define vhash_insert_string_key VNAME(vhash_insert_string_key)
#define vhash_remove_string_key_item VNAME(vhash_remove_string_key_item)
#define vhash_remove_string_key VNAME(vhash_remove_string_key)
#define vhash_replace_string_key_item VNAME(vhash_replace_string_key_item)
#define vhash_map_function VNAME(vhash_map_function)
#define vhash_map_function_with_return VNAME(vhash_map_function_with_return)
#define vhash_string_key_map_function VNAME(vhash_string_key_map_function)
#define vhash_string_key_map_function_with_return VNAME(vhash_string_key_map_function_with_return)
#define vhash_to_vlist VNAME(vhash_to_vlist)
#define vhash_string_keys_to_vlist VNAME(vhash_string_keys_to_vlist)
#define vhash_item_set_to_vlist VNAME(vhash_item_set_to_vlist)
#define vhash_merge_vhash VNAME(vhash_merge_vhash)
#define vhash_string_key_merge_vhash VNAME(vhash_string_key_merge_vhash)

#endif

/*!
	The vhash is a linear probing hash.

	This vhash assumes all keys are unique but doesn't enforce it
	so it can be used as a set hash also.
	If not remove the old key/item first the add the new one.

	The vhash can operate on void pointers or string keys.
	Do not mix these uses.

	This auto resizes at 50% capacity for
	best performance choose a size 2*max items expected.
*/

struct vhash_node_t;
#ifdef HOOPS_DEFINED

#include "hoops_memory.h"

namespace HOOPS {
template <typename T, class Alloc, size_t> class Banked_Array;
};
typedef HOOPS::Banked_Array<vhash_node_t, POOL_Allocator<vhash_node_t>, 4> vhash_node_t_Banked_Array;
#endif

typedef struct vhash_s {
#ifdef HOOPS_DEFINED
	vhash_node_t_Banked_Array * table;
	Memory_Pool * memory_pool;
#else
	vhash_node_t * table;
	vmalloc_t vmalloc;
	vfree_t vfree;
#endif
	unsigned long key_count;
	unsigned long unique_count;
	unsigned long table_size;
	bool is_string;

} vhash_t;

/*! \def VHASH_STATUS_FAILED
	\brief A failure status
*/
#define VHASH_STATUS_FAILED 0
/*! \def VHASH_STATUS_SUCCESS
	\brief A success status
*/
#define VHASH_STATUS_SUCCESS 1
/*! \def VHASH_STATUS_INSERTED
	\brief A item was inserted 
*/
#define VHASH_STATUS_INSERTED 2

/*! \def vhash_status_t
	\brief A type for function status
*/
typedef int vhash_status_t;

#ifdef HOOPS_DEFINED
VHASH_API vhash_t * VHASH_CDECL new_vhash(
	unsigned long table_size,
	Memory_Pool * memory_pool);
#else
/*!
	\brief This function creates and initalises a vhash structure.
	\var table_size How big to create the hash >0 please
	\var vhas_malloc A malloc function for vhash to use    
	\var vhas_free A free function for vhash to use    
	\return The vhash structure 
*/
VHASH_API vhash_t * VHASH_CDECL new_vhash(
	unsigned long table_size,
	vmalloc_t vmalloc,
	vfree_t	vfree);
#endif

/*!
	\brief This function clones a vhash structure.
	\var vhash The vhash to clone
*/
VHASH_API vhash_t * VHASH_CDECL clone_vhash(vhash_t * vhash);

/*!
	\brief This function destroys and cleans up a vhash structure.
	\var vhash The vhash to delete
*/
VHASH_API void VHASH_CDECL delete_vhash(vhash_t * vhash);

/*!
	\brief This function flushes all keys from existing hash.
	\var vhash The vhash to flush
*/
VHASH_API void VHASH_CDECL vhash_flush(vhash_t * vhash);


#ifdef HOOPS_DEFINED
VHASH_API Memory_Pool * vhash_memory_pool(vhash_t * vhash);
#else
VHASH_API vmalloc_t vhash_malloc(vhash_t * vhash);
VHASH_API vfree_t vhash_free(vhash_t * vhash);
#endif


#define VHASH_MERGE_DEFAULT					0x00000000
#define VHASH_MERGE_DESTRUCTIVE				0x00000001
#define VHASH_MERGE_REPLACE_DUPLICATES		0x00000002
#define	VHASH_MERGE_PRESERVE_DUPLICATES		0x00000004

/*!
	\brief This function merges one hash into another
	\var vhash_to The vhash to merge to
	\var vhash_from The vhash to merge from
	\var flags The how to merge
	\return a vhash_status_t the result of the merge
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_merge_vhash(
	vhash_t * vhash_to,
	vhash_t * vhash_from,
	int flags);


/*!
	\brief This function rebuilds the hash a specified size never rebuild less than 2*count
	\var vhash The vhash to operate on
	\var table_size The new size
	\return a vhash_status_t the result of the rebuild
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_rebuild_table(
	vhash_t * vhash,
	unsigned long table_size);

/*!
	\brief This function returns the number of items in the vhash.
	\var vhash The vhash to operate on
	\return The number of items in the vhash
*/
VHASH_API unsigned long VHASH_CDECL vhash_count(vhash_t* vhash);

/*!
	\brief This function returns the number of unique keys used in the vhash.
	\var vhash The vhash to operate on
	\return The number of unique keys used
*/
VHASH_API unsigned long VHASH_CDECL vhash_unique_keys(vhash_t* vhash);

/*!
	\brief This function returns the size of the hash table.
	\var vhash The vhash to operate on
	\return The size of the hash table
*/
VHASH_API unsigned long VHASH_CDECL vhash_table_size(vhash_t* vhash);

/*!
	\brief This function calls a function once per item in the vhash.
	\var vhash The vhash to operate on
	\var function The function to call (item,key,user_data) 
	\var user_data A void pointer to pass with each item
*/
VHASH_API void VHASH_CDECL vhash_map_function(
	vhash_t * v,
	void(*function)(void*, void*, void*),
	void * user_data);


typedef int vhash_map_return_t;

#define VHASH_MAP_RETURN_CONTINUE	0x00000000
#define VHASH_MAP_RETURN_STOP		0x00000001
#define VHASH_MAP_RETURN_DELETE		0x00000002


/*!
	\brief This function calls a function once per item in the vhash and can take 
			action on the item.  The current actions include deleting an item and stopping
			after finding a particular item.
	\var vhash The vhash to operate on
	\var function The function to call (item,key,user_data) 
	\var user_data A void pointer to pass with each item
*/
VHASH_API void VHASH_CDECL vhash_map_function_with_return(
	vhash_t * v,
	vhash_map_return_t (*function)(void*, void*, void*),
	void * user_data);

/*! \def vhash_pair_t
	\brief A type for key item pairs
*/
typedef struct vhash_pair_s {
	void * key;
	void * item;
#ifdef HOOPS_DEFINED
	Memory_Pool * memory_pool;
#else
	vfree_t	free;
#endif
} vhash_pair_t;

/*! \def vhash_string_key_pair_t
	\brief A type for key item pairs when the vhash is storing strings
*/
typedef struct vhash_string_key_pair_s {
	const char * key;
	void * item;
#ifdef HOOPS_DEFINED
	Memory_Pool * memory_pool;
#else
	vfree_t	free;
#endif
} vhash_string_key_pair_t;

#ifdef HOOPS_DEFINED
VHASH_API vhash_status_t VHASH_CDECL vhash_to_vlist(
	vhash_t * vhash,
	vlist_t * vlist,
	Memory_Pool * memory_pool);
#else
/*!
	\brief This function builds a vlist of item key pairs.
	\var vhash The vhash to operate on
	\var vlist A vlist to populate with key item pairs
	\var vhash_pair_malloc A malloc function to user to allocate the key item pairs
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_to_vlist(
	vhash_t * vhash,
	vlist_t * vlist,
	void *(VHASH_CDECL *vhash_pair_malloc) (size_t));
#endif

/*!
\brief This function builds a vlist of the set of items matching a key.
\var vhash The vhash to operate on
\var vlist A vlist to populate with items
\var in_key A item key
\return the number of items added to the list
*/
VHASH_API int VHASH_CDECL vhash_item_set_to_vlist(
	vhash_t * vhash,
	vlist_t * vlist,
	void * in_key);

/*!
	\brief This function replaces or adds a item to the vhash.
	\var vhash The vhash to operate on
	\var in_key A item key
	\var new_item The item to add to the vhash
	\var replaced_item A pointer to a void pointer that a replaced item can be write to
	\return a vhash_status_t the result of the action (returns VHASH_STATUS_SUCCESS always)
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_replace_item(
	vhash_t * v,
	void * in_key, 
	void * new_item,
	void ** replaced_item);

/*!
	\brief This function adds a item to the vhash.
	\var vhash The vhash to operate on
	\var in_key A item key
	\var item The item to add to the vhash
	\return a vhash_status_t the result of the action (returns VHASH_STATUS_SUCCESS always)
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_insert_item(
	vhash_t * v,
	void * in_key,
	void * item);

/*!
	\brief This function removes an item from the vhash.
	\var vhash The vhash to operate on
	\var in_key A item key
	\var removed_item A pointer to a void pointer that a removed item can be write to
	\return a vhash_status_t the result of the action 
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_remove_item(
	vhash_t * v,
	void * in_key,
	void ** removed_item);

/*!
	\brief This function looks up an item from the vhash.
	\var vhash The vhash to operate on
	\var in_key A item key
	\var out_item A pointer to a void pointer that a looked up item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_lookup_item(
	const vhash_t * v,
	void * in_key,
	void ** out_item);

/*!
	\brief This function behaves exactly like vhash_lookup_item, except it skips a number of 
		matches equal to the given 'n'.  When hash keys are not guaranteed to be unique, this function
		allows access to all of the items that match a given key.
	\var vhash The vhash to operate on
	\var in_key A item key
	\var n The number of matches to skip
	\var out_item A pointer to a void pointer that a looked up item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_lookup_nth_item(
	const vhash_t * v,
	void * in_key,
	int n,
	void ** out_item);

/*!
	\brief This function behaves exactly like vhash_remove_item, except it skips a number of 
		matches equal to the given 'n'.  When hash keys are not guaranteed to be unique, this function
		allows access to all of the items that match a given key.
	\var vhash The vhash to operate on
	\var in_key A item key
	\var n The number of matches to skip
	\var out_item A pointer to a void pointer that a removed item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_remove_nth_item(
	vhash_t * v,
	void * in_key,
	int n,
	void ** removed_item);


/*!
	\brief This function calls a function once per item in the vhash.
	\brief The keys are strings.

	\var vhash The vhash to operate on
	\var function The function to call (item,key,user_data) 
	\var user_data A void pointer to pass with each item
*/
VHASH_API void VHASH_CDECL vhash_string_key_map_function(
	vhash_t * v,
	void(*function)(void*, const char *, void*),
	void * user_data);

/*!
	\brief This function calls a function once per item in the vhash and can take 
			action on the item.  The current actions include deleting an item and stopping
			after finding a particular item.
	\brief The keys are strings.

	\var vhash The vhash to operate on
	\var function The function to call (item,key,user_data) 
	\var user_data A void pointer to pass with each item
*/
VHASH_API void VHASH_CDECL vhash_string_key_map_function_with_return(
	vhash_t * v,
	vhash_map_return_t(*function)(void*, const char *, void*),
	void * user_data);


/*!
	\brief This function replaces or adds a item to the vhash.
	\brief The keys are strings.
	\brief replaced_item is valid only if return status is VHASH_STATUS_SUCCESS 
	\brief returns VHASH_STATUS_INSERTED is no item with same key existed
	\var vhash The vhash to operate on
	\var string_key A null terminated item key
	\var new_item The item to add to the vhash
	\var replaced_item A pointer to a void pointer that a replaced item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_replace_string_key_item(
	vhash_t * v,
	const char * string_key, 
	void * new_item,
	void ** replaced_item);

/*!
	\brief This function adds a item to the vhash.
	\brief The keys are strings.
	\var vhash The vhash to operate on
	\var string_key A null terminated item key
	\var item The item to add to the vhash
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_insert_string_key_item(
	vhash_t * v,
	const char * string_key, 
	void * item);

/*!
	\brief This function removes an item from the vhash.
	\brief The keys are strings.
	\var vhash The vhash to operate on
	\var string_key A null terminated item key
	\var removed_item A pointer to a void pointer that a removed item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_remove_string_key_item(
	vhash_t * v,
	const char * string_key,
	void ** removed_item);

/*!
	\brief This function looks up an item from the vhash.
	\brief The keys are strings.
	\var vhash The vhash to operate on
	\var string_key A null terminated item key
	\var out_item A pointer to a void pointer that a looked up item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_lookup_string_key_item(
	vhash_t * v,
	const char * string_key,
	void ** out_item);

/*!
	\brief This function behaves exactly like vhash_lookup_string_key_item, except it skips a number of 
		matches equal to the given 'n'.  When hash keys are not guaranteed to be unique, this function
		allows access to all of the items that match a given key.
	\brief The keys are strings.
	\var vhash The vhash to operate on
	\var string_key A null terminated item key
	\var n The number of matches to skip
	\var out_item A pointer to a void pointer that a looked up item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_lookup_nth_string_key_item(
	vhash_t * v,
	const char * string_key,
	int n,
	void ** out_item);

/*!
	\brief This function behaves exactly like vhash_remove_string_key_item, except it skips a number of 
		matches equal to the given 'n'.  When hash keys are not guaranteed to be unique, this function
		allows access to all of the items that match a given key.
	\brief The keys are strings.
	\var vhash The vhash to operate on
	\var string_key A null terminated item key
	\var n The number of matches to skip
	\var out_item A pointer to a void pointer that a removed item can be write to
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_remove_nth_string_key_item(
	vhash_t * v,
	const char * string_key,
	int n,
	void ** out_item);

#ifdef HOOPS_DEFINED
VHASH_API vhash_status_t VHASH_CDECL vhash_string_keys_to_vlist(
	vhash_t * vhash,
	vlist_t * vlist,
	Memory_Pool * memory_pool);
#else
/*!
	\brief This function builds a string key item pair from the vhash.
	\var vhash The vhash to operate on
	\var vlist A vlist to populate with string key item pairs
	\var vhash_pair_malloc A malloc function to user to allocate the key item pairs
	\return a vhash_status_t the result of the action
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_string_keys_to_vlist(
	vhash_t * vhash,
	vlist_t * vlist,
	void *(VHASH_CDECL *vhash_pair_malloc) (size_t));
#endif

/*!
	\brief This function merges one string hash into another
	\var vhash_to The vhash to merge to
	\var vhash_from The vhash to merge from
	\var flags The how to merge
	\return a vhash_status_t the result of the merge
*/
VHASH_API vhash_status_t VHASH_CDECL vhash_string_key_merge_vhash(
	vhash_t * vhash_to,
	vhash_t * vhash_from,
	int flags);


/*THIS IS SUPERSEDED*/
VHASH_API void vhash_insert(
	vhash_t * vhash,
	void * key, void * item);

/*THIS IS SUPERSEDED*/
VHASH_API void * vhash_remove(
	vhash_t * vhash,
	void * key);

/*THIS IS SUPERSEDED*/
VHASH_API void * vhash_lookup(
	vhash_t * vhash,
	void * key);


/*THIS IS SUPERSEDED*/
VHASH_API void vhash_insert_string_key(
	vhash_t * v,
	const char * string, 
	void * item);

/*THIS IS SUPERSEDED*/
VHASH_API void * vhash_remove_string_key(
	vhash_t * v,
	const char * string);

/*THIS IS SUPERSEDED*/
VHASH_API void * vhash_lookup_string_key(
	vhash_t * v,
	const char * string);


#ifndef HOOPS_DEFINED
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
#endif 


/* template wrapper */
#ifdef __cplusplus
extern "C++" {


template <typename T1, typename T2> class VHash
#ifdef HOOPS_DEFINED
	: public CMO_Pooled
#endif
{

public:

	class Pair {
		public:
#ifdef HOOPS_DEFINED
			static void operator delete(void * p){
				FREE(p, Pair);
			};
#else
			static void operator delete(void * p){
				((Pair*)p)->vpair.free(p);
			};
#endif
			T1 Key() {
				T1 y=0;
				memcpy(&y, &vpair.key, sizeof(T1));
				return y;  
			};
			T2 Item() {
				T2 y=0;
				memcpy(&y, &vpair.item, sizeof(T2));
				return y;  
			};
		private:
			vhash_pair_t vpair;
	};

	class PairList : public VList<Pair*>
	{
		public:
		PairList(struct vlist_s * vl):VList<Pair*>(vl) {;};
		~PairList() {VList<Pair*>::DeleteItems();};
	};

	typedef vhash_status_t status;

	static const int Success=VHASH_STATUS_SUCCESS;
	static const int Failed=VHASH_STATUS_FAILED;
	static const int Inserted=VHASH_STATUS_INSERTED;

#ifdef HOOPS_DEFINED
	VHash(Memory_Pool * memory_pool, int initial_size=16)
		{m_vhash = new_vhash(initial_size, memory_pool);};
	
	VHash(VHash<T1,T2> const *hash_to_clone)
		{m_vhash = clone_vhash(hash_to_clone->m_vhash);};
#else
	VHash(int initial_size=16)
		{ m_vhash = new_vhash(initial_size,malloc,free);};

	VHash(void * (VHASH_CDECL *  vhash_malloc) (size_t), void (VHASH_CDECL *vhash_free) (void *))
		{m_vhash = new_vhash(16, vhash_malloc,vhash_free);};

	VHash(VHash<T1,T2> const *hash_to_clone)
		{m_vhash = clone_vhash(hash_to_clone->m_vhash);};
#endif

	virtual ~VHash() {delete_vhash(m_vhash);
			VLIST_COMPILE_TIME_ASSERT(sizeof(T1) <= sizeof(void *));
			VLIST_COMPILE_TIME_ASSERT(sizeof(T2) <= sizeof(void *)); };

	virtual void Flush() { vhash_flush(m_vhash); };

	void Resize(int size = -1) { vhash_rebuild_table(m_vhash, size); };

	unsigned long Count() const { return m_vhash->key_count; /*(vhash_count(m_vhash));*/ };

	unsigned long UniqueCount() const { return m_vhash->unique_count; /*(vhash_unique_keys(m_vhash));*/ };

	unsigned long TableSize() const { return m_vhash->table_size; /*(vhash_table_size(m_vhash));*/ };

#ifdef HOOPS_DEFINED
// 	PairList * GetPairList() {
//   		if (m_vhash->memory_pool)
//   			return POOL_NEW(m_vhash->memory_pool, PairList)(GetPairListInternal()); 
// 		return DEFAULT_POOL_NEW(PairList)(GetPairListInternal()); 
// 	}
#else
	PairList * GetPairList() {
		return new PairList(GetPairListInternal()); }
#endif

	status Merge(VHash<T1,T2> *hash_to_merge, int flags=VHASH_MERGE_DEFAULT) {
		return vhash_merge_vhash(m_vhash, hash_to_merge->m_vhash, flags);
	};

	void DeleteItems() {
		vhash_map_function(m_vhash, &delete_function_helper, 0);
		vhash_flush(m_vhash);
	};

	void MapFunction(void(*function)(T2, T1, void const * const), void const * const user_data=0) {
		MapFunctionData data(function,user_data);
		vhash_map_function(m_vhash, &map_function_helper, (void*)&data);
	};

	void MapFunctionWithReturn(vhash_map_return_t(*function)(T2, T1, void const * const), void const * const user_data=0) {
		MapFunctionWithReturnData data(function,user_data);
		vhash_map_function_with_return(m_vhash, &map_function_with_return_helper, (void*)&data);
	};

	status ReplaceItem(T1 key, T2 item, T2 *replaced_item=0){
		void * tmp_item = 0;
		if(replaced_item)
			*replaced_item = 0;
		void *x1=0,*x2=0;
		memcpy(&x1, &key, sizeof(T1));
		memcpy(&x2, &item, sizeof(T2));
		vhash_status_t retval = vhash_replace_item(m_vhash, x1, x2, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && replaced_item)
			memcpy(replaced_item, &tmp_item, sizeof(T2));
		return retval;
	};

	status InsertItem(T1 key, T2 item=0){
		void *x1=0,*x2=0;
		memcpy(&x1, &key, sizeof(T1));
		memcpy(&x2, &item, sizeof(T2));
		return vhash_insert_item(m_vhash, x1, x2);
	};

	status LookupItem(T1 key, T2 *looked_up_item=0){
		void * tmp_item = 0;
		if(looked_up_item)
			*looked_up_item = 0;
		void *x1=0;
		memcpy(&x1, &key, sizeof(T1));
		vhash_status_t retval = vhash_lookup_item(m_vhash, x1, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && looked_up_item)
			memcpy(looked_up_item, &tmp_item, sizeof(T2));
		return retval;
	};

	T2 Lookup(T1 key) {
		void *x1 = 0;
		memcpy(&x1, &key, sizeof(T1));
		void * tmp_item = vhash_lookup(m_vhash, x1);
		T2 y=0;
		memcpy(&y, &tmp_item, sizeof(T2));
		return y; 
	};

	T2 Remove(T1 key) {
		void *x1 = 0;
		memcpy(&x1, &key, sizeof(T1));
		void * tmp_item = vhash_remove(m_vhash, x1);
		T2 y=0;
		memcpy(&y, &tmp_item, sizeof(T2));
		return y; 
	};
	
	
	status RemoveItem(T1 key, T2 *removed_item=0){
		void * tmp_item = 0;
		if (removed_item)
			*removed_item = 0;
		void *x1 = 0;
		memcpy(&x1, &key, sizeof(T1));
		vhash_status_t retval = vhash_remove_item(m_vhash, x1, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && removed_item)
			memcpy(removed_item, &tmp_item, sizeof(T2));
		return retval;
	};
		

	status LookupNthItem(T1 key,int n, T2 *out_item=0){
		void * tmp_item = 0;
		if (out_item)
			*out_item = 0;
		void *x1=0;
		memcpy(&x1, &key, sizeof(T1));
		vhash_status_t retval = vhash_lookup_nth_item(m_vhash, x1, n, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && out_item)
			memcpy(out_item, &tmp_item, sizeof(T2));
		return retval;
	};

	status RemoveNthItem(T1 key, int n, T2 *removed_item=0){
		void * tmp_item = 0;
		if(removed_item)
			*removed_item = 0;
		void *x1=0;
		memcpy(&x1, &key, sizeof(T1));
		vhash_status_t retval = vhash_remove_nth_item(m_vhash, x1, n, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && removed_item)
			memcpy(removed_item, &tmp_item, sizeof(T2));
		return retval;
	};

private:
	vhash_t *m_vhash;

	vlist_s * GetPairListInternal() {
#ifdef HOOPS_DEFINED		
		struct vlist_s * lv = new_vlist(vhash_memory_pool(m_vhash));
#else
		struct vlist_s * lv = new_vlist(vhash_malloc(m_vhash),vhash_free(m_vhash));
#endif

		if (VHASH_STATUS_FAILED == vhash_to_vlist(m_vhash, lv, 0)) {
			delete_vlist(lv);
			return 0;
		}
		return lv;
	}

	typedef  void(*MapFunctionType)(T2, T1, void const * const);
	typedef  vhash_map_return_t(*MapFunctionWithReturnType)(T2, T1, void const * const);
	
	class MapFunctionData {
		public:	
		MapFunctionData(MapFunctionType function=0, void const * const data=0) {
			m_function=function;
			m_data=(void*)data;
		};
		MapFunctionType m_function;
		void * m_data;
	};

	class MapFunctionWithReturnData {
		public:	
		MapFunctionWithReturnData(MapFunctionWithReturnType function=0, void const * const data=0) {
			m_function=function;
			m_data=(void*)data;
		};
		MapFunctionWithReturnType m_function;
		void * m_data;
	};

	static void delete_function_helper(void* in_item, void* in_key, void*in_data) {
		T2 item=0;
		memcpy(&item, &in_item, sizeof(T2));
		delete item;
		(void)in_key;
		(void)in_data; 
	};

	static void map_function_helper(void* in_item, void* in_key, void*in_data) {
		MapFunctionData *data = (MapFunctionData *)in_data;
		T1 key=0;
		T2 item=0;
		memcpy(&key, &in_key, sizeof(T1));
		memcpy(&item, &in_item, sizeof(T2));
		data->m_function(item,key,data->m_data);
	};

	static vhash_map_return_t map_function_with_return_helper(void* in_item, void* in_key, void*in_data) {
		MapFunctionWithReturnData *data = (MapFunctionWithReturnData *)in_data;
		T1 key=0;
		T2 item=0;
		memcpy(&key, &in_key, sizeof(T1));
		memcpy(&item, &in_item, sizeof(T2));
		return data->m_function(item,key,data->m_data);
	};

};

#ifndef HOOPS_DEFINED

template <typename T1> class VStringHash 
#ifdef HOOPS_DEFINED
: public CMO_Pooled
#endif
{
public:

	class Pair {
		public:
#ifdef HOOPS_DEFINED
			static void operator delete(void * p){
				ASSERT(((Pair*)p)->vpair.memory_pool);
				FREE(p, Pair);
			};
#else
			static void operator delete(void * p){((Pair*)p)->vpair.free(p);};
#endif
			const char * Key() {return vpair.key;};
			T1 Item() {
				T1 y=0;
				memcpy(&y, &vpair.item, sizeof(T1));
				return y;  
			};
		private:
			vhash_string_key_pair_t vpair;
	};

	class PairList : public VList<Pair*> {
		public:
			PairList(struct vlist_s * vl):VList<Pair*>(vl) {;};
			~PairList() {VList<Pair*>::DeleteItems();};
	};

	typedef vhash_status_t status;

	static const int Success=VHASH_STATUS_SUCCESS;
	static const int Failed=VHASH_STATUS_FAILED;
	static const int Inserted=VHASH_STATUS_INSERTED;

#ifdef HOOPS_DEFINED
	VStringHash(Memory_Pool * memory_pool, int initial_size=16)
		{m_vhash = new_vhash(initial_size, memory_pool);};

	VStringHash(VStringHash<T1> const *hash_to_clone)
		{m_vhash = clone_vhash(hash_to_clone->m_vhash);};
#else

	VStringHash(int initial_size=16)
		{ m_vhash = new_vhash(initial_size,malloc,free);};

	VStringHash(void * (VHASH_CDECL *  vhash_malloc) (size_t), void (VHASH_CDECL *vhash_free) (void *), int initial_size=16)
		{m_vhash = new_vhash(initial_size, vhash_malloc,vhash_free); };

	VStringHash(VStringHash<T1> const *hash_to_clone)
		{m_vhash = clone_vhash(hash_to_clone->m_vhash);};
#endif

	~VStringHash() {delete_vhash(m_vhash);VLIST_COMPILE_TIME_ASSERT(sizeof(T1) <= sizeof(void *)); };

	void Flush() { vhash_flush(m_vhash); };

	void Resize(int size = -1) { vhash_rebuild_table(m_vhash, size); };

	unsigned long Count() { return (vhash_count(m_vhash)); };

	unsigned long UniqueCount() { return (vhash_unique_keys(m_vhash)); };

	unsigned long TableSize() { return (vhash_table_size(m_vhash)); };

	status ReplaceItem(const char *key, T1 item, T1 *replaced_item=0){
		void * tmp_item = 0;
		if (replaced_item)
			*replaced_item = 0; 
		void *x1=0;
		memcpy(&x1, &item, sizeof(T1)); //-V512
		vhash_status_t retval = vhash_replace_string_key_item(m_vhash, key, x1, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && replaced_item)
			memcpy(replaced_item, &tmp_item, sizeof(T1)); //-V512
		return retval;
	};

	status InsertItem(const char *key, T1 item=0) {
		void *x1=0;
		memcpy(&x1, &item, sizeof(T1)); //-V512
		return vhash_insert_string_key_item(m_vhash, key, x1); 
	};
	
	status LookupItem(const char *key, T1 *looked_up_item=0){
		void * tmp_item = 0;
		if(looked_up_item)
			*looked_up_item = 0;
		vhash_status_t retval = vhash_lookup_string_key_item(m_vhash, key, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && looked_up_item)
			memcpy(looked_up_item, &tmp_item, sizeof(T1)); //-V512
		return retval;	
	};


	T1 Lookup(const char *key) {
		void * tmp_item = vhash_lookup_string_key(m_vhash, key);
		T1 y=0;
		memcpy(&y, &tmp_item, sizeof(T1)); //-V512
		return y; 
	};

	T1 Remove(const char *key) {
		void * tmp_item = vhash_remove_string_key(m_vhash, key);
		T1 y=0;
		memcpy(&y, &tmp_item, sizeof(T1)); //-V512
		return y; 
	};
	
	status RemoveItem(const char *key, T1 *removed_item=0){
		void * tmp_item = 0;
		if (removed_item)
			*removed_item = 0;
		vhash_status_t retval = vhash_remove_string_key_item(m_vhash, key, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && removed_item)
			memcpy(removed_item, &tmp_item, sizeof(T1)); //-V512
		return retval;
	};

	status LookupNthItem(const char *key, int n, T1 *out_item=0){
		void * tmp_item = 0;
		if (out_item)
			*out_item = 0;
		vhash_status_t retval = vhash_lookup_nth_string_key_item(m_vhash, key, n, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && out_item)
			memcpy(out_item, &tmp_item, sizeof(T1)); //-V512
		return retval;
	};

	status RemoveNthItem(const char *key, int n, T1 *removed_item=0){
		void * tmp_item = 0;
		if (removed_item)
			*removed_item = 0;
		vhash_status_t retval = vhash_remove_nth_string_key_item(m_vhash, key, n, &tmp_item);
		if (VHASH_STATUS_SUCCESS == retval && removed_item)
			memcpy(removed_item, &tmp_item, sizeof(T1)); //-V512
		return retval;
	};

	PairList * GetPairList() {
#ifdef HOOPS_DEFINED
		struct vlist_s *lv = new_vlist(vhash_memory_pool(m_vhash));

		if (VHASH_STATUS_FAILED == vhash_string_keys_to_vlist(m_vhash, lv, 0)) {
			delete_vlist(lv);
			return 0;
		}
		PairList * pl;
		if (m_vhash->memory_pool)
			pl = POOL_NEW(m_vhash->memory_pool, PairList)(lv);
		else
			pl = DEFAULT_POOL_NEW(PairList)(lv);
		return pl;
#else
		struct vlist_s *lv = new_vlist(vhash_malloc(m_vhash),vhash_free(m_vhash));

		if (VHASH_STATUS_FAILED == vhash_string_keys_to_vlist(m_vhash, lv, 0)) {
			delete_vlist(lv);
			return 0;
		}
		PairList * pl = new PairList(lv);
		return pl;
#endif
	};

	status Merge(VStringHash<T1> *hash_to_merge, int flags=VHASH_MERGE_DEFAULT) {
		return vhash_string_key_merge_vhash(m_vhash, hash_to_merge->m_vhash, flags);
	};
	
	void DeleteItems() {
		vhash_string_key_map_function(m_vhash, &delete_function_helper, 0);
		vhash_flush(m_vhash);
	};

	void MapFunction(void(*function)(T1, const char *, void const * const), void const * const user_data=0) {
		MapFunctionData data(function,user_data);
		vhash_string_key_map_function(m_vhash, &map_function_helper, (void*)&data);
	};

	void MapFunctionWithReturn(vhash_map_return_t(*function)(T1, const char *, void const * const), 
								void const * const user_data=0) {
		MapFunctionWithReturnData data(function,user_data);
		vhash_string_key_map_function_with_return(m_vhash, &map_function_with_return_helper, (void*)&data);
	};

private:
	vhash_t *m_vhash;

	typedef  void(*MapFunctionType)(T1, const char *, void const * const);
	typedef  vhash_map_return_t(*MapFunctionWithReturnType)(T1, const char *, void const * const);
	
	class MapFunctionData {
		public:	
		MapFunctionData(MapFunctionType function=0, void const * const data=0) {
			m_function=function;
			m_data=(void*)data;
		};
		MapFunctionType m_function;
		void * m_data;
	};

	class MapFunctionWithReturnData {
		public:	
		MapFunctionWithReturnData(MapFunctionWithReturnType function=0, void const * const data=0) {
			m_function=function;
			m_data=(void*)data;
		};
		MapFunctionWithReturnType m_function;
		void * m_data;
	};

	static void delete_function_helper(void* in_item, const char * in_key, void*in_data) {
		T1 item=0;
		memcpy(&item, &in_item, sizeof(T1)); //-V512
		delete item;
		(void)in_data;
		(void)in_key;
	};

	static void map_function_helper(void* in_a, const char * in_b, void*in_data) {
		MapFunctionData *data = (MapFunctionData *)in_data;
		T1 a=0;
		memcpy(&a, &in_a, sizeof(T1)); //-V512
		data->m_function(a,in_b,data->m_data);
	};

	static vhash_map_return_t map_function_with_return_helper(void* in_a, const char * in_b, void*in_data) {
		MapFunctionWithReturnData *data = (MapFunctionWithReturnData *)in_data;
		T1 a=0;
		memcpy(&a, &in_a, sizeof(T1)); //-V512
		return data->m_function(a,in_b,data->m_data);
	};

};
#endif

} /* extern "C++" */
#endif


#endif /*VHASH_H*/


