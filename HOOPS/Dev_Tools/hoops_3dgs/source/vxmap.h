#ifndef VXHASH_H
#define VXHASH_H

#include "hoops.h"

#include <memory>
#include <type_traits>
#include <cstddef>

#ifdef _MSC_VER
#	pragma warning(push)   // Save the current warning state
#	pragma warning(disable:4530)
#endif
#include <iterator>
#ifdef _MSC_VER
#	pragma warning(pop)   // Restore all warnings to the previous state
#endif


template <typename K, class Hash = Hasher<K>, class Pred = std::equal_to<K>, class Alloc = std::allocator<K> >
class VXSet {

public:
	typedef K		key_type;
	typedef K		value_type;
	typedef size_t	size_type;

	typedef Hash hasher;
	hasher hash_function() const {return hasher();}

	typedef Pred key_equal;
	key_equal key_eq() const {return key_equal();}

	typedef Alloc allocator_type;
	inline allocator_type get_allocator() const {return allocator_type(_the_allocator);}

private:
	
	Alloc	_the_allocator;

	typedef Banked_Array<value_type, Alloc> Bank_Type;

	typedef typename std::make_unsigned<typename std::conditional<std::is_pointer<key_type>::value, size_t, key_type>::type>::type	Special_K_Type;

	enum Special : Special_K_Type {
		EMPTY_ = 0,
		TOMBSTONE_ = 1,
		KEY_MASK_ = (Special_K_Type)~(EMPTY_ | TOMBSTONE_)
	};

	inline key_type EMPTY() const { return (key_type)EMPTY_; }
	inline key_type TOMBSTONE() const { return (key_type)TOMBSTONE_; }
	inline key_type KEY_MASK() const { return (key_type)KEY_MASK_; }

	typename Destructible<Bank_Type>::unique_ptr	_bank;
	size_t		_size;

	value_type	_empty;
	value_type	_tombstone;

	inline size_t hash_key(const key_type & key) const {
		hasher _hasher;
		size_t hash = _hasher(key);
		hash &= _bank->size() - 1;
		return hash;
	}

	inline bool is_full() const {
		return (_size > ((_bank->size()>>1) + (_bank->size()>>2))); // 75%
	}

	size_t internal_find(const value_type key, const value_type *& out_item) const
	{
		ASSERT(key != EMPTY());
		ASSERT(key != TOMBSTONE());
		ASSERT(out_item == null);

		size_t	hash = hash_key(key);

		for (auto it = _bank->cindex_iterator(hash), end = _bank->cend(); it != end; ++it) {

			value_type * item = it.entry_valid();
			value_type	value;
			if (item == null || (value = *item, key_equal()(value, EMPTY())))
				return std::numeric_limits<size_t>::max();
			if (key_equal()(value, key)) {
				out_item = item;
				return it.index();
			}
		}

		for (auto it = _bank->cbegin(), end = _bank->cindex_iterator(hash); it != end; ++it) {

			value_type * item = it.entry_valid();
			value_type	value;
			if (item == null || (value = *item, key_equal()(value, EMPTY())))
				return std::numeric_limits<size_t>::max();
			if (key_equal()(value, key)) {
				out_item = item;
				return it.index();
			}
		}

		return std::numeric_limits<size_t>::max();
	}

