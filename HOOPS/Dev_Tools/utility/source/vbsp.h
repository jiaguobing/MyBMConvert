// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef VBSP_H
#define VBSP_H

#include <stdint.h>
#include <assert.h>

#include "vlist.h"
#include "vhash.h"

enum VBSPStatus {
	VBSP_STATUS_STOP		= 0,
	VBSP_STATUS_CONTINUE	= 1,
	VBSP_STATUS_SOMETHING	= 2,
};
	
enum PartitionType {
	BSP_NONE	= 0x0,
	BSPX 		= 0x1,
	BSPY 		= 0x2,
	BSPZ 		= 0x4,
	BSP_GARBAGE	= 0x8
};

enum TreeTraversal {
	ROOT_ENTER				= 0,
	ROOT_EXIT				= 1,
	LEFT_BOTTOM_FRONT_ENTER	= 2,
	LEFT_BOTTOM_FRONT_EXIT	= 3,
	RIGHT_TOP_BACK_ENTER	= 4,
	RIGHT_TOP_BACK_EXIT		= 5,
	OVERLAP_ENTER			= 6,
	OVERLAP_EXIT			= 7
};

//#define VBSP_MEM_LOGGING 1
#define ITERATIVE_VBSP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

template <typename T> class BSPItem {
public:
	BSPItem () {};
	BSPItem ( T item_in, float const item_bounds_in[6] = 0 ) {};
	
	virtual ~BSPItem () {};

	virtual T GetItem () = 0;
	virtual float const * GetItemBounds () = 0;
};

template <typename T> class DefaultBSPItem : public BSPItem<T> {
public:
	DefaultBSPItem () {
		m_free_bounds = false;
		m_item_bounds = 0;
	};
	
	DefaultBSPItem ( T item_in, float const item_bounds_in[6] = 0 ) {
		m_item = item_in;
		m_free_bounds = false;
		m_item_bounds = 0;
		
		if (item_bounds_in == reinterpret_cast<float *>(item_in))
			m_item_bounds = reinterpret_cast<float *>(m_item);
		else if (item_bounds_in) {
			m_free_bounds = true;
			m_item_bounds = new float [6];
			memcpy(m_item_bounds, item_bounds_in, 6 * sizeof(float));	
		}
	};
	
	virtual ~DefaultBSPItem () {
		if (m_free_bounds)
			delete [] m_item_bounds;
	};
	
	virtual T GetItem () { return m_item; };
	
	virtual float const * GetItemBounds () { return m_item_bounds; };
	
protected:	
	bool		m_free_bounds;
	float *		m_item_bounds;
	T			m_item;
};

// overload this class to handle the storing of the items in the BSP tree
template <typename T> class BSPNodeItemContainer {
public:
	BSPNodeItemContainer () {};
	BSPNodeItemContainer ( float const bbox_in[6], int depth_in, void * container_data_in ) {};
	
	virtual ~BSPNodeItemContainer () {};
	
	// called whenever an item should be added to a container
	virtual bool AddItem ( T item_in, float const item_bounds_in[6] = 0 ) = 0;
	// returns the number of items stored in the container
	virtual int Count () = 0;
	
	class iterator {
	public:
		iterator () {};
		virtual ~iterator () {};
		
		// pre-increment
		iterator operator ++ ();
		// post-increment
		iterator operator ++ ( int );
		bool operator == ( iterator const & rhs );
		bool operator != ( iterator const & rhs );
		virtual BSPItem<T> * operator * () { return 0; };
	};
	
	iterator begin ();
	iterator end ();
};

