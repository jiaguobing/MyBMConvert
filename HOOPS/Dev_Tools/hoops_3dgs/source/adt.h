// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef ADT_H
#define ADT_H

#ifndef HOOPS_DEFINED
#include "hoops.h"
#endif



#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4530)	// C++ exception handler used, but unwind semantics are not enabled.
#endif
#include <vector>
#include <set>
//#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <string>
#ifdef _MSC_VER
#	pragma warning(pop)
#endif


#include "unrolled_list.h"
#include "unordered.h"


#define V_PREFIX HOOPS_3DGS_

#include "vlist.h"
#include "vhash.h"
//#include "vconf.h"

#define UTF_UTILS_API HOOPS_API

#include "utf_utils.h"


/* Abstract Data Types -- type independent data structures based on void pointers */


typedef char vbsp_status;

#define VBSP_STATUS_STOP		0
#define VBSP_STATUS_CONTINUE	1
#define VBSP_STATUS_SOMETHING	2

/*
 * VBSP
 */
/* bsp_compare_action
	compares two objects of some caller-defined types (e.g. face_node_t).  Since the only information that
		the VBSP has about the objects is their bounds, it is assumed that this function will know what to do
		with them.	The comparison function is also responsible for doing whatever actions are required when
		the conditions are met (e.g. compute_selection_by_shell's test_face_face_callback does a HI_VArray_Append)
   \param passthru the value provided as one of the arguments to the various test functions.
		It will generally be a pointer to a structure containing all of the data that is global to an
		entire test (e.g. test_bsp_bsp) operation.
   \param item1 the first object of the caller-defined type
   \param item2 (as with item1 above) the second object of some caller-defined type
   \returns false if there is no need to do any further tests (e.g. clash detected)
 */
typedef vbsp_status (*bsp_compare_action)(void *passthru, void *item1, void *item2);
typedef vbsp_status (*bsp_compare_action2)(void *passthru, void *item1, int res);
/* bsp_map_action
	applies the given function pointer to each of the items that had been inserted via HI_BSP_Insert
   \param passthru the value provided as one of the arguments to the various test functions.
   \param item the object of some caller-defined type (e.g. face_node_t)
   \returns false if there is no need to continue (e.g. we were searching for something and found it)
 */
typedef vbsp_status (*bsp_map_action)(void *item, void *passthru);

// for SimpleCuboid
//#include "database.h"

/* contents of the following structure should be considered private.  There should be
 * no need to read or write them except through the provided API */

template <typename F>
struct VBSPT;

template <typename F>
class bsp_unsorted_t : public CMO_Pooled {
public:
	bsp_unsorted_t() {
		;
	}

	~bsp_unsorted_t() {
		;
	}

	static const int	BSP_UNSORTED_LIMIT = 10;
	typedef std::pair<Cuboid_3D<F>, void *> item_type;
	int			used;
	item_type	items[BSP_UNSORTED_LIMIT];
};

template <typename F>
struct bsp_node_t {
	VBSPT<F> *			tree;
	void **				contents;
	bsp_unsorted_t<F> *	unsorted;
	bsp_node_t<F> *		nodes[3];  /* low, high, split */
	Cuboid_3D<F>		cuboid;

	int 				partition_type; /* BSPX, BSPY, or BSPZ */
	int 				contents_used;
	int 				contents_allocated;
};

template <typename F>
struct VBSPT {
	Memory_Pool *			memory_pool;
	bsp_node_t<F> *			root;				/* type is defined in vbsp.c */
	bsp_compare_action		compare;			/* set every time from arg HI_Test_BSP_* arg list */
	bsp_compare_action2		compare2;			/* set every time from arg HI_Test_BSP_* arg list */
	void *					compare_passthru;	/* ditto */
	Cuboid_3D<F>			bbox;				/* bounding volume that contains all items inserted */
	Vector_3D<F>			dir;
	F						current_distance;
	F						allowance;
	int						max_depth;			/* used only when items are inserted */
	bool					current_distance_set;
	bool					hasDirection;
	bool					has_closest_distance;
};


typedef bsp_unsorted_t<float>	bsp_unsorted;
typedef bsp_node_t<float>		bsp_node;
typedef VBSPT<float>			VBSP;
typedef bsp_unsorted_t<double>	dbsp_unsorted;
typedef bsp_node_t<double>		dbsp_node;
typedef VBSPT<double>			DVBSP;