	void internal_rehash (size_t new_size)
	{
		size_t		old_size = _size;

		_size = 0;
		if (_empty == EMPTY())
			_size++;
		if (_tombstone == TOMBSTONE())
			_size++;

		typename Destructible<Bank_Type>::unique_ptr	old_bank;
		old_bank.swap(_bank);
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), new_size);

		for (auto it = old_bank->cbegin(), end = old_bank->cend(); it != end; ++it) {
			value_type * item = it.entry_valid();
			value_type	value;
			if (item != null && (value = *item, ((Special_K_Type)value & KEY_MASK_) > TOMBSTONE_)) {
				const value_type * dummy_item = null;
				bool had_key = false;
				size_t index = internal_insert(*item, dummy_item, had_key);
				ASSERT(had_key == false);
				ASSERT(index != std::numeric_limits<size_t>::max());
			}
		}

		ASSERT(old_size == _size);
		old_bank = null;
	}

	size_t internal_insert(const value_type key, const value_type *& out_item, bool & had_key)
	{
		ASSERT(key != EMPTY());
		ASSERT(key != TOMBSTONE());
		ASSERT(out_item == null);
		ASSERT(had_key == false);

		size_t index = internal_find(key, out_item);

		if (index != std::numeric_limits<size_t>::max()) {
			had_key = true;
			auto it = _bank->index_iterator(index);
			*it = key;
			return index;
		}

		if (is_full()) {
			internal_rehash (_bank->size()*2);
			return internal_insert(key, out_item, had_key);
		}

		ASSERT(_bank->end() == _bank->index_iterator(_bank->size()));

		size_t	hash = hash_key(key);

		for (auto it = _bank->index_iterator(hash), end = _bank->end(); it != end; ++it) {
			value_type * item = it.entry_valid();
			value_type	value;
			if (item == null || (value = *item, (((Special_K_Type)value & KEY_MASK_) <= TOMBSTONE_))) {
				*it = key;
				out_item = &*it;
				_size++;
				return it.index();
			}
		}

		for (auto it = _bank->begin(), end = _bank->index_iterator(hash); it != end; ++it) {
			value_type * item = it.entry_valid();
			value_type	value;
			if (item == null || (value = *item, (((Special_K_Type)value & KEY_MASK_) <= TOMBSTONE_))) {
				*it = key;
				out_item = &*it;
				_size++;
				return it.index();
			}
		}
		
		ASSERT(0); // bug
		return std::numeric_limits<size_t>::max();
	}

