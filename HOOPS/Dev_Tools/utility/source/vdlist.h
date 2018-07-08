// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! 
    \file vdlist.h
    \brief A documented header file containing the vdlist 
*/

#ifndef VDLIST_H
#define VDLIST_H


#include <stdlib.h>

#ifdef HOOPS_DEFINED
#error // this should not happen
#endif
 
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef _MSC_VER
#define VDLIST_SYS_CDECL __cdecl
#else
#define VDLIST_SYS_CDECL 
#endif

#ifdef LINUX_SYSTEM
	#define VDLIST_API __attribute__ ((visibility ("default")))
#else
	#define VDLIST_API extern "C"
#endif

#define VDLIST_CDECL

#ifdef _MSC_VER
#define VDLIST_SYS_CDECL __cdecl
#else
#define VDLIST_SYS_CDECL 
#endif

typedef void *(VDLIST_SYS_CDECL *vmalloc_t) (size_t);
typedef void (VDLIST_SYS_CDECL *vfree_t) (void*);


typedef int (VDLIST_SYS_CDECL *vcompare_t) (const void*, const void *, const void *);


/*!
	The vdlist is doubly linked list that stores void pointers.
	It has a cursor to traverse the list. 
*/
typedef struct vdlist_node_s{
	void * item;
	struct vdlist_node_s * next;
	struct vdlist_node_s * prev;
} vdlist_node_t;

typedef struct vdlist_cursor_s{
	vdlist_node_t * node;
	char state;
	long index;
} vdlist_cursor_t;

/* Definitions for the states of a vdlist cursor 
 * VDLIST_BEGIN is the state if any of the following are true
 * 1) list is empty
 * 2) cursor has rewound off list
 * VDLIST_END is the state if any of the following are true
 * 1) list is non-empty and the node the cursor was pointing at was removed
 * 2) cursor has advanced off list
 * VDLIST_ON_LIST is the state otherwise
 *
 * The vdlist tries to maintain the primary cursor when the list
 * is changed by adding or removing. However, the vdlist knows nothing of the 
 * unique cursors. A unique cursor must be reset before using again when
 * your code modifies the list using anything *but* that unique cursor.
 * 
 * A cursor at VDLIST_BEGIN will have index -1.
 * A cursor at VDLIST_END will have index equal to count.
 */
#define VDLIST_BEGIN		0x00
#define VDLIST_ON_LIST		0x01
#define VDLIST_END			0x02

typedef struct vdlist_s{
	vdlist_node_t * head;
	vdlist_node_t * tail;
	vdlist_cursor_t cursor;
	long count;

	vmalloc_t vmalloc;
	vfree_t vfree;

} vdlist_t;

/*!
    \brief This function creates and initalises a vdlist structure.
	\var vdlist_malloc A malloc function for vdlist to use	
	\var vdlist_free A free function for vdlist to use	
	\return The vdlist structure 
*/
VDLIST_API vdlist_t* VDLIST_CDECL new_vdlist(
	vmalloc_t vdlist_malloc,
	vfree_t vdlist_free);

/*!
    \brief This function clones a vdlist structure.
	\var vdlist The list to clone
*/
VDLIST_API vdlist_t* VDLIST_CDECL clone_vdlist( 
	vdlist_t* vdlist); 

/*!
    \brief This function destroys and cleans up a vdlist structure.
	\var vdlist The list to delete
*/
VDLIST_API void VDLIST_CDECL delete_vdlist( 
	vdlist_t* vdlist); 

/*!
    \brief This function adds an item to the head of a vdlist structure.
	\var vdlist The list operate on
	\var item A void pointer to store 
*/
VDLIST_API void VDLIST_CDECL vdlist_add_first( 
	vdlist_t* vdlist, 
	void* item);

/*!
    \brief This function adds an item to the tail of a vdlist structure.
	\var vdlist The list operate on
	\var item A void pointer to store 
*/
VDLIST_API void VDLIST_CDECL vdlist_add_last( 
	vdlist_t* vdlist, 
	void* item);