/* creates a new VBSP object */
GLOBAL_FUNCTION VBSP *HI_New_BSP(Memory_Pool * memory_pool, SimpleCuboid const & bbox_in, int max_depth_in);
GLOBAL_FUNCTION DVBSP *HI_New_BSP(Memory_Pool * memory_pool, DSimpleCuboid const & bbox_in, int max_depth_in);

/* HI_Delete_BSP is responsible only for deletion of the bsp itself.  Deletion of any items that were
   inserted into the bsp is the user's responsibility (hint: use HI_BSP_Map_Function) */
GLOBAL_FUNCTION void HI_Delete_BSP(VBSP *tree);
GLOBAL_FUNCTION void HI_Delete_BSP(DVBSP *tree);

/* If possible, start your data type with the 6 floats for bounds and use
	  the same pointer for both item and item bounds (null for item_bounds will
	  have the same effect).  This will avoid the need to copy the bounding box
	  at the time of insertion (since we can know that the box pointer is not going to
	  be prematurely freed)
   \param tree the VBSP object, as created by HI_New_BSP
   \param item the item to be inserted
   \param item bounds an array of 6 floats (min[xyz],max[xyz]) that contain the bounding cuboid of the
	  inserted item.  If either null or the same address as "item", we assume that "item" starts with the
	  relevant information and take it from there (thus avoiding a copy)
*/
GLOBAL_FUNCTION void HI_BSP_Insert(VBSP *tree, void *item, SimpleCuboid const * item_bounds=null);
GLOBAL_FUNCTION void HI_BSP_Insert(DVBSP *tree, void *item, DSimpleCuboid const * item_bounds=null);


/* just like HVHash's map function, except your function returns
	  1 to keep going
	  0 to stop */
GLOBAL_FUNCTION int HI_BSP_Map_Function(
	VBSP *			tree,
	bsp_map_action	function,
	void *			user_data);
GLOBAL_FUNCTION int HI_BSP_Map_Function(
	DVBSP *			tree,
	bsp_map_action	function,
	void *			user_data);

/* these functions are effectively a variant of Map_Function that is simply
	  conditional on matching a certain criteria (either matching bbox'es or
	  intersecting a ray) */
/* test a bsp against provided bounding cuboid */
GLOBAL_FUNCTION int HI_Test_BSP(
	VBSP *					tree,
	bsp_compare_action		compare_callback,
	void *					passthru,
	void *					item2,
	SimpleCuboid const &	bounds,
	bool					closest_distance);
GLOBAL_FUNCTION int HI_Test_BSP(
	DVBSP *					tree,
	bsp_compare_action		compare_callback,
	void *					passthru,
	void *					item2,
	DSimpleCuboid const &	bounds,
	bool					closest_distance);

GLOBAL_FUNCTION int HI_Test_BSP_Bounding_Reject(
	VBSP *					tree,
	bsp_compare_action		compare_callback,
	bsp_compare_action2		compare_callback2,
	void *					passthru,
	void *					item2,
	SimpleCuboid const &	bounds);
GLOBAL_FUNCTION int HI_Test_BSP_Bounding_Reject(
	DVBSP *					tree,
	bsp_compare_action		compare_callback,
	bsp_compare_action2		compare_callback2,
	void *					passthru,
	void *					item2,
	DSimpleCuboid const &	bounds);

/* test the items in one bsp against the items in the other one */
GLOBAL_FUNCTION int HI_Test_BSP_BSP(
	VBSP *				tree,
	bsp_compare_action	compare_callback,
	void *				passthru,
	VBSP *				item2_bsp,
	bool				hasDirection,
	float				allowance = 0.0f);
GLOBAL_FUNCTION int HI_Test_BSP_BSP(
	DVBSP *				tree,
	bsp_compare_action	compare_callback,
	void *				passthru,
	DVBSP *				item2_bsp,
	bool				hasDirection,
	double				allowance = 0.0);

/* test a bsp against a ray */
GLOBAL_FUNCTION bool HI_Test_BSP_Ray(
	VBSP *				tree,
	bsp_compare_action	compare_callback,
	void *				compare_passthru,
	void *				part,
	Point const *		start,
	Vector const *		direction);