public:


	VXSet(size_t intial_capacity=16)
		: _the_allocator(Alloc()), _bank(null), _size(0), _empty(TOMBSTONE()), _tombstone(EMPTY()) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<value_type>::value);
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		ASSERT(intial_capacity >= 1);
		intial_capacity = Nearest_Power_Of_2(static_cast<uint32_t>(intial_capacity));
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), intial_capacity);
	}

	VXSet(Alloc const & a_allocator, size_t intial_capacity = 16)
		: _the_allocator(a_allocator), _bank(null), _size(0), _empty(TOMBSTONE()), _tombstone(EMPTY()) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<value_type>::value);
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		ASSERT(intial_capacity >= 1);
		intial_capacity = Nearest_Power_Of_2(static_cast<uint32_t>(intial_capacity));
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), intial_capacity);
	}

	VXSet(VXSet const & that)
		: _the_allocator(that.get_allocator()), _bank(null), _size(that._size), _empty(that._empty), _tombstone(that._tombstone) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<value_type>::value);
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), that._bank->size());
		auto nit = _bank.begin();
		for (auto it = that._bank->cbegin(), end = that._bank->cend(); it != end; ++it, ++nit) {
			if (it.entry_valid())
				nit = it;
		}
	}

	VXSet(VXSet && that)
		: _the_allocator(that.get_allocator()), _bank(null), _size(that._size), _empty(that._empty), _tombstone(that._tombstone) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<value_type>::value);
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), 16);
		_bank.swap(that._bank);
		that._size = 0;
	}

	~VXSet() {
		clear();
		_bank.reset();
	}

	inline bool empty() const {
		return (_size == 0);
	}

	inline size_t size() const {
		return _size;
	}

	inline void clear() {

		if (!empty()) {
			_bank->ResetAllBanks();
			_size = 0;
			_empty = TOMBSTONE();
			_tombstone = EMPTY();
		}
	}

	void rehash(size_type n) {

		size_type use_n = Nearest_Power_Of_2(static_cast<uint32_t>(n));
		if (size() < ((use_n >> 1) + (use_n >> 2))) // 75%
			internal_rehash(use_n);
	}

	class const_iterator : public std::iterator<std::forward_iterator_tag, value_type, size_t, value_type const *, value_type const &>
	{
	private:

		VXSet *				_vx;
		value_type const *	_special;
	
		typename Bank_Type::const_iterator	_bank_it;
		typename Bank_Type::const_iterator	_bank_end;

		INLINE void advance_to_item() {

			while (_bank_it != _bank_end) {
				value_type * item = _bank_it.entry_valid();
				value_type	value;
				if (item != null && (value = *item, ((Special_K_Type)value & KEY_MASK_) > TOMBSTONE_))
					break;
				_bank_it.advance();
			}

			if (_bank_it == _bank_end) {
				ASSERT(_special != (value_type const *)~0); // advanced the end

				while (1) { // not a loop

					if (_special == null && _vx->_empty == _vx->EMPTY()) {
						_special = &_vx->_empty;
						break;
					}
					else if ((_special == null || _special == &_vx->_empty) && _vx->_tombstone == _vx->TOMBSTONE()) {
						_special = &_vx->_tombstone;
						break;
					}

					_special = (value_type const *)~0;
					break;
				}
			}
		}

		INLINE const_iterator(bool begin, VXSet const * const vx) : _vx((VXSet *)vx) {

			_bank_end = vx->_bank->cend();

			if (begin) {
				_bank_it = vx->_bank->cbegin();
				_special = null;
				advance_to_item();
			}
			else {
				_bank_it = _bank_end;
				_special = (value_type const *)~0;
			}
		}

		INLINE const_iterator(VXSet const * const vx, bool is_tombstone) : _vx((VXSet *)vx) {

			_bank_end = vx->_bank->cend();
			_bank_it = _bank_end;

			if (is_tombstone) {
				_special = &_vx->_tombstone;
				ASSERT(_vx->_tombstone == _vx->TOMBSTONE());
			}
			else {
				_special = &_vx->_empty;
				ASSERT(_vx->_empty == _vx->EMPTY());
			}
		}

		INLINE const_iterator(size_t index, VXSet const * const vx) : _vx((VXSet *)vx) {

			ASSERT(index < vx->_bank->size());

			_bank_end = vx->_bank->cend();
			_bank_it = vx->_bank->cindex_iterator(index);

			_special = null;
#ifdef HOOPS_DEBUG_BUILD
			value_type * item = _bank_it.entry_valid();
			value_type	value;
			ASSERT(item != null && (value = *item, ((Special_K_Type)value & KEY_MASK_) != 0));
#endif
		}

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type const *	pointer;
		typedef value_type const &	reference;

		INLINE const_iterator() : _vx(null), _bank_it(), _bank_end(), _special(null) {}

		INLINE reference operator*() const {
			if (_special == null)
				return *_bank_it;
			ASSERT(_special != (value_type const *)~0); // dereferenced end
			return *_special;
		}

		INLINE pointer operator->() const {
			return &(**this);
		}

		INLINE const_iterator & operator++() {
			++_bank_it;
			advance_to_item();
			return *this;
		}

		INLINE const_iterator operator++(int) {
			const_iterator old_it = *this;
			++(*this);
			return old_it;
		}

		INLINE bool operator ==(const_iterator const & that) const {
			ASSERT(_vx == that._vx);
			return (_special == that._special && _bank_it == that._bank_it && _bank_end == that._bank_end);
		}

		INLINE bool operator !=(const_iterator const & that) const {
			return !(*this == that);
		}

		friend class VXSet;
	};

	class iterator : public const_iterator
	{
	private:

		INLINE iterator(bool begin, VXSet const * const vx) : const_iterator(begin, vx) {}

		INLINE iterator(size_t index, VXSet const * const vx) : const_iterator(index, vx) {}

		INLINE iterator(VXSet const * const vx, bool is_tombstone) : const_iterator(vx, is_tombstone) {}

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type *		pointer;
		typedef value_type &		reference;

		INLINE iterator() : const_iterator() {}

		INLINE iterator & operator++() {
			const_iterator::operator++();
			return *this;
		}

		INLINE iterator operator++(int) {
			iterator old_it = *this;
			++(*this);
			return old_it;
		}

		friend class VXSet;
	};


	INLINE iterator begin() {
		return iterator(true, this);
	}

	INLINE iterator end() {
		return iterator(false, this);
	}

	INLINE const_iterator begin() const {
		return const_iterator(true, this);
	}

	INLINE const_iterator end() const {
		return const_iterator(false, this);
	}

	INLINE const_iterator cbegin() const {
		return const_iterator(true, this);
	}

	INLINE const_iterator cend() const {
		return const_iterator(false, this);
	}


	INLINE const_iterator find(const value_type & key) const {

		if (key == TOMBSTONE()) {
			if (_tombstone == TOMBSTONE())
				return const_iterator(this, true);
		}
		else if (key == EMPTY()) {
			if (_empty == EMPTY()) {
				return const_iterator(this, false);
			}
		}
		else {
			const value_type * out_element = null;

			size_t index = internal_find(key, out_element);

			if (index != std::numeric_limits<size_t>::max()) {
				ASSERT(key_equal()(*out_element, key));
				return const_iterator(index, this);
			}
		}

		return cend();
	}

	// bool is true if it was inserted false if it already existed
	INLINE std::pair<iterator, bool> insert(const value_type & key) {

		if (key == TOMBSTONE()) {
			bool had_key = false;
			if (_tombstone == TOMBSTONE())
				had_key = true;
			else
				++_size;
			_tombstone = TOMBSTONE();
			return std::make_pair(iterator(this, true), !had_key);
		}
		else if (key == EMPTY()) {
			bool had_key = false;
			if (_empty == EMPTY())
				had_key = true;
			else
				++_size;
			_empty = EMPTY();
			return std::make_pair(iterator(this, false), !had_key);
		}

		bool had_key = false;
		const value_type * out_element = null;
		size_t	index = internal_insert(key, out_element, had_key);
		ASSERT(key_equal()(*out_element, key));

		return std::make_pair(iterator(index, this), !had_key);
	}

	INLINE size_t erase(const value_type & key) {

		if (key == EMPTY()) {
			if (_empty == EMPTY()) {
				_empty = TOMBSTONE();
				_size--;
				return 1;
			}
		}
		else if (key == TOMBSTONE()) {
			if (_tombstone == TOMBSTONE()) {
				_tombstone = EMPTY();
				_size--;
				return 1;
			}
		}
		else {

			const value_type * out_element = null;
			size_t index = internal_find(key, out_element);

			if (out_element != null) {
				ASSERT(key_equal()(*out_element, key));
				*Alter(out_element) = TOMBSTONE();
				_size--;
				if (_size == 0)
					_bank->ResetAllBanks();
				return 1;
			}
			else
				ASSERT(index == std::numeric_limits<size_t>::max());
		}

		return 0;
	}

	INLINE iterator erase(const_iterator position) {
		
		ASSERT(position._vx == this);

		if (position._special == null) {
			ASSERT(position._bank_it != position._bank_end);
			auto bank_it = _bank->index_iterator(position._bank_it.index());
			*bank_it = TOMBSTONE();
			_size--;
			if (_size == 0) {
				_bank->ResetAllBanks();
				return end();
			}
			bank_it.advance();
			auto bank_end = _bank->end();
			while (bank_it != bank_end) {
				value_type *	item = bank_it.entry_valid();
				value_type		value;
				if (item != null && (value = *item, ((Special_K_Type)value & KEY_MASK_) > TOMBSTONE_))
					break;
				bank_it.advance();
			}
			if (bank_it != _bank->end())
				return iterator(bank_it.index(), this);
			else if (_empty == EMPTY())
				return iterator(this, false);
			else if (_tombstone == TOMBSTONE())
				return iterator(this, true);
			return end();
		}
		else if (position._special == &_empty) {
			ASSERT(_empty == EMPTY());
			_empty = TOMBSTONE();
			_size--;
			if (_size == 0)
				_bank->ResetAllBanks();
			else if (_tombstone == TOMBSTONE())
				return iterator(this, true);
			return end();
		}
		else if (position._special == &_tombstone) {
			ASSERT(_tombstone == TOMBSTONE());
			_tombstone = EMPTY();
			_size--;
			if (_size == 0)
				_bank->ResetAllBanks();
			return end();
		}

		ASSERT(position._special != (value_type const *)~0); // dereferenced end
		return end();
	}


};