/*!
    \brief This function adds an item before the cursor of a vdlist structure.
	\var vdlist The list operate on
	\var item A void pointer to store 
*/
VDLIST_API void VDLIST_CDECL vdlist_add_before_cursor( 
	vdlist_t* vdlist, 
	void* item);

/*!
    \brief This function adds an item after the cursor to the vdlist structure.
	\var vdlist The list operate on
	\var item A void pointer to store 
*/
VDLIST_API void VDLIST_CDECL vdlist_add_after_cursor(
	vdlist_t* vdlist,
	void *item);

/*!
    \brief This function removes an item from the vdlist structure.
	\var vdlist The list operate on
	\var item A void pointer to remove from the list 
	\return 0 if item not found ~0 if found
*/
VDLIST_API int VDLIST_CDECL vdlist_remove(
	vdlist_t* vdlist, 
	void* item);

/*!
    \brief This function removes the first item from the vdlist structure.
	\var vdlist The list operate on
	\return 0 if vdlist is empty or first item pointer
*/
VDLIST_API void * VDLIST_CDECL vdlist_remove_first(
	vdlist_t* vdlist);

/*!
    \brief This function removes the last item from the vdlist structure.
	\var vdlist The list operate on
	\return 0 if vdlist is empty or first item pointer
*/
VDLIST_API void * VDLIST_CDECL vdlist_remove_last(
	vdlist_t* vdlist);

/*!
    \brief This function removes the next item after the cursor from the vdlist structure.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_remove_cursor_next(
	vdlist_t* vdlist);

/*!
    \brief This function removes the item before the cursor from the vdlist structure.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_remove_cursor_prev(
	vdlist_t* vdlist);

/*!
    \brief This function removes the item at the cursor from the vdlist structure. cursor become the next item on the list
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_remove_at_cursor(
	vdlist_t* vdlist);

/*!
    \brief This function moves the cursor to the front of the vdlist structure.
	\var vdlist The list operate on
*/
VDLIST_API void VDLIST_CDECL vdlist_reset_cursor(
	vdlist_t* vdlist);

/*!
    \brief This function moves the cursor to the back of the vdlist structure.
	\var vdlist The list operate on
*/
VDLIST_API void VDLIST_CDECL vdlist_reset_cursor_back(
	vdlist_t* vdlist);

/*!
    \brief This function returns item at the cursor but doesn't remove it.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_peek_cursor(
	vdlist_t* vdlist);

/*!
    \brief This function returns next item after the cursor but doesn't remove it.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_peek_cursor_next(
	vdlist_t* vdlist);

/*!
    \brief This function returns next item's next after the cursor but doesn't remove it.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_peek_cursor_next_next(
	vdlist_t* vdlist);

/*!
    \brief This function returns prev item after the cursor but doesn't remove it.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_peek_cursor_prev(
	vdlist_t* vdlist);

/*!
    \brief This function returns prev item's prev after the cursor but doesn't remove it.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_peek_cursor_prev_prev(
	vdlist_t* vdlist);

/*!
    \brief This function moves the cursor to the next postition in the vdlist structure.
	\var vdlist The list operate on
*/
VDLIST_API void VDLIST_CDECL vdlist_advance_cursor(
	vdlist_t* vdlist);

/*!
    \brief This function moves the cursor to the previous postition in the vdlist structure.
	\var vdlist The list operate on
*/
VDLIST_API void VDLIST_CDECL vdlist_rewind_cursor(
	vdlist_t* vdlist);

/*!
    \brief This function returns first item on the vdlist but doesn't remove it.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_peek_first(
	vdlist_t* vdlist);

/*!
    \brief This function returns last item on the vdlist but doesn't remove it.
	\var vdlist The list operate on
	\return 0 if no such item exists else the item
*/
VDLIST_API void * VDLIST_CDECL vdlist_peek_last(
	vdlist_t* vdlist);