GLOBAL_FUNCTION bool HI_Test_BSP_Ray(
	DVBSP *				tree,
	bsp_compare_action	compare_callback,
	void *				compare_passthru,
	void *				part,
	DPoint const *		start,
	DVector const *		direction);








Begin_HOOPS_Namespace

template <typename T, class Alloc = std::allocator<T>, size_t TARGET_K=4>
class Batched_Node_Allocator {

public:

	typedef T value_type;

	typedef Alloc allocator_type;
		
	inline allocator_type get_allocator() const {return allocator_type(_the_allocator);}

private:

	Alloc	_the_allocator;

	inline char * mallocate(size_t n) {
		typedef typename Alloc::template rebind<char>::other rebound;
		return rebound(get_allocator()).allocate(n);
	}

	inline void freeate(char * p, size_t n) {
		typedef typename Alloc::template rebind<char>::other rebound;
		return rebound(get_allocator()).deallocate(p, n);
	}

	enum ConstExpr: size_t {
		SIZEOF_T = sizeof(T),
		BANK_BYTES = TARGET_K*1024,
		PER_BANK_COUNT = BANK_BYTES/sizeof(T),
		BANK_SIZE = PER_BANK_COUNT*sizeof(T)
	};

	struct T_sized {
		char _t_sized_data[SIZEOF_T];
	};

	struct Batched_Node {
		Batched_Node() : _next(null), _used(0) {
			ZERO_ARRAY(_data, PER_BANK_COUNT, T);
		}
		Batched_Node *	_next;
		T_sized			_data[PER_BANK_COUNT];
		size_t			_used;
	};

	Batched_Node *		_nodes;
	Batched_Node		_node;
		
	struct Free_List {
		Free_List *	next;
	};

	Free_List *	_free_list;

public:

	Batched_Node_Allocator() : _the_allocator(Alloc()), _nodes(&_node), _free_list(null) {
		ASSERT(PER_BANK_COUNT > 16); // seems like a ok lower bounds
		ASSERT(BANK_SIZE<=BANK_BYTES);
	};

	Batched_Node_Allocator(Alloc const & a_allocator) : _the_allocator(a_allocator), _nodes(&_node), _free_list(null) {
		ASSERT(PER_BANK_COUNT > 16); // seems like a ok lower bounds
		ASSERT(BANK_SIZE<=BANK_BYTES);
	};

	virtual ~Batched_Node_Allocator() {
		Clear(false);
	};

	void Clear(bool reset = true) {
		while (_nodes != null) {
			Batched_Node *	victim = _nodes;
			_nodes = _nodes->_next;
			if (victim != &_node) {
				victim->~Batched_Node();
				freeate((char*)victim, sizeof(Batched_Node));
			}

		}
		if (reset) {
			ZERO_ARRAY(&_node, PER_BANK_COUNT, T);
			_nodes = &_node;
			_free_list = null;
		}
	};

	T * Alloc_Node() {
		if (_free_list != null) {
			T * node = (T *)_free_list;
			_free_list = _free_list->next;
			ZERO_STRUCT(node, T);
			return node;
		}
		if (_nodes->_used == PER_BANK_COUNT) {
			Batched_Node *	node = (Batched_Node *)mallocate(sizeof(Batched_Node));
			new(node)Batched_Node();
			node->_next = _nodes;
			_nodes = node;
		}
		return (T*)&_nodes->_data[_nodes->_used++];
	};

	void Free_Node(T * node) {
		Free_List *	free_node = (Free_List *)node;
		free_node->next = _free_list;
		_free_list = free_node;
	};
};



template <typename T, class Alloc = std::allocator<T>, size_t TARGET_K=4>
class Banked_Array {

public:

	typedef T value_type;

	typedef Alloc allocator_type;
	inline allocator_type get_allocator() {return allocator_type(_the_allocator);}

private:

	Alloc	_the_allocator;

	inline char * mallocate(size_t n) {
		typedef typename Alloc::template rebind<char>::other rebound;
		return rebound(get_allocator()).allocate(n);
	}

	inline void freeate(char * p, size_t n) {
		typedef typename Alloc::template rebind<char>::other rebound;
		return rebound(get_allocator()).deallocate(p, n);
	}

	T **			_banks;
	T *				_single_bank;

	size_t			_n_banks;
	size_t			_size;