template <typename K, typename T, class Hash = Hasher<K>, class Pred = std::equal_to<K>, class Alloc = std::allocator<std::pair<const K, T> > >
class VXMap {

public:

	typedef K key_type;
	typedef T mapped_type;
	typedef std::pair<const key_type, mapped_type> value_type;

	typedef size_t	size_type;

	typedef Hash hasher;
	hasher hash_function() const { return hasher(); }

	typedef Pred key_equal;
	key_equal key_eq() const { return key_equal(); }

	typedef Alloc allocator_type;
	inline allocator_type get_allocator() const { return allocator_type(_the_allocator); }

private:
	enum {
		Key,
		Item
	};

	Alloc	_the_allocator;

	typedef Banked_Array<value_type, Alloc> Bank_Type;

	typedef typename std::make_unsigned<typename std::conditional<std::is_pointer<key_type>::value, size_t, key_type>::type>::type	Special_K_Type;

	enum Special : Special_K_Type {
		EMPTY_ = 0,
		TOMBSTONE_ = 1,
		KEY_MASK_ = (Special_K_Type)~(EMPTY_ | TOMBSTONE_)
	};

	inline key_type EMPTY() const { return (key_type)EMPTY_; }
	inline key_type TOMBSTONE() const { return (key_type)TOMBSTONE_; }
	inline key_type KEY_MASK() const { return (key_type)KEY_MASK_; }

	typename Destructible<Bank_Type>::unique_ptr	_bank;
	size_t		_size;

	typedef std::pair<key_type, mapped_type> nc_value_type;