/*!
    \brief This function returns number of items in the vdlist.
	\var vdlist The list operate on
	\return Number of items in the list.
*/
VDLIST_API long VDLIST_CDECL vdlist_count(
	vdlist_t* vdlist);

/*!
    \brief This function checks the existence of a item in the vdlist.
	\var vdlist The list operate on
	\var item A void pointer look for
	\return 0 if item not found ~0 if found
*/
VDLIST_API int VDLIST_CDECL vdlist_item_exists(
	vdlist_t* vdlist, 
	void* item);

/*!
    \brief This function writes the items in the vdlist into a array doesn't change the list .
	\var vdlist The list operate on
	\var items An array of void pointers the size of number of items in the list
*/
VDLIST_API void VDLIST_CDECL vdlist_items_to_array(
	vdlist_t* vdlist, 
	void** items);

/*!
    \brief This function calls a function once for each item in the vdlist. 
	\var vdlist The list operate on
	\var function A pointer to a function taking an item and user data.
	\var user_data A pointer passed with each function call. 
*/
VDLIST_API void VDLIST_CDECL vdlist_map_function(
	vdlist_t* vdlist, 
	void(*function)(void*, void*),
	void * user_data);

/*!
    \brief This function peeks at the nth item in the vdlist. 
	\var vdlist The list operate on
	\var index A 0 based index into the vdlist
	\return 0 if no such item exists else the item
*/
VDLIST_API void* VDLIST_CDECL vdlist_nth_item(
	vdlist_t* vdlist,
	long index);

/*!
    \brief This function reverses the items in the vdlist.
	\var vdlist The list operate on
*/
VDLIST_API void VDLIST_CDECL vdlist_reverse(
	vdlist_t* vdlist);

/*!
    \brief This function wipes out all items in the list
	\var vdlist The list operate on
*/
VDLIST_API void VDLIST_CDECL vdlist_flush(
	vdlist_t* vdlist);

/*!
    \brief This function sort items in the list 
	\var vlist The list operate on
	\var compare The a qsort style compare function
*/
VDLIST_API void VDLIST_CDECL vdlist_sort(
	vdlist_t* vlist,
	vcompare_t compare,
	void * data);

/*!
    \brief This function adds a item to a sorted list
	\var vlist The list operate on
	\var item A void pointer to store 
	\var compare The a qsort style compare function
*/
VDLIST_API void VDLIST_CDECL vdlist_add_sorted(
	vdlist_t* vlist,
	void* item,
	vcompare_t compare,
	void * data);