	INLINE void Index_To_Bank_And_Offset(size_t index, size_t & bank, size_t & offset) const {

		if (_n_banks == 1) {
			bank = 0;
			offset = index;
		}
		else {
			bank = index >> BANK_SHIFT;
			offset = index & (PER_BANK_COUNT-1);
		}

	};

	INLINE size_t bank_count(size_t bank) const {
		size_t	use_bank_count = PER_BANK_COUNT;
		if (bank == _n_banks-1)
			use_bank_count = _size - (bank*PER_BANK_COUNT);
		return use_bank_count;
	}

	void get_bank(size_t bank) {
		size_t count = bank_count(bank);
		_banks[bank] = (T *)mallocate(count * sizeof(T));
		ZERO_ARRAY(_banks[bank], count, T);
	}

	void free_bank(size_t bank) {
		ASSERT(_banks[bank] != null);
		size_t count = bank_count(bank);
		freeate((char*)_banks[bank], count * sizeof(T));
		_banks[bank] = null;
	}

	Banked_Array();
	void operator = (Banked_Array<T, Alloc, TARGET_K> const &that);

	enum ConstExpr: size_t {
		SIZEOF_T = sizeof(T),
		BANK_BYTES = TARGET_K*1024,
		BANK_SHIFT = log_<(BANK_BYTES/sizeof(T))>::value,
		PER_BANK_COUNT = 1 << BANK_SHIFT,
		BANK_SIZE = PER_BANK_COUNT*sizeof(T)
	};

public:
	Banked_Array(size_t in_size)
		: _the_allocator(Alloc()), _single_bank(null), _size(in_size)
	{
		ASSERT(LOWBIT(PER_BANK_COUNT) == PER_BANK_COUNT);
		ASSERT(PER_BANK_COUNT > 16); // seems like a ok lower bounds
		ASSERT(BANK_SIZE<=BANK_BYTES);

		if (_size <= PER_BANK_COUNT) {
			_n_banks = 1;
			_banks = &_single_bank;
			_banks[0] = null;
		}
		else
		{
#ifdef HOOPS_DEBUG_BUILD
			size_t pbc = PER_BANK_COUNT;
			size_t bank_shift = 0;
			while (pbc > 1) {
				bank_shift++;
				pbc >>= 1;
			}
			ASSERT(bank_shift == BANK_SHIFT);
#endif
			_n_banks = (_size + PER_BANK_COUNT - 1)/PER_BANK_COUNT;
			ASSERT(_n_banks>1);
			_banks = (T**)mallocate(_n_banks * sizeof(T*));
			ZERO_ARRAY(_banks, _n_banks, T*);
		}
	};

	Banked_Array(Alloc const & a_allocator, size_t in_size)
		: _the_allocator(a_allocator), _single_bank(null), _size(in_size)
	{
		ASSERT(LOWBIT(PER_BANK_COUNT) == PER_BANK_COUNT);
		ASSERT(PER_BANK_COUNT > 16); // seems like a ok lower bounds
		ASSERT(BANK_SIZE<=BANK_BYTES);

		if (_size <= PER_BANK_COUNT) {
			_n_banks = 1;
			_banks = &_single_bank;
			_banks[0] = null;
		}
		else
		{
#ifdef HOOPS_DEBUG_BUILD
			size_t pbc = PER_BANK_COUNT;
			size_t bank_shift = 0;
			while (pbc > 1) {
				bank_shift++;
				pbc >>= 1;
			}
			ASSERT(bank_shift == BANK_SHIFT);
#endif
			_n_banks = (_size + PER_BANK_COUNT - 1)/PER_BANK_COUNT;
			ASSERT(_n_banks>1);
			_banks = (T**)mallocate(_n_banks * sizeof(T*));
			ZERO_ARRAY(_banks, _n_banks, T*);
		}
	};

	Banked_Array(Banked_Array<T, Alloc, TARGET_K> const &that)
		: _the_allocator(that._the_allocator), _single_bank(null), _n_banks(that._n_banks), _size(that._size)
	{

		if (_n_banks == 1) {
			_banks = &_single_bank;
		}
		else {
			_banks = (T**)mallocate(_n_banks * sizeof(T*));
			ZERO_ARRAY(_banks, _n_banks, T*);
		}

		for (size_t i=0; i<_n_banks; i++) {
			if (that._banks[i] != null) {
				size_t count = bank_count(i);
				_banks[i] = (T *)mallocate(count * sizeof(T));
				COPY_ARRAY(that._banks[i], count, T, _banks[i]);
			}
		}
	};