	nc_value_type	_empty;
	nc_value_type	_tombstone;

	inline size_t hash_key(const key_type & key) const {
		hasher _hasher;
		size_t hash = _hasher(key);
		hash &= _bank->size() - 1;
		return hash;
	}

	inline bool is_full() const {
		return (_size > ((_bank->size() >> 1) + (_bank->size() >> 2))); // 75%
	}

	size_t internal_find(const key_type key, const value_type *& out_item) const
	{
		ASSERT(key != EMPTY());
		ASSERT(key != TOMBSTONE());
		ASSERT(out_item == null);

		size_t	hash = hash_key(key);

		for (auto it = _bank->cindex_iterator(hash), end = _bank->cend(); it != end; ++it) {

			value_type *	item = it.entry_valid();
			key_type		item_key;
			if (item == null || (item_key = std::get<Key>(*item), key_equal()(item_key, EMPTY())))
				return std::numeric_limits<size_t>::max();
			if (key_equal()(item_key, key)) {
				out_item = item;
				return it.index();
			}
		}

		for (auto it = _bank->cbegin(), end = _bank->cindex_iterator(hash); it != end; ++it) {

			value_type *	item = it.entry_valid();
			key_type		item_key;
			if (item == null || (item_key = std::get<Key>(*item), key_equal()(item_key, EMPTY())))
				return std::numeric_limits<size_t>::max();
			if (key_equal()(item_key, key)) {
				out_item = item;
				return it.index();
			}
		}

		return std::numeric_limits<size_t>::max();
	}

	void internal_rehash(size_t new_size)
	{
		size_t		old_size = _size;

		_size = 0;
		if (std::get<Key>(_empty) == EMPTY())
			_size++;
		if (std::get<Key>(_tombstone) == TOMBSTONE())
			_size++;

		typename Destructible<Bank_Type>::unique_ptr	old_bank;
		old_bank.swap(_bank);
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), new_size);

		for (auto it = old_bank->cbegin(), end = old_bank->cend(); it != end; ++it) {

			value_type *	item = it.entry_valid();
			key_type		item_key;
			if (item != null && (item_key = std::get<Key>(*item), ((Special_K_Type)item_key & KEY_MASK_) > TOMBSTONE_)) {
				const value_type * dummy_item = null;
				bool had_key = false;
				size_t index = internal_insert(*item, dummy_item, had_key);
				ASSERT(had_key == false);
				ASSERT(index != std::numeric_limits<size_t>::max());
			}

		}

		ASSERT(old_size == _size);
		old_bank = null;
	}

	size_t internal_insert(const value_type & in_item, const value_type *& out_item, bool & had_key)
	{
		const key_type key = std::get<Key>(in_item);
		ASSERT(key != EMPTY());
		ASSERT(key != TOMBSTONE());
		ASSERT(out_item == null);
		ASSERT(had_key == false);

		size_t index = internal_find(key, out_item);

		if (index != std::numeric_limits<size_t>::max()) {
			had_key = true;
			auto it = _bank->index_iterator(index);
			reinterpret_cast<nc_value_type &>(*it) = in_item;
			return index;
		}

		if (is_full()) {
			internal_rehash(_bank->size() * 2);
			return internal_insert(in_item, out_item, had_key);
		}

		ASSERT(_bank->end() == _bank->index_iterator(_bank->size()));

		size_t	hash = hash_key(key);

		for (auto it = _bank->index_iterator(hash), end = _bank->end(); it != end; ++it) {
			value_type *	item = it.entry_valid();
			key_type		item_key;
			if (item == null || (item_key = std::get<Key>(*item), (((Special_K_Type)item_key & KEY_MASK_) <= TOMBSTONE_))) {
				reinterpret_cast<nc_value_type &>(*it) = in_item;
				out_item = &*it;
				_size++;
				return it.index();
			}
		}

		for (auto it = _bank->begin(), end = _bank->index_iterator(hash); it != end; ++it) {
			value_type *	item = it.entry_valid();
			key_type		item_key;
			if (item == null || (item_key = std::get<Key>(*item), (((Special_K_Type)item_key & KEY_MASK_) <= TOMBSTONE_))) {
				reinterpret_cast<nc_value_type &>(*it) = in_item;
				out_item = &*it;
				_size++;
				return it.index();
			}
		}

		ASSERT(0); // bug
		return std::numeric_limits<size_t>::max();
	}