VDLIST_API vdlist_cursor_t * VDLIST_CDECL vdlist_new_unique_cursor(vdlist_t* vdlist);
VDLIST_API void VDLIST_CDECL vdlist_delete_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void VDLIST_CDECL vdlist_add_before_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc, void* item); 
VDLIST_API void VDLIST_CDECL vdlist_add_after_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc, void* item); 
VDLIST_API void VDLIST_CDECL vdlist_reset_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc); 
VDLIST_API void VDLIST_CDECL vdlist_reset_unique_cursor_back(vdlist_t* vdlist, vdlist_cursor_t* uc); 
VDLIST_API void * VDLIST_CDECL vdlist_peek_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void * VDLIST_CDECL vdlist_peek_unique_cursor_next(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void * VDLIST_CDECL vdlist_peek_unique_cursor_next_next(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void * VDLIST_CDECL vdlist_peek_unique_cursor_prev(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void * VDLIST_CDECL vdlist_peek_unique_cursor_prev_prev(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void * VDLIST_CDECL vdlist_remove_unique_cursor_next(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void * VDLIST_CDECL vdlist_remove_unique_cursor_prev(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void * VDLIST_CDECL vdlist_remove_at_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void VDLIST_CDECL vdlist_advance_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc);
VDLIST_API void VDLIST_CDECL vdlist_rewind_unique_cursor(vdlist_t* vdlist, vdlist_cursor_t* uc);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


#ifndef V_POINTER_SIZED_INT
#ifdef _WIN64
#define  V_POINTER_SIZED_INT    unsigned __int64
#else
#define  V_POINTER_SIZED_INT    unsigned long
#endif
#endif



/* these are used for the internal functions */
#define VDLIST_RESET_CURSOR(_XL_, _uc_) \
	(_uc_)->node = (_XL_)->head; \
	if(!(_uc_)->node) { \
		(_uc_)->state = VDLIST_BEGIN; \
		(_uc_)->index=-1; \
	} \
	else { \
		(_uc_)->state = VDLIST_ON_LIST; \
		(_uc_)->index=0; \
	}

#define VDLIST_RESET_CURSOR_BACK(_XL_, _uc_) \
	(_uc_)->node = (_XL_)->tail; \
	(_uc_)->index = (_XL_)->count - 1; \
	if(!(_uc_)->node) \
		(_uc_)->state = VDLIST_BEGIN; \
	else \
		(_uc_)->state = VDLIST_ON_LIST; \

#define VDLIST_PEEK_CURSOR(_uc_) \
	((!(_uc_)->node)?0:(_uc_)->node->item)

#define	VDLIST_ADVANCE_CURSOR(_XL_, _uc_) \
	if((_uc_)->state == VDLIST_ON_LIST) { \
		(_uc_)->index++; \
		(_uc_)->node = (_uc_)->node->next; \
		if(!(_uc_)->node) \
			(_uc_)->state = VDLIST_END; \
	} \
	else if ((_uc_)->state == VDLIST_BEGIN) { \
		VDLIST_RESET_CURSOR(_XL_, _uc_) \
	}

#define	VDLIST_REWIND_CURSOR(_XL_, _uc_) \
	if((_uc_)->state == VDLIST_ON_LIST) { \
		(_uc_)->index--; \
		(_uc_)->node = (_uc_)->node->prev; \
		if(!(_uc_)->node) \
			(_uc_)->state = VDLIST_BEGIN; \
	} \
	else if ((_uc_)->state == VDLIST_END) { \
		VDLIST_RESET_CURSOR_BACK(_XL_, _uc_) \
	}



#ifdef _MSC_VER
#	pragma warning (disable: 4127)	/* conditional expression is constant */
#endif



/* template wrapper */
#ifdef __cplusplus
extern "C++" {

/*the following macro generates a compile-time errors
  if anyone tries to instantiate a VDList with incompatible types */
#define VDLIST_COMPILE_TIME_ASSERT(expr) { char unnamed[(expr)?1:0]; (void)unnamed; }

template <class T> class VDList 
{
public:

	VDList(struct vdlist_s * vl) { m_vdlist = vl; size_assert(); };

	VDList() { m_vdlist = new_vdlist(malloc,free); size_assert(); };
	
	VDList(void * (VDLIST_CDECL *  vdlist_malloc) (size_t), void (VDLIST_CDECL *vdlist_free) (void *)) 
		{m_vdlist = new_vdlist( vdlist_malloc,vdlist_free); size_assert(); };

	~VDList() {
		delete_vdlist(m_vdlist);
	};

	void AddFirst(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vdlist_add_first(m_vdlist, x); 
	};

	void AddLast(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vdlist_add_last(m_vdlist, x);
	};

	void AddBeforeCursor(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vdlist_add_before_cursor(m_vdlist, x); 
	};

	void AddAfterCursor(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vdlist_add_after_cursor(m_vdlist, x); 
	};

	int Remove(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		return (vdlist_remove(m_vdlist, x)); 
	};

	T RemoveFirst() { 
		void *x = vdlist_remove_first(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

 	void ResetCursor() { vdlist_reset_cursor(m_vdlist); };

	void ResetCursorBack() { vdlist_reset_cursor_back(m_vdlist); };

 	T PeekCursor() { 
		void *x = vdlist_peek_cursor(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

 	T PeekCursorNext() { 
		void *x = vdlist_peek_cursor_next(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	T PeekCursorPrev() { 
		void *x = vdlist_peek_cursor_prev(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	T RemoveAtCursor() { 
		void *x = vdlist_remove_at_cursor(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	T RemoveCursorNext() { 
		void *x = vdlist_remove_cursor_next(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	T PeekCursorNextNext() { 
		void *x = vdlist_peek_cursor_next_next(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};
	T PeekCursorPrevPrev() { 
		void *x = vdlist_peek_cursor_prev_prev(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	void AdvanceCursor() { vdlist_advance_cursor(m_vdlist); };

	void RewindCursor()	{ vdlist_rewind_cursor(m_vdlist); };

	T PeekFirst() { 
		void *x = vdlist_peek_first(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

 	T PeekLast() { 
		void *x = vdlist_peek_last(m_vdlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y;  
	};

 	long Count() { return (vdlist_count(m_vdlist)); };

	int ItemExists(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		return (vdlist_item_exists(m_vdlist, x)); 
	};

	void ItemsToArray(T* items) {
		if (sizeof(T) != sizeof(V_POINTER_SIZED_INT)) {
			V_POINTER_SIZED_INT *tmp=new V_POINTER_SIZED_INT[m_vdlist->count];
			vdlist_items_to_array(m_vdlist, (void **)tmp);
			for(long i=0; i<m_vdlist->count; i++)
				memcpy(&items[i], &tmp[i], sizeof(T));
			delete[] tmp;
		}
		else
			vdlist_items_to_array(m_vdlist, (void **)items);
	};

	void MapFunction(void(*function)(T, void*), void const * const user_data=0) {
		long i = Count();
		ResetCursor();
		while(i){
			(*function)(PeekCursor(),(void*)user_data);
			AdvanceCursor();
			i--;
		}
	};

	void DeleteItems() {
		long i = Count();
		ResetCursor();
		while(i){
			delete PeekCursor();
			AdvanceCursor();
			i--;
		}
	};

	T NthItem(long index) { 
		void *x = vdlist_nth_item(m_vdlist, index);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y;  
	};

	typedef  int(*SortFunctionType)(T, T, void const * const data);
	
	void Sort(SortFunctionType compare,  void const * const data=0) { 
		SortFunctionData sfd(compare, data);
		vdlist_sort(m_vdlist, sort_helper, (void*)&sfd);
	};


	void AddSorted(T item, SortFunctionType compare,  void const * const data=0) { 
		SortFunctionData sfd(compare, data);
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vdlist_add_sorted(m_vdlist, x, sort_helper, (void*)&sfd);
	};
	
	void Reverse() { vdlist_reverse(m_vdlist); };
	
	void Flush() { vdlist_flush(m_vdlist); };

private:
	struct vdlist_s *m_vdlist;
	
	class SortFunctionData {
		public:	
		SortFunctionData(SortFunctionType function=0, void const * const data=0) {
			m_function=function;
			m_data=(void*)data;
		};
		SortFunctionType m_function;
		void * m_data;
	};

	static int sort_helper(const void* in_a, const void* in_b, const void * in_data) {
		SortFunctionData *data = (SortFunctionData *)in_data;
		T a=0;
		T b=0;
		memcpy(&a, &in_a, sizeof(T));
		memcpy(&b, &in_b, sizeof(T));
		return data->m_function(a,b,data->m_data);
	};

	/* VDList cannot hold any type that cannot be converted to and from a (void *) */
	void size_assert() { VDLIST_COMPILE_TIME_ASSERT(sizeof(T) <= sizeof(void *)); };

};
} /* extern "C++" */
#endif





#endif /*VDLIST_H*/