template <typename T> class DefaultBSPNodeItemContainer : public BSPNodeItemContainer<T> {
public:
	DefaultBSPNodeItemContainer ( float const bbox_in[6], int depth_in, void * container_data_in ) {
		(void) depth_in;
		memcpy(m_bbox, bbox_in, 6 * sizeof(float));
		m_container_data = container_data_in;
		m_items_allocated = 4;
		m_items = new BSPItem<T> * [m_items_allocated];
		m_items_used = 0;
	};
	
	virtual ~DefaultBSPNodeItemContainer () {
		if (m_items) {
			for ( int i = 0 ; i < m_items_used ; i++ )
				delete m_items[i];
		
			delete [] m_items;
		}
	};
	
	virtual bool AddItem ( T item_in, float const item_bounds_in[6] = 0 ) {
		if (m_items_used == m_items_allocated) {
			m_items_allocated = (int)((float)m_items_allocated * 1.5 + 4.0);
			
			BSPItem<T> **temp;
			temp = new BSPItem<T> * [m_items_allocated];

			if (m_items) {
				memcpy(temp, m_items, m_items_used * sizeof(BSPItem<T> *));

				delete [] m_items;
			}
			m_items = temp;
		}
	
		m_items[m_items_used++] = new DefaultBSPItem<T>(item_in, item_bounds_in);

		return true;
	};

	virtual int Count () {
		return m_items_used;
	};
	
	class iterator : public BSPNodeItemContainer<T>::iterator {
	public:
		iterator () {
			m_item_list = 0;
		};
		
		iterator ( BSPItem<T> ** item_list_in ) {
			m_item_list = item_list_in;
		};
		
		virtual ~iterator () {};
		
		iterator operator ++ () {
			++m_item_list;
			return *this;
		};
		
		iterator operator ++ ( int ) {
			iterator tmp(m_item_list);
			++m_item_list;
			return tmp;
		};
		
		bool operator == ( iterator const & rhs ) {
			if (m_item_list == rhs.m_item_list)
				return true;

			return false;
		};
		
		bool operator != ( iterator const & rhs ) {
			return !(*this == rhs);
		};
		
		virtual BSPItem<T> * operator * () {
			return *m_item_list;
		};
		
	protected:
		BSPItem<T> ** m_item_list;
	};
	
	iterator begin () {
		return iterator(m_items);
	};
	
	iterator end () {
		return iterator(m_items + m_items_used);
	};
	
protected:
	float			m_bbox[6];
	void *			m_container_data;
	int 			m_items_allocated;
	int 			m_items_used;
	BSPItem<T> ** 	m_items;
};

#include <string.h>
#include "cache_memory_object.h"

// If ItemContainer is specified, it should inherit from BSPNodeItemContainer:
//   - either as a class template, e.g., 
//	       template <typename T> class DefaultBSPNodeItemContainer : BSPNodeItemContainer<T> { . . . }
//   - or as a regular class for a specific type, e.g., 
//		   class SomeItemContainer : BSPNodeItemContainer<SomeActualClass> { . . . }

// forward declaration
template <typename T, class ItemContainer> class VBSP;
template <typename T, class ItemContainer> class BSPNodeMetadata;
	