public:


	VXMap(size_t intial_capacity = 16)
		: _the_allocator(Alloc()), _bank(null), _size(0), 
			_empty(std::make_pair(TOMBSTONE(), (mapped_type)0)), _tombstone(std::make_pair(EMPTY(), (mapped_type)0)) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<key_type>::value);
		H_COMPILE_TIME_ASSERT(offsetof(value_type, first) == offsetof(nc_value_type, first));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, second) == offsetof(nc_value_type, second));
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		ASSERT(intial_capacity >= 1);
		intial_capacity = Nearest_Power_Of_2(static_cast<uint32_t>(intial_capacity));
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), intial_capacity);
	}

	VXMap(Alloc const & a_allocator, size_t intial_capacity = 16)
		: _the_allocator(a_allocator), _bank(null), _size(0),
			_empty(std::make_pair(TOMBSTONE(), (mapped_type)0)), _tombstone(std::make_pair(EMPTY(), (mapped_type)0)) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<key_type>::value);
		H_COMPILE_TIME_ASSERT(offsetof(value_type, first) == offsetof(nc_value_type, first));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, second) == offsetof(nc_value_type, second));
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		ASSERT(intial_capacity >= 1);
		intial_capacity = Nearest_Power_Of_2(static_cast<uint32_t>(intial_capacity));
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), intial_capacity);
	}

	VXMap(VXMap const & that)
		: _the_allocator(that.get_allocator()), _bank(null), _size(that._size), _empty(that._empty), _tombstone(that._tombstone) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<key_type>::value);
		H_COMPILE_TIME_ASSERT(offsetof(value_type, first) == offsetof(nc_value_type, first));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, second) == offsetof(nc_value_type, second));
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), that._bank->size());
		auto nit = _bank.begin();
		for (auto it = that._bank->cbegin(), end = that._bank->cend(); it != end; ++it, ++nit) {
			if (it.entry_valid())
				nit = it;
		}
	}

	VXMap(VXMap && that)
		: _the_allocator(that.get_allocator()), _bank(null), _size(that._size), _empty(that._empty), _tombstone(that._tombstone) {
		H_COMPILE_TIME_ASSERT(std::is_scalar<key_type>::value);
		H_COMPILE_TIME_ASSERT(offsetof(value_type, first) == offsetof(nc_value_type, first));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, second) == offsetof(nc_value_type, second));
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		_bank = Construct<Bank_Type>(get_allocator().get_memory_pool(), 16);
		_bank.swap(that._bank);
		that._size = 0;
	}

	~VXMap() {
		clear();
		_bank.reset();
	}

	inline bool empty() const {
		return (_size == 0);
	}

	inline size_t size() const {
		return _size;
	}

	inline void clear() {

		if (!empty()) {
			_bank->ResetAllBanks();
			_size = 0;
			_empty = std::make_pair(TOMBSTONE(), (mapped_type)0);
			_tombstone = std::make_pair(EMPTY(), (mapped_type)0);
		}
	}

	void rehash(size_type n) {

		size_type use_n = Nearest_Power_Of_2(static_cast<uint32_t>(n));
		if (size() < ((use_n >> 1) + (use_n >> 2))) // 75%
			internal_rehash(use_n);
	}

	class const_iterator : public std::iterator<std::forward_iterator_tag, value_type, size_t, value_type const *, value_type const &>
	{
	private:

		VXMap *			_vx;
		nc_value_type *	_special;

		typename Bank_Type::const_iterator	_bank_it;
		typename Bank_Type::const_iterator	_bank_end;

		INLINE void advance_to_item() {

			while (_bank_it != _bank_end) {
				value_type *	item = _bank_it.entry_valid();
				key_type		item_key;
				if (item != null && (item_key = std::get<Key>(*item), ((Special_K_Type)item_key & KEY_MASK_) > TOMBSTONE_))
					break;
				_bank_it.advance();
			}

			if (_bank_it == _bank_end) {
				ASSERT(_special != (nc_value_type const *)~0); // advanced the end

				while (1) { // not a loop

					if (_special == null && std::get<Key>(_vx->_empty) == _vx->EMPTY()) {
						_special = &_vx->_empty;
						break;
					}
					else if ((_special == null || _special == &_vx->_empty) && std::get<Key>(_vx->_tombstone) == _vx->TOMBSTONE()) {
						_special = &_vx->_tombstone;
						break;
					}

					_special = (nc_value_type *)~0;
					break;
				}
			}
		}

		INLINE const_iterator(VXMap const * const vx, bool is_tombstone) : _vx((VXMap *)vx) {

			_bank_end = vx->_bank->cend();
			_bank_it = _bank_end;

			if (is_tombstone) {
				_special = &_vx->_tombstone;
				ASSERT(std::get<Key>(_vx->_tombstone) == _vx->TOMBSTONE());
			}
			else {
				_special = &_vx->_empty;
				ASSERT(std::get<Key>(_vx->_empty) == _vx->EMPTY());
			}
		}

		INLINE const_iterator(size_t index, VXMap const * const vx) : _vx((VXMap *)vx) {

			ASSERT(index < vx->_bank->size());

			_bank_end = vx->_bank->cend();
			_bank_it = vx->_bank->cindex_iterator(index);

			_special = null;
#ifdef HOOPS_DEBUG_BUILD
			value_type *	item = _bank_it.entry_valid();
			key_type		item_key;
			ASSERT(item != null && (item_key = std::get<Key>(*item), ((Special_K_Type)item_key & KEY_MASK_) != 0));
#endif
		}

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type const *	pointer;
		typedef value_type const &	reference;

		INLINE const_iterator() : _vx(null), _bank_it(), _bank_end(), _special(null) {}

		INLINE const_iterator(bool begin, VXMap const * const vx) : _vx((VXMap *)vx) {

			_bank_end = vx->_bank->cend();

			if (begin) {
				_bank_it = vx->_bank->cbegin();
				_special = null;
				advance_to_item();
			}
			else {
				_bank_it = _bank_end;
				_special = (nc_value_type *)~0;
			}
		}

		INLINE reference operator*() const {
			if (_special == null)
				return *_bank_it;
			ASSERT(_special != (nc_value_type *)~0); // dereferenced end
			return *(pointer)_special;
		}

		INLINE pointer operator->() const {
			return &(**this);
		}

		INLINE const_iterator & operator++() {
			++_bank_it;
			advance_to_item();
			return *this;
		}

		INLINE const_iterator operator++(int) {
			const_iterator old_it = *this;
			++(*this);
			return old_it;
		}

		INLINE bool operator ==(const_iterator const & that) const {
			ASSERT(_vx == that._vx);
			return (_special == that._special && _bank_it == that._bank_it && _bank_end == that._bank_end);
		}

		INLINE bool operator !=(const_iterator const & that) const {
			return !(*this == that);
		}

		friend class VXMap;
	};

	class iterator : public const_iterator
	{
	private:
		typedef	const_iterator CI;

		INLINE iterator(bool begin, VXMap const * const vx) : const_iterator(begin, vx) {}

		INLINE iterator(size_t index, VXMap const * const vx) : const_iterator(index, vx) {}

		INLINE iterator(VXMap const * const vx, bool is_tombstone) : const_iterator(vx, is_tombstone) {}

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type *		pointer;
		typedef value_type &		reference;

		INLINE iterator() : const_iterator() {}

		INLINE reference operator*() const {
			if (CI::_special == null)
				return const_cast<reference &>(*CI::_bank_it);
			ASSERT(CI::_special != (nc_value_type const *)~0); // dereferenced end
			return *((value_type *)CI::_special);
		}

		INLINE pointer operator->() const {
			return &(**this);
		}

		INLINE iterator & operator++() {
			const_iterator::operator++();
			return *this;
		}

		INLINE iterator operator++(int) {
			iterator old_it = *this;
			++(*this);
			return old_it;
		}

		friend class VXMap;
	};


	INLINE iterator begin() {
		return iterator(true, this);
	}

	INLINE iterator end() {
		return iterator(false, this);
	}

	INLINE const_iterator begin() const {
		return const_iterator(true, this);
	}

	INLINE const_iterator end() const {
		return const_iterator(false, this);
	}

	INLINE const_iterator cbegin() const {
		return const_iterator(true, this);
	}

	INLINE const_iterator cend() const {
		return const_iterator(false, this);
	}


	INLINE const_iterator find(const key_type & key) const {

		if (key == TOMBSTONE()) {
			if (std::get<Key>(_tombstone) == TOMBSTONE())
				return const_iterator(this, true);
		}
		else if (key == EMPTY()) {
			if (std::get<Key>(_empty) == EMPTY()) {
				return const_iterator(this, false);
			}
		}
		else {
			const value_type * out_element = null;

			size_t index = internal_find(key, out_element);

			if (index != std::numeric_limits<size_t>::max()) {
				ASSERT(key_equal()(std::get<Key>(*out_element), key));
				return const_iterator(index, this);
			}
		}

		return cend();
	}

	INLINE iterator find(const key_type & key) {

		if (key == TOMBSTONE()) {
			if (std::get<Key>(_tombstone) == TOMBSTONE())
				return iterator(this, true);
		}
		else if (key == EMPTY()) {
			if (std::get<Key>(_empty) == EMPTY()) {
				return iterator(this, false);
			}
		}
		else {
			const value_type * out_element = null;

			size_t index = internal_find(key, out_element);

			if (index != std::numeric_limits<size_t>::max()) {
				ASSERT(key_equal()(std::get<Key>(*out_element), key));
				return iterator(index, this);
			}
		}

		return end();
	}

	// bool is true if it was inserted false if it already existed
	INLINE std::pair<iterator, bool> insert(const value_type & in_item) {

		const key_type key = std::get<Key>(in_item);

		if (key == TOMBSTONE()) {
			bool had_key = false;
			if (std::get<Key>(_tombstone) == TOMBSTONE())
				had_key = true;
			else
				++_size;
			_tombstone = in_item;
			return std::make_pair(iterator(this, true), !had_key);
		}
		else if (key == EMPTY()) {
			bool had_key = false;
			if (std::get<Key>(_empty) == EMPTY())
				had_key = true;
			else
				++_size;
			_empty = in_item;
			return std::make_pair(iterator(this, false), !had_key);
		}

		bool had_key = false;
		const value_type * out_element = null;
		size_t	index = internal_insert(in_item, out_element, had_key);
		ASSERT(key_equal()(std::get<Key>(*out_element), key));

		return std::make_pair(iterator(index, this), !had_key);
	}

	INLINE size_t erase(const key_type & key) {

		if (key == EMPTY()) {
			if (std::get<Key>(_empty) == EMPTY()) {
				_empty = std::make_pair(TOMBSTONE(), (mapped_type)0);
				_size--;
				return 1;
			}
		}
		else if (key == TOMBSTONE()) {
			if (std::get<Key>(_tombstone) == TOMBSTONE()) {
				_tombstone = std::make_pair(EMPTY(), (mapped_type)0);
				_size--;
				return 1;
			}
		}
		else {

			const value_type * out_element = null;
			size_t index = internal_find(key, out_element);

			if (out_element != null) {
				ASSERT(key_equal()(std::get<Key>(*out_element), key));
				*((nc_value_type *)out_element) = std::make_pair(TOMBSTONE(), (mapped_type)0);
				_size--;
				if (_size == 0)
					_bank->ResetAllBanks();
				return 1;
			}
			else
				ASSERT(index == std::numeric_limits<size_t>::max());
		}

		return 0;
	}

	INLINE iterator erase(const_iterator position) {

		ASSERT(position._vx == this);

		if (position._special == null) {
			ASSERT(position._bank_it != position._bank_end);
			auto bank_it = _bank->index_iterator(position._bank_it.index());
			reinterpret_cast<nc_value_type &>(*bank_it) = std::make_pair(TOMBSTONE(), (mapped_type)0);
			_size--;
			if (_size == 0) {
				_bank->ResetAllBanks();
				return end();
			}
			bank_it.advance();
			auto bank_end = _bank->end();
			while (bank_it != bank_end) {
				value_type *	item = bank_it.entry_valid();
				key_type		item_key;
				if (item != null && (item_key = std::get<Key>(*item), ((Special_K_Type)item_key & KEY_MASK_) > TOMBSTONE_))
					break;
				bank_it.advance();
			}
			if (bank_it != _bank->end())
				return iterator(bank_it.index(), this);
			else if (std::get<Key>(_empty) == EMPTY())
				return iterator(this, false);
			else if (std::get<Key>(_tombstone) == TOMBSTONE())
				return iterator(this, true);
			return end();
		}
		else if (position._special == &_empty) {
			ASSERT(std::get<Key>(_empty) == EMPTY());
			_empty = std::make_pair(TOMBSTONE(), (mapped_type)0);
			_size--;
			if (_size == 0)
				_bank->ResetAllBanks();
			else if (std::get<Key>(_tombstone) == TOMBSTONE())
				return iterator(this, true);
			return end();
		}
		else if (position._special == &_tombstone) {
			ASSERT(std::get<Key>(_tombstone) == TOMBSTONE());
			_tombstone = std::make_pair(EMPTY(), (mapped_type)0);
			_size--;
			if (_size == 0)
				_bank->ResetAllBanks();
			return end();
		}

		ASSERT(position._special != (nc_value_type const *)~0); // dereferenced end
		return end();
	}


};



#endif