	~Banked_Array() {
		ResetAllBanks();
		if (_n_banks > 1)
			freeate((char*)_banks, _n_banks*sizeof(T*));
	};

	INLINE size_t size() const {return _size;}
	INLINE bool empty() const {return (_size>0);}

	INLINE T * entry_valid(size_t index) const {
		ASSERT(index < _size);
		size_t bank, offset;
		Index_To_Bank_And_Offset(index, bank, offset);
		T * ptr = null;
		if (_banks[bank] != null)
			ptr = &_banks[bank][offset];
		return ptr;
	};

	INLINE T & operator [] (size_t index) {
		ASSERT(index < _size);
		size_t bank, offset;
		Index_To_Bank_And_Offset(index, bank, offset);
		if (_banks[bank] == null)
			get_bank(bank);
		return _banks[bank][offset];
	};

	INLINE T const & operator [] (size_t index) const { //-V659
		return Alter(this)->operator [](index);
	};

	bool operator == (Banked_Array<T, Alloc, TARGET_K> const & that) const {
		if (_size != that._size)
			return false;
		if (_n_banks == that._n_banks) {
			for (size_t i=0; i<_n_banks; ++i) {
				T *		this_bank = _banks[i];
				T *		that_bank = that._banks[i];
				if (this_bank != null && that_bank != null) {
					size_t count = bank_count(i);
					for (size_t j=0; j<count; j++) {
						if (this_bank[j] != that_bank[j])
							return false;
					}
				}
				else if (this_bank != null || that_bank != null)
					return false;
			}
		}
		else {	// ugh
			for (size_t i=0; i<_size; ++i) {
				T *		our_entry = entry_valid (i);
				T *		their_entry = that.entry_valid (i);
				if (our_entry != null) {
					if (their_entry != null) {
						if (*our_entry != *their_entry)
							return false;
					}
					else
						return false;
				}
				else if (their_entry != null)
					return false;
			}
		}
		return true;
	}

	void NullAll() {
		for (size_t i=0; i<_n_banks; ++i) {
			if (_banks[i] != null) {
				size_t count = bank_count(i);
				for (size_t j=0; j<count; j++) {
						_banks[i][j] = null;
				}
			}
		}
	}

	void ResetAllBanks() {
		if (_banks != null) {
			for (size_t i=0; i<_n_banks; ++i) {
				if (_banks[i] != null)
					free_bank(i);
			}
		}
	}



	class const_iterator : public std::iterator<std::random_access_iterator_tag, value_type, size_t, value_type const *, value_type const &>
	{
	protected:
		Banked_Array *	_ba;
		size_t			_index;

		inline void limit_to_end() const {
			if (_index>_ba->size())
				Alter(_index) = _ba->size();
		}

	public:
		typedef std::random_access_iterator_tag iterator_category;
		//typedef value_type			value_type;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type const *	pointer;
		typedef value_type const &	reference;

		inline const_iterator() : _ba(null), _index(0) {}

		inline const_iterator(size_t index, Banked_Array const * const ba) : _ba((Banked_Array*)ba), _index(index) {
			limit_to_end();
		}

		INLINE T * entry_valid() const {
			return _ba->entry_valid(_index);
		};

		INLINE size_t index() const {
			return _index;
		};

		INLINE void advance() { // find a valid entry or end
			while (_index < _ba->size()) {
				++_index;
				if (_index == _ba->size() || entry_valid())
					break;
			}
		};

		inline reference operator*() const {
			return _ba->operator[](_index);
		}

		inline pointer operator->() const {
			return &(**this);
		}

		inline const_iterator & operator++() {
			++_index;
			limit_to_end();
			return *this;
		}

		inline const_iterator operator++(int) {
			const_iterator old_it = *this;
			++(*this);
			return old_it;
		}

		inline const_iterator & operator--() {
			if (_index > 0)
				--_index;
			else
				_index = std::numeric_limits<size_t>::max(); // rend
			return *this;
		}

		inline const_iterator operator--(int) {
			const_iterator old_it = *this;
			--(*this);
			return old_it;
		}