template <typename T, class ItemContainer> class BSPNode : public CacheMemoryObject {
	template <typename T2, class ItemContainer2> friend class BSPNodeMetadata;
	template <typename T2, class ItemContainer2> friend class VBSP;

public:
	BSPNode ( VBSP<T, ItemContainer> * tree_in, const float bounds_in[6], int max_depth_in, PartitionType partition_type_in, int auto_depth_in, int depth_in ) {
		m_tree = tree_in;
		m_parent = 0;
		m_tree->m_node_count++;
		memcpy(m_bounds, bounds_in, 6 * sizeof(float));
		m_max_depth = max_depth_in;
		m_auto_depth = auto_depth_in;
		m_partition_type = partition_type_in;
		m_remaining_dimensions = BSPX | BSPY | BSPZ;
		
		m_dim_min_offset = m_dim_max_offset = -1;
		switch (m_partition_type) {
			case BSPX: {
				m_dim_max_offset = 3;
				m_dim_min_offset = 0;
			} break;
			case BSPY: {
				m_dim_max_offset = 4;
				m_dim_min_offset = 1;
			} break;
			case BSPZ: {
				m_dim_max_offset = 5;
				m_dim_min_offset = 2;
			} break;
			default:
				assert(0);
		}

		m_cutoffs[0] = m_bounds[m_dim_min_offset] + m_tree->GetLowCutoff() * (m_bounds[m_dim_max_offset] - m_bounds[m_dim_min_offset]);
		m_cutoffs[1] = m_bounds[m_dim_min_offset] + m_tree->GetHighCutoff() * (m_bounds[m_dim_max_offset] - m_bounds[m_dim_min_offset]);

		m_depth = depth_in;

		if (m_depth > m_tree->m_current_depth)
			m_tree->m_current_depth = m_depth;
		
		m_contents_used = 0;
		m_contents = 0;

		m_unsorted_used = 0;
		m_unsorted = 0;
		if (m_auto_depth == 0)
			m_unsorted = new ItemContainer(m_bounds, m_depth, m_tree->GetContainerData());
		
		m_num_items_seen = 0;
		m_need_to_count_item = true;

		m_kids[0] = m_kids[1] = m_kids[2] = 0; // NULL
		
		m_collision = false;
	};
	
	~BSPNode () {
		if (m_contents) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE contents\n", m_contents, this);
			m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
			delete m_contents;
		}
		if (m_unsorted) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE unsorted\n", m_unsorted, this);
			m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
			delete m_unsorted;
		}
		if (m_kids[0]) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE kids[0]\n", m_kids[0], this);
			m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
			//operator delete (kids[0], sizeof(BSPNode<T>));
			delete m_kids[0];
		}
		if (m_kids[1]) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE kids[1]\n", m_kids[1], this);
			m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
			//operator delete (kids[1], sizeof(BSPNode<T>));
			delete m_kids[1];
		}
		if (m_kids[2]) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE kids[2]\n", m_kids[2], this);
			m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
			//operator delete (kids[2], sizeof(BSPNode<T>));
			delete m_kids[2];
		}
	};
	
	void AppendContents ( T data ) {
		if (!m_contents)
			m_contents = new ItemContainer(m_bounds, m_depth, m_tree->GetContainerData());
		
		bool appended_item = m_contents->AddItem(data);

		if (!appended_item) {
			BSPNode<T, ItemContainer> *i = this;
			while (i) {
				i->m_num_items_seen--;
				i = i->m_parent;
			}

			m_tree->m_culled_count++;
		}
		else
			m_contents_used++;
	};
	
	bool AppendUnsorted ( T item, float const item_bounds[6] ) {
		if (m_unsorted_used >= m_tree->GetUnsortedLimit())
			return false;
		
		bool appended_item = m_unsorted->AddItem(item, item_bounds);

		if (!appended_item) {
			BSPNode<T, ItemContainer> *i = this;
			while (i) {
				i->m_num_items_seen--;
				i = i->m_parent;
			}

			m_tree->m_culled_count++;
		}
		else
			m_unsorted_used++;

		return true;	
	};
	
	void SortUnsorted () {
		ItemContainer *unsorted_copy = m_unsorted;
		m_unsorted = 0; // NULL - avoid stack overflow
		m_need_to_count_item = false;

		typename ItemContainer::iterator i;
		for ( i = unsorted_copy->begin() ; i != unsorted_copy->end() ; i++ )
			InsertRecursive((*i)->GetItem(), (*i)->GetItemBounds());

		delete unsorted_copy;

		m_unsorted_used = 0;
		m_need_to_count_item = true;
	};
	
	PartitionType GetNextDimension () {
		if (m_remaining_dimensions == BSP_NONE)
			return BSP_NONE;
			
		PartitionType next_dimension = m_partition_type;
		do {
			int next_dimension_int = next_dimension;
			next_dimension_int <<= 1;
			next_dimension = (PartitionType)next_dimension_int;
			if (next_dimension == BSP_GARBAGE)
				next_dimension = BSPX;
		} while (!(next_dimension & m_remaining_dimensions));
		
		return next_dimension;
	};
	
	BSPNode<T, ItemContainer> * InsertRecursive ( T item, float const item_bounds[6] ) {
#ifdef ITERATIVE_VBSP
		BSPNode<T, ItemContainer> *i_node = this;
	
		while (1) {
			if (i_node->m_need_to_count_item)
				i_node->m_num_items_seen++;
	
			if (i_node->m_max_depth == 0) {
				i_node->AppendContents(item);
				return i_node;
			}
			if (i_node->m_unsorted) {
				if (i_node->AppendUnsorted(item, item_bounds)) {
						
					return i_node;
				}
				i_node->SortUnsorted();
			}
	
			float bounds_save;
	
			if (item_bounds[i_node->m_dim_max_offset] < i_node->m_cutoffs[1]) {
				/* fits on the left/bottom/front */
				bounds_save = i_node->m_bounds[i_node->m_dim_max_offset];
				i_node->m_bounds[i_node->m_dim_max_offset] = i_node->m_cutoffs[1];
				
				if (i_node->m_kids[0] == 0) {
					i_node->m_kids[0] = new BSPNode<T, ItemContainer>(i_node->m_tree, i_node->m_bounds, 
						(i_node->m_max_depth == -1 ? i_node->m_max_depth : i_node->m_max_depth - 1), i_node->GetNextDimension(),
						(i_node->m_auto_depth == 0 ? 0 : i_node->m_auto_depth - 1), i_node->m_depth + 1);
					i_node->m_kids[0]->m_parent = i_node;
#ifdef VBSP_MEM_LOGGING
					char str[4096];
					sprintf(str, "+%x\t%x\tNEW kids[0]\n", i_node->m_kids[0], i_node);
					i_node->m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
				}
				
				i_node->m_bounds[i_node->m_dim_max_offset] = bounds_save;
				i_node = i_node->m_kids[0];
			}
			else if (item_bounds[i_node->m_dim_min_offset] > i_node->m_cutoffs[0]) {
				/* fits on the right/top/back */
				bounds_save = i_node->m_bounds[i_node->m_dim_min_offset];
				i_node->m_bounds[i_node->m_dim_min_offset] = i_node->m_cutoffs[0];
				
				if (i_node->m_kids[1] == 0) {
					i_node->m_kids[1] = new BSPNode<T, ItemContainer>(i_node->m_tree, i_node->m_bounds, 
						(i_node->m_max_depth == -1 ? i_node->m_max_depth : i_node->m_max_depth - 1), i_node->GetNextDimension(),
						(i_node->m_auto_depth == 0 ? 0 : i_node->m_auto_depth - 1), i_node->m_depth + 1);
					i_node->m_kids[1]->m_parent = i_node;
#ifdef VBSP_MEM_LOGGING
					char str[4096];
					sprintf(str, "+%x\t%x\tNEW kids[1]\n", i_node->m_kids[1], i_node);
					i_node->m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
				}
				
				i_node->m_bounds[i_node->m_dim_min_offset] = bounds_save;
				i_node = i_node->m_kids[1];
			}
			else {
				if (!i_node->m_tree->GetSortLosers()) { 
					i_node->AppendContents(item);
					return i_node;
				}
				else {
					i_node->m_remaining_dimensions &= ~i_node->m_partition_type;
					if (i_node->m_remaining_dimensions == BSP_NONE) {
						/* item_bounds crosses the entire overlap region, as well as the overlap regions
						 * of all other dimensions.	 it belongs at this level */
						i_node->AppendContents(item);
						return i_node;
					}
					else {
						if (i_node->m_kids[2] == 0) {
							i_node->m_kids[2] = new BSPNode<T, ItemContainer>(i_node->m_tree, i_node->m_bounds, 
								(i_node->m_max_depth == -1 ? i_node->m_max_depth : i_node->m_max_depth - 1), i_node->GetNextDimension(),
								(i_node->m_auto_depth == 0 ? 0 : i_node->m_auto_depth - 1), i_node->m_depth + 1);
							i_node->m_kids[2]->m_parent = i_node;
#ifdef VBSP_MEM_LOGGING
							char str[4096];
							sprintf(str, "+%x\t%x\tNEW kids[2]\n", i_node->m_kids[2], this);
							i_node->m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
						}
	
						i_node = i_node->m_kids[2];
					}
				}
			}
		}
#else
		if (m_need_to_count_item)
			m_num_items_seen++;

		if (m_max_depth == 0) {
			AppendContents(item);
			return this;
		}
		if (m_unsorted) {
			if (AppendUnsorted(item, item_bounds)) {

				return this;
			}
			SortUnsorted();
		}

		float bounds_save;

		if (item_bounds[m_dim_max_offset] < m_cutoffs[1]) {
			/* fits on the left/bottom/front */
			bounds_save = m_bounds[m_dim_max_offset];
			m_bounds[m_dim_max_offset] = m_cutoffs[1];

			if (m_kids[0] == 0) {
				m_kids[0] = new BSPNode<T, ItemContainer>(m_tree, m_bounds, (m_max_depth == -1 ? m_max_depth : m_max_depth - 1), GetNextDimension());
#ifdef VBSP_MEM_LOGGING
				char str[4096];
				sprintf(str, "+%x\t%x\tNEW kids[0]\n", m_kids[0], this);
				m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
			}

			m_kids[0]->InsertRecursive(item, item_bounds);
			m_bounds[m_dim_max_offset] = bounds_save;
		}
		else if (item_bounds[m_dim_min_offset] > m_cutoffs[0]) {
			/* fits on the right/top/back */
			bounds_save = m_bounds[m_dim_min_offset];
			m_bounds[m_dim_min_offset] = m_cutoffs[0];

			if (m_kids[1] == 0) {
				m_kids[1] = new BSPNode<T, ItemContainer>(m_tree, m_bounds, (m_max_depth == -1 ? m_max_depth : m_max_depth - 1), GetNextDimension());
#ifdef VBSP_MEM_LOGGING
				char str[4096];
				sprintf(str, "+%x\t%x\tNEW kids[1]\n", m_kids[1], this);
				m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
			}

			m_kids[1]->InsertRecursive(item, item_bounds);
			m_bounds[m_dim_min_offset] = bounds_save;
		}
		else {
			if (!m_tree->GetSortLosers()) {
				AppendContents(item);
				return this;
			}
			else {
				m_remaining_dimensions &= ~m_partition_type;
				if (m_remaining_dimensions == BSP_NONE) {
					/* item_bounds crosses the entire overlap region, as well as the overlap regions
					* of all other dimensions.	 it belongs at this level */
					AppendContents(item);
					return this;
				}
				else {
					if (m_kids[2] == 0) {
						m_kids[2] = new BSPNode<T, ItemContainer>(m_tree, m_bounds, (m_max_depth == -1 ? m_max_depth : m_max_depth - 1), GetNextDimension());
#ifdef VBSP_MEM_LOGGING
						char str[4096];
						sprintf(str, "+%x\t%x\tNEW kids[2]\n", m_kids[2], this);
						m_tree->Write(str);
#endif // VBSP_MEM_LOGGING
					}

					m_kids[2]->InsertRecursive(item, item_bounds);
				}
			}
		}
#endif
	};
	
	VBSPStatus TestRecursive ( VBSPStatus (*function) (T, T, void *), T part, float const part_bounds[6], void * user_data ) {
		typename ItemContainer::iterator i;
		
		for ( i = m_contents->begin() ; i != m_contents->end() ; i++ ) {
			if (function ((*i)->m_item, part, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}
		
		for ( i = m_unsorted->begin() ; i != m_unsorted->end() ; i++ ) {
			if (function ((*i)->m_item, part, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}
		
		// not sure what should be done here or if this should ever happen
		if (m_partition_type != BSPX && m_partition_type != BSPY && m_partition_type != BSPZ) 
			return VBSP_STATUS_STOP;
		
		if (m_kids[0] && (part_bounds[m_dim_min_offset] <= m_kids[0]->m_bounds[m_dim_max_offset]))
			if (m_kids[0]->TestRecursive(function, part, part_bounds, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[1] && (part_bounds[m_dim_max_offset] >= m_kids[1]->m_bounds[m_dim_min_offset]))
			if (m_kids[1]->TestRecursive(function, part, part_bounds, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[2])
			if (m_kids[2]->TestRecursive(function, part, part_bounds, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		return VBSP_STATUS_CONTINUE;
	};

	VBSPStatus MapFunctionRecursive ( VBSPStatus (*function) (T, void *), void * user_data ) {
		typename ItemContainer::iterator i;

		if (m_contents_used) {
			for ( i = m_contents->begin() ; i != m_contents->end() ; i++ ) {
				if (function ((*i)->GetItem(), user_data) == VBSP_STATUS_STOP)
					return VBSP_STATUS_STOP;
			}
		}

		if (m_unsorted_used) {
			for ( i = m_unsorted->begin() ; i != m_unsorted->end() ; i++ ) {
				if (function ((*i)->GetItem(), user_data) == VBSP_STATUS_STOP)
					return VBSP_STATUS_STOP;
			}
		}
	
		if (m_kids[0])
			if (m_kids[0]->MapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[1])
			if (m_kids[1]->MapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[2])
			if (m_kids[2]->MapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		return VBSP_STATUS_CONTINUE;
	};

	VBSPStatus PerNodeMapFunctionRecursive ( VBSPStatus (*function) (ItemContainer *, void *), void * user_data ) {
		if (m_unsorted_used) {
			if (function(m_unsorted, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}
		else if (m_contents_used) {
			if (function(m_contents, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		}

		if (m_kids[0])
			if (m_kids[0]->PerNodeMapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[1])
			if (m_kids[1]->PerNodeMapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[2])
			if (m_kids[2]->PerNodeMapFunctionRecursive(function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;

		return VBSP_STATUS_CONTINUE;
	};
	
	VBSPStatus PerNodeMapFunctionRecursive ( TreeTraversal tree_pos, VBSPStatus (*function) (TreeTraversal, ItemContainer *, BSPNodeMetadata<T, ItemContainer> *, void *), void * user_data ) {
#ifdef ITERATIVE_VBSP
		VList<BSPNode<T, ItemContainer> *> node_list;
		VList<int> position_list;
		VHash<BSPNode<T, ItemContainer> *, int> recurse_hash;

		node_list.AddFirst(this);
		position_list.AddFirst(tree_pos);

		while (1) {
			if (node_list.Count() == 0) {
				assert(recurse_hash.Count() == 0);
				assert(position_list.Count() == 0);
				break;
			}

			BSPNode<T, ItemContainer> *i_node = node_list.PeekFirst();
			ItemContainer *container = (i_node->m_unsorted_used ? i_node->m_unsorted : (i_node->m_contents_used ? i_node->m_contents : 0));
			BSPNodeMetadata<T, ItemContainer> node_metadata(i_node);

			if (recurse_hash.LookupItem(i_node) == VHASH_STATUS_FAILED) {
				// call function on entry
				if (function((TreeTraversal)position_list.PeekFirst(), container, &node_metadata, user_data) == VBSP_STATUS_STOP)
					return VBSP_STATUS_STOP;

				if (i_node->m_kids[2]) {
					node_list.AddFirst(i_node->m_kids[2]);
					position_list.AddFirst(OVERLAP_ENTER);
				}
				if (i_node->m_kids[1]) {
					node_list.AddFirst(i_node->m_kids[1]);
					position_list.AddFirst(RIGHT_TOP_BACK_ENTER);
				}
				if (i_node->m_kids[0]) {
					node_list.AddFirst(i_node->m_kids[0]);
					position_list.AddFirst(LEFT_BOTTOM_FRONT_ENTER);
				}

				recurse_hash.InsertItem(i_node, 0);
			}
			else {
				// call function on exit
				if (function((TreeTraversal)(position_list.RemoveFirst() + 1), container, &node_metadata, user_data) == VBSP_STATUS_STOP)
					return VBSP_STATUS_STOP;
			
				node_list.RemoveFirst();
				recurse_hash.RemoveItem(i_node);
			}
		}

		return VBSP_STATUS_CONTINUE;
#else
		// call function on entry
		ItemContainer *container = (m_unsorted_used ? m_unsorted : (m_contents_used ? m_contents : 0));
		BSPNodeMetadata<T, ItemContainer> node_metadata(this);
		if (function(tree_pos, container, &node_metadata, user_data) == VBSP_STATUS_STOP)
			return VBSP_STATUS_STOP;

		if (m_kids[0])
			if (m_kids[0]->PerNodeMapFunctionRecursive(LEFT_BOTTOM_FRONT_ENTER, function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[1])
			if (m_kids[1]->PerNodeMapFunctionRecursive(RIGHT_TOP_BACK_ENTER, function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;
		if (m_kids[2])
			if (m_kids[2]->PerNodeMapFunctionRecursive(OVERLAP_ENTER, function, user_data) == VBSP_STATUS_STOP)
				return VBSP_STATUS_STOP;

		// call function on exit
		if (function((TreeTraversal)(tree_pos + 1), container, &node_metadata, user_data) == VBSP_STATUS_STOP)
			return VBSP_STATUS_STOP;

		return VBSP_STATUS_CONTINUE;
#endif
	};
	
protected:
	
	VBSP<T, ItemContainer> *		m_tree;
	BSPNode<T, ItemContainer> *		m_parent;
	float 							m_bounds[6];
	float							m_cutoffs[2];
	int 							m_max_depth;
	int								m_auto_depth;
	PartitionType 					m_partition_type;
	int 							m_dim_min_offset;
	int 							m_dim_max_offset;
	                        		
	int 							m_remaining_dimensions;
	                        		
	int 							m_contents_used;
	ItemContainer *					m_contents;
	                        		
	int 							m_unsorted_used;
	ItemContainer *					m_unsorted;

	uint64_t						m_num_items_seen;
	bool							m_need_to_count_item;
	
	BSPNode<T, ItemContainer> *		m_kids[3];
	
	bool 							m_collision;

	int								m_depth;
};

template <typename T, class ItemContainer> class BSPNodeMetadata {
public:
	BSPNodeMetadata ( BSPNode<T, ItemContainer> * original_node_in ) {
		m_original_node = original_node_in;
	};

	~BSPNodeMetadata () {};

	uint64_t GetNumItemsInSubtree () const { return m_original_node->m_num_items_seen; };
	uint64_t GetNumItemsInLeftBottomFrontSubtree () const { return (m_original_node->m_kids[0] ? m_original_node->m_kids[0]->m_num_items_seen : 0); };
	uint64_t GetNumItemsInRightTopBackSubtree () const { return (m_original_node->m_kids[1] ? m_original_node->m_kids[1]->m_num_items_seen : 0); };
	uint64_t GetNumItemsInOverlapSubtree () const { return (m_original_node->m_kids[2] ? m_original_node->m_kids[2]->m_num_items_seen : 0); };
	int GetDepth () const { return m_original_node->m_depth; };

	float const * GetBounds () const { return m_original_node->m_bounds; };

private:
	BSPNode<T, ItemContainer> * m_original_node;
};

/*
	for all bounding boxes represented as float arrays, the convention is:
		min_x, min_y, min_z, max_x, max_y, max_z
 */

template <typename T, class ItemContainer = DefaultBSPNodeItemContainer<T> > class VBSP {
	template <typename T2, class ItemContainer2> friend class BSPNode;

public:
	
	/*	- max_depth of -1 means there is no maximum depth - just keep creating nodes as needed
		- defaults represent existing defaults (defines) from hvbsp.cpp in 3dGS
	 */
	VBSP ( float const bbox_in[6], int max_depth_in, int unsorted_limit_in = 10, bool sort_losers_in = true, 
		   float low_cutoff_in = 0.40f, float high_cutoff_in = 0.60f, int auto_depth_in = 0, void * container_data_in = 0 ) {
		memcpy(m_bbox, bbox_in, 6 * sizeof(float));
		m_max_depth = max_depth_in;
		m_unsorted_limit = unsorted_limit_in;
		m_sort_losers = sort_losers_in;
		m_low_cutoff = low_cutoff_in;
		m_high_cutoff = high_cutoff_in;
		m_container_data = container_data_in;
		m_node_count = 0;
		m_last_insertion_point = 0;
		m_current_depth = 0;
		m_culled_count = 0;
		m_root = new BSPNode<T, ItemContainer>(this, m_bbox, m_max_depth, BSPX, auto_depth_in, 0);		

#ifdef VBSP_MEM_LOGGING
		char str[4096];
		sprintf(str, "+%x\t%x\tNEW root BSPNode\n", root, this);
		Write(str);
#endif // VBSP_MEM_LOGGING
	};
	
	~VBSP () {
		if (m_root) {
#ifdef VBSP_MEM_LOGGING
			char str[4096];
			sprintf(str, "-%x\t%x\tDELETE root BSPNode\n", root, this);
			Write(str);
#endif // MEM_LOGGIING	
			delete m_root;		
		}
	};
	
	/* 
		Extract the first 24 bytes of the item being inserted into the tree are six floats (arranged in the
		conventional bbox fashion) which represent the bounding box to be used for that item.
	 */
	void Insert ( T item ) {
		float const * item_bounds = (float const *)item;
		Insert(item, item_bounds);
	};

	void Insert ( T item, float const item_bounds[6] ) {
		assert(item_bounds);

		if (m_last_insertion_point &&
			m_last_insertion_point->m_bounds[0] < item_bounds[0] && m_last_insertion_point->m_bounds[3] > item_bounds[3] &&
			m_last_insertion_point->m_bounds[1] < item_bounds[1] && m_last_insertion_point->m_bounds[4] > item_bounds[4] &&
			m_last_insertion_point->m_bounds[2] < item_bounds[2] && m_last_insertion_point->m_bounds[5] > item_bounds[5]) 
		{
			BSPNode<T, ItemContainer> *i = m_last_insertion_point->m_parent;
			while (i) {
				i->m_num_items_seen++;
				i = i->m_parent;
			}

			m_last_insertion_point->InsertRecursive(item, item_bounds);

			return;
		}
		
		m_last_insertion_point = m_root->InsertRecursive(item, item_bounds);
	};


	VBSPStatus Test ( VBSPStatus (*function) (T, T, void *), T part, float const part_bounds[6], void * user_data ) {
		return m_root->TestRecursive(function, part, part_bounds, user_data);
	};

	VBSPStatus MapFunction ( VBSPStatus (*function) (T, void *), void * user_data ) {
		return m_root->MapFunctionRecursive(function, user_data);
	};

	VBSPStatus PerNodeMapFunction ( VBSPStatus (*function) (ItemContainer *, void *), void * user_data ) {
		return m_root->PerNodeMapFunctionRecursive(function, user_data);
	};
	 
	VBSPStatus PerNodeMapFunction ( VBSPStatus (*function) (TreeTraversal, ItemContainer *, BSPNodeMetadata<T, ItemContainer> *, void *), void * user_data ) {
		return m_root->PerNodeMapFunctionRecursive(ROOT_ENTER, function, user_data);
	};
	
	inline int GetUnsortedLimit () { return m_unsorted_limit; };
	inline bool GetSortLosers () { return m_sort_losers; };
	inline float GetLowCutoff () { return m_low_cutoff; };
	inline float GetHighCutoff () { return m_high_cutoff; };
	inline uint64_t GetNodeCount () { return m_node_count; };
	inline void * GetContainerData () { return m_container_data; };

	inline int GetCurrentDepth () { return m_current_depth; };
	inline uint64_t GetCulledCount () { return m_culled_count; };

#ifdef VBSP_MEM_LOGGING
	static void Write ( const char * str ) {
		FILE *fp;
		fp = fopen("vbsp_new_delete_logging.txt", "a+");
		fwrite(str, 1, strlen(str), fp);
		fclose(fp);
	}
#endif // VBSP_MEM_LOGGING

		
protected:

	float 							m_bbox[6];
	int 							m_max_depth;	
	int 							m_unsorted_limit;
	bool 							m_sort_losers;
	float 							m_low_cutoff;
	float 							m_high_cutoff;
	void *							m_container_data;
	BSPNode<T, ItemContainer> * 	m_root;
	BSPNode<T, ItemContainer> *		m_last_insertion_point;
	uint64_t						m_node_count;

	int								m_current_depth;
	uint64_t						m_culled_count;
}; 


#endif // VBSP_H