		inline const_iterator & operator+=(difference_type n) {
			if (_index >= (size_t)Abs(n)) {
				_index += n;
				limit_to_end();
			}
			else if (n > 0) {
				_index += n;
				limit_to_end();
			}
			else
				_index = std::numeric_limits<size_t>::max(); // rend
			return *this;
		}

		inline const_iterator & operator-=(difference_type n) {
			(*this)+=-n;
			return *this;
		}

		inline const_iterator operator+(difference_type n) const {
			const_iterator copy_it = *this;
			copy_it += n;
			return copy_it;
		}

		inline const_iterator operator-(difference_type n) const {
			const_iterator copy_it = *this;
			copy_it += -n;
			return copy_it;
		}

		inline difference_type operator-(const_iterator const & that) const {
			ASSERT(_ba == that._ba);
			ASSERT(_index <= _ba->size());
			ASSERT(that._index <= _ba->size());
// 			limit_to_end();
// 			that.limit_to_end();
			difference_type index = _index;
			difference_type that_index = that._index;
			return (index - that_index);
		}

		inline reference operator[](difference_type offset) const {
			return (*(*this + offset));
		}

		inline bool operator <(const_iterator const & that) const {
			ASSERT(_ba == that._ba);
			ASSERT(_index <= _ba->size());
			ASSERT(that._index <= _ba->size());
// 			limit_to_end();
// 			that.limit_to_end();
			return _index < that._index;
		}

		inline bool operator >(const_iterator const & that) const {
			ASSERT(_ba == that._ba);
			ASSERT(_index <= _ba->size());
			ASSERT(that._index <= _ba->size());
// 			limit_to_end();
// 			that.limit_to_end();
			return _index > that._index;
		}

		inline bool operator <=(const_iterator const & that) const {
			ASSERT(_ba == that._ba);
			ASSERT(_index <= _ba->size());
			ASSERT(that._index <= _ba->size());
// 			limit_to_end();
// 			that.limit_to_end();
			return _index <= that._index;
		}

		inline bool operator >=(const_iterator const & that) const {
			ASSERT(_ba == that._ba);
			ASSERT(_index <= _ba->size());
			ASSERT(that._index <= _ba->size());
// 			limit_to_end();
// 			that.limit_to_end();
			return _index >= that._index;
		}

		inline bool operator ==(const_iterator const & that) const {
			ASSERT(_ba == that._ba);
			ASSERT(_index <= _ba->size());
			ASSERT(that._index <= _ba->size());
// 			limit_to_end();
// 			that.limit_to_end();
			return _index == that._index;
		}

		inline bool operator !=(const_iterator const & that) const {
			return !(*this == that);
		}

		friend class Banked_Array;
	};

	class iterator : public const_iterator
	{
	public:
		typedef std::random_access_iterator_tag iterator_category;
		//typedef value_type			value_type;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type *			pointer;
		typedef value_type &			reference;

		inline iterator() : const_iterator() {}

		inline iterator(size_t index, Banked_Array * ba) : const_iterator(index, ba) {}

		inline reference operator*() {
			return const_cast<reference>(const_iterator::operator*());
		}

		inline pointer operator->() {
			return const_cast<pointer>(&(**this));
		}

		inline iterator & operator++() {
			const_iterator::operator++();
			return *this;
		}

		inline iterator operator++(int) {
			iterator old_it = *this;
			++(*this);
			return old_it;
		}

		inline iterator & operator--() {
			const_iterator::operator--();
			return *this;
		}

		inline iterator operator--(int) {
			iterator old_it = *this;
			--(*this);
			return old_it;
		}

		inline iterator & operator+=(difference_type n) {
			const_iterator::operator+=(n);
			return *this;
		}

		inline iterator & operator-=(difference_type n) {
			const_iterator::operator-=(n);
			return *this;
		}

		inline iterator operator+(difference_type n) const {
			iterator copy_it = *this;
			copy_it += n;
			return copy_it;
		}

		inline iterator operator-(difference_type n) const {
			iterator copy_it = *this;
			copy_it += -n;
			return copy_it;
		}

		inline difference_type operator-(iterator const & that) const {
			return const_iterator::operator-(that);
		}

		friend class Banked_Array;
	};


	inline iterator begin() {
		return iterator(0, this);
	}

	inline iterator end() {
		return iterator(size(), this);
	}

	inline iterator index_iterator(size_t index) {
		return iterator(index, this);
	}

	inline const_iterator begin() const {
		return const_iterator(0, this);
	}

	inline const_iterator end() const {
		return const_iterator(size(), this);
	}

	inline const_iterator index_iterator(size_t index) const {
		return const_iterator(index, this);
	}

	inline const_iterator cbegin() const {
		return const_iterator(0, this);
	}

	inline const_iterator cend() const {
		return const_iterator(size(), this);
	}

	inline const_iterator cindex_iterator(size_t index) const { //-V524
		return const_iterator(index, this);
	}
};

#include "vxmap.h"

namespace INI {

	enum Config_Style {
		Flat,
		Chunked,
	};

	template <char delimiter = ':', Config_Style style = Config_Style::Chunked>
	class Config {

	public:

		typedef std::basic_string<char, std::char_traits<char>, std::allocator<char>>	string;
		typedef std::unordered_map<string, string>	chunk_t;
		typedef std::vector<chunk_t>	chunks_t;

		chunks_t	chunks;

		Config() {
			;
		}

		~Config() {
			;
		}

		bool Read(const char * filename) {

			chunks.clear();

			if (filename == null || filename[0] == '\0')
				return false;

			FILE *	file = fopen(filename, "rb");

			if (file == NULL)
				return false;

			fseek(file, 0, SEEK_SET);

			chunks.push_back(std::move(chunk_t()));

			bool done = false;

			string	line_buffer;
			line_buffer.reserve(256);

			bool	have_a_char = false;
			int		a_char = 0;

			while (!done) {

				line_buffer.clear();

				// getline-ish
				while (1) {

					if (have_a_char)
						have_a_char = false;
					else
						a_char = fgetc(file);

					if (a_char == EOF) {
						done = true;
						if (!line_buffer.empty())
							line_buffer.push_back('\0');
						break;
					}

					if (a_char == 0x0D || a_char == 0x0A) {

						int last_char = a_char;
						a_char = fgetc(file);
						if (a_char == 0x0D || a_char == 0x0A) {
							if (a_char == last_char)
								have_a_char = true;
						}
						else
							have_a_char = true;

						line_buffer.push_back('\0');
						break;
					}

					if (a_char == '\t' || a_char == '\0')
						a_char = ' ';

					line_buffer.push_back((char)a_char);
				}

				if (line_buffer.empty() || line_buffer.front() == '\0') {
					if (style == Config_Style::Chunked && !chunks.back().empty())
						chunks.push_back(std::move(chunk_t()));
					continue;
				}

				size_t i = 0;
				size_t n = line_buffer.size();

				while (i<n && line_buffer[i] == ' ')
					i++;

				if (line_buffer[i] == '\0')
					continue;

				if (line_buffer[i] == ';' || line_buffer[i] == '#')
					continue;

				size_t	option_type_start = i;

				while (i<n && (line_buffer[i] != ' ' && line_buffer[i] != delimiter))
					i++;

				size_t	option_type_end = i;

				// default empty option
				size_t	option_start = 0;
				size_t	option_end = 0;

				i++;

				// skip space and delimiter
				while (i<n) {
					if (line_buffer[i] == ' ' || line_buffer[i] == delimiter)
						i++;
					else
						break;
				}

				if (line_buffer[i] == ';' || line_buffer[i] == '#')
					;
				else if (line_buffer[i] == '"' || line_buffer[i] == '\'') {
					char look_for_match = line_buffer[i];
					// quoted option
					i++;
					// real option start
					option_start = i;
					while(i<n && line_buffer[i] != look_for_match)
						i++;
					option_end = i;
				}
				else {
					// unquoted option
					// real option start
					option_start = i;
					while (i<n && (line_buffer[i] != '#' && line_buffer[i] != ';'))
						i++;
					// backup over any blanks
					while (line_buffer[i] == ' ')
						i--;
					option_end = i;
				}

				chunks.back().insert(std::make_pair(
					string(&line_buffer.data()[option_type_start], option_type_end-option_type_start),
					string(&line_buffer.data()[option_start], option_end-option_start)));
			}

			if (chunks.back().empty())
				chunks.pop_back();

			fclose(file);

			return true;
		}
	};
} // INI

End_HOOPS_Namespace

#endif /*ADT_H*/


