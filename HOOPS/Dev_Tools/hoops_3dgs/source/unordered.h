#ifndef UNORDERED_H
#define UNORDERED_H

#include "hoops.h"

#include <cstdio>

#include <vector>
#include <limits>
#include <tuple>
#include <memory>

#ifdef _MSC_VER
#	pragma warning(push)   // Save the current warning state
#	pragma warning(disable:4530)
#endif
#include <iterator>
#ifdef _MSC_VER
#	pragma warning(pop)   // Restore all warnings to the previous state
#endif


template <typename Alloc>
struct Oneway_Allocator_Node {

	typedef Alloc allocator_type;

	Alloc	_the_allocator;

	inline allocator_type get_allocator() {return allocator_type(_the_allocator);}

	struct node_t {
		node_t *		next;
		size_t			size;
		size_t			index;
		size_t			undex;
	};

	node_t *	_node;

	inline Oneway_Allocator_Node(Alloc const & a_allocator) : _the_allocator(a_allocator), _node(0) {}

	inline ~Oneway_Allocator_Node() {
		while(_node != 0) {
			auto next = _node->next;
			ASSERT(_node->undex == _node->index);
			freeate((char*)_node, _node->size);
			_node = next;
		}
	}

	inline void prepare(size_t n) {
		
		size_t		needed_size = n + sizeof(node_t); // XXX rounded
		char *		buffer = mallocate(needed_size);
		node_t *	node = (node_t *)buffer;

		node->next = _node;
		node->size = needed_size;
		node->index = sizeof(node_t); // XXX rounded
		node->undex = sizeof(node_t); // XXX rounded
		_node = node;
	}

	inline char * allocate(size_t n, size_t sizeof_value) {

		size_t needed = n * sizeof_value;
		char * p;
		if (needed + _node->index < _node->size) {
			// XXX rounded
			p = (char *)_node + _node->index;
			_node->index += needed;
		}
		else {
			// not rounded
			p = mallocate(needed);
		}
		return p;
	}


	inline void deallocate(char * p, size_t n, size_t sizeof_value) {

		node_t *	node = _node;
		node_t **	linker = &_node;

		while(node != 0) {
			if (p > (char *)node && p < ((char *)node + node->index)) {
				size_t needed = n * sizeof_value; // XXX rounded
				node->undex += needed;
				if (node->undex == node->index) {
					if (node == _node) {
						node->index = sizeof(node_t); // XXX rounded
						node->undex = sizeof(node_t); // XXX rounded
					}
					else {
						*linker = node->next;
						freeate((char *)node, node->size);
					}
				}
				return;
			}
			linker = &node->next;
			node = node->next;
		}

		freeate(p, n * sizeof_value);
	}

	inline char * mallocate(size_t n) {
		typedef typename Alloc::template rebind<char>::other rebound;
		return rebound(get_allocator()).allocate(n);
	}

	inline void freeate(char * p, size_t n) {
		typedef typename Alloc::template rebind<char>::other rebound;
		return rebound(get_allocator()).deallocate(p, n);
	}
};

template <typename T, typename Alloc>
class Oneway_Allocator
{
public:
	Oneway_Allocator_Node<Alloc> *	node;
	typedef T					value_type;
	typedef value_type *		pointer;
	typedef value_type const *	const_pointer;
	typedef value_type &		reference;
	typedef value_type const &	const_reference;
	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;

	Oneway_Allocator(Oneway_Allocator_Node<Alloc> * node) : node(node) {}

	Oneway_Allocator(Oneway_Allocator<T, Alloc> const & in_that) {
		node = in_that.node;
	}
	~Oneway_Allocator() {}

	template <typename U> Oneway_Allocator(Oneway_Allocator<U, Alloc> const & in_that) {
		node = in_that.node;
	}

	template <typename U>
	struct rebind
	{
		typedef Oneway_Allocator<U, Alloc> other;
	};

	pointer address(reference x) const { return &x; }
	const_pointer address(const_reference x) const { return &x; }

	pointer allocate(size_type size, void * v = 0) {
		UNREFERENCED(v);
		ASSERT(node != null);	
		pointer p = null;
		if (size > 0) {
			p = (pointer)node->allocate(size, sizeof(value_type));
			SET_MEMORY (p, size * sizeof(value_type), 0);
		}
		return p;
	}
	void deallocate(pointer p, size_type n) {

		ASSERT(node != null);
		node->deallocate((char*)p, n, sizeof(value_type));
	}

	//void construct(pointer p, const_reference x) { new(p) T(x); }

	template<typename Arg>
	void construct(T* p, Arg && arg) {
		::new((void *)p) T(std::forward<Arg>(arg));
	}

	void destroy(pointer p) { UNREFERENCED(p); p->~T(); }
 
	size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(T); }

private:
	Oneway_Allocator() {}
};

template <typename T, typename U, typename Alloc>
bool operator==(const Oneway_Allocator<T, Alloc> & a, const Oneway_Allocator<U, Alloc> & b) { return a.node == b.node; }

template <typename T, typename U, typename Alloc>
bool operator!=(const Oneway_Allocator<T, Alloc> & a, const Oneway_Allocator<U, Alloc> & b) { return !(a == b); }



template <typename K, typename T, class Hash = Hasher<K>, class Pred = std::equal_to<K>, class Alloc = std::allocator< std::pair<K,T> > >
class Unordered {

public:

	typedef K key_type;
	typedef T mapped_type;
	typedef std::pair<const K, T> value_type;

private:

	typedef std::pair<K, T> nc_value_type;

	enum {
		Key,
		Item
	};

	typedef std::vector<nc_value_type, Oneway_Allocator<nc_value_type, Alloc>> bucket_type;
	typedef std::vector<bucket_type, Oneway_Allocator<bucket_type, Alloc> > buckets_type;

	struct bucket_placement_type {;
		char x[sizeof(bucket_type)];
	};
	typedef std::vector<bucket_placement_type, Oneway_Allocator<bucket_placement_type, Alloc> > buckets_placement_type;

	Oneway_Allocator_Node<Alloc> _oana;

	char			_buckets_buffer[sizeof(buckets_type)];
	buckets_type *	_buckets;

	size_t _size;
	size_t _n_overflowed;

	static const size_t BUCKET_BYTES = 128;
	static const intptr_t _reserved = BUCKET_BYTES/sizeof(nc_value_type);
	static const size_t _weight = sizeof(bucket_type) + (_reserved * sizeof(nc_value_type));

	inline buckets_type * place_buckets(char * new_buckets_buffer, size_t size) {
		_oana.prepare(_weight*size + sizeof(buckets_type));
		new (new_buckets_buffer) buckets_placement_type(Oneway_Allocator<bucket_placement_type, Alloc>(&_oana));
		buckets_placement_type *	bpt = (buckets_placement_type *)new_buckets_buffer;
		bpt->resize(size);
		for (size_t i=0; i<size; ++i)
			new (&(*bpt)[i]) bucket_type(Oneway_Allocator<nc_value_type, Alloc>(&_oana));
		buckets_type *	new_bucket = (buckets_type *)bpt;
		return new_bucket;
	}

private:
	// XXX need to add this
	Unordered const & operator= (Unordered const & that);

public:

	typedef Hash hasher;
	hasher hash_function() const {return hasher();}

	typedef Pred key_equal;
	key_equal key_eq() const {return key_equal();}

	typedef Alloc allocator_type;
	allocator_type get_allocator() const {return allocator_type();}

	Unordered(size_t initial_buckets=1) : _oana(Alloc()), _size(0), _n_overflowed(0) {
		// maybe we need to increase BUCKET_BYTES or you should not put such a large structure in the hash
		H_COMPILE_TIME_ASSERT(_reserved > 1);
		H_COMPILE_TIME_ASSERT(sizeof(bucket_placement_type) == sizeof(bucket_type));
		H_COMPILE_TIME_ASSERT(sizeof(buckets_placement_type) == sizeof(buckets_type));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, first) == offsetof(nc_value_type, first));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, second) == offsetof(nc_value_type, second));
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		ASSERT(initial_buckets >= 1);
		initial_buckets = Nearest_Power_Of_2(static_cast<uint32_t>(initial_buckets));
		_buckets = place_buckets(_buckets_buffer, initial_buckets);
	}

	Unordered(Alloc const & a_allocator, size_t initial_buckets=1) : _oana(a_allocator), _size(0), _n_overflowed(0) {
		// maybe we need to increase BUCKET_BYTES or you should not put such a large structure in the hash
		H_COMPILE_TIME_ASSERT(_reserved > 1);
		H_COMPILE_TIME_ASSERT(sizeof(bucket_placement_type) == sizeof(bucket_type));
		H_COMPILE_TIME_ASSERT(sizeof(buckets_placement_type) == sizeof(buckets_type));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, first) == offsetof(nc_value_type, first));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, second) == offsetof(nc_value_type, second));
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		ASSERT(initial_buckets >= 1);
		initial_buckets = Nearest_Power_Of_2(static_cast<uint32_t>(initial_buckets));
		_buckets = place_buckets(_buckets_buffer, initial_buckets);
	}

	Unordered(Unordered const & that) : _oana(Alter(that)._oana.get_allocator()), _size(that._size), _n_overflowed(that._n_overflowed)
	{
		H_COMPILE_TIME_ASSERT(_reserved > 1);
		H_COMPILE_TIME_ASSERT(sizeof(bucket_placement_type) == sizeof(bucket_type));
		H_COMPILE_TIME_ASSERT(sizeof(buckets_placement_type) == sizeof(buckets_type));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, first) == offsetof(nc_value_type, first));
		H_COMPILE_TIME_ASSERT(offsetof(value_type, second) == offsetof(nc_value_type, second));
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		ASSERT(that._buckets->size() >= 1);
		size_t n_buckets = that._buckets->size();
		_buckets = place_buckets(_buckets_buffer, n_buckets);
		for (size_t i = 0; i < n_buckets; ++i) {
			bucket_type &	source_bucket = (*that._buckets)[i];
			if (!source_bucket.empty()) {
				bucket_type &	target_bucket = (*_buckets)[i];
				target_bucket.reserve(source_bucket.capacity());
				std::copy(source_bucket.cbegin(), source_bucket.cend(), std::back_inserter(target_bucket));
			}
		}
	}



#if (0)
	Unordered& operator=(Unordered const & other)
	{
		// To be implemented
	}

	Unordered(Unordered && other)
	{
		// To be implemented
	}

	Unordered& operator=(Unordered && other)
	{
		// To be implemented
	}
#endif
	~Unordered() {
		clear();
		_buckets->~buckets_type();
	}

private:

	inline size_t bucket (const K & key, size_t	n_buckets) const {
		Hash	hasher;
		size_t	hashed_key = hasher(key);
		return hashed_key & (n_buckets-1);
	}

	inline bool internal_find(bucket_type const & b, const K & key, size_t & element) const
	{
		const_local_iterator	lit = const_local_iterator(b, 0);
		const_local_iterator	lend = const_local_iterator(b);

		element = 0;
		while (lit != lend) {
			if (key_equal()(lit->first, key))
				return true;
			++element;
			++lit;
		}
		return false;
	}

	template <bool rehashable>
	inline void internal_insert(buckets_type & ibuckets, nc_value_type & data, size_t & index, size_t & element) {

		bucket_type &	b = ibuckets[index];
		
		if (b.capacity() == 0)
			b.reserve(_reserved);

		size_t	n = b.size();

		if (n > _reserved >> 1) {
			++_n_overflowed;
		
			if (rehashable) {
				size_t	n_buckets = ibuckets.size();
				if (_n_overflowed > n_buckets>>2) {
					internal_rehash(data, index, element, n_buckets);
					return;
				}
			}
		}

		element = b.size();
		b.push_back(std::move(data));
		++_size;
	}

	void internal_rehash(nc_value_type & data, size_t & index, size_t & element, size_t n_buckets) {
		size_t new_size = n_buckets * 2;
		if (new_size < 32)
			new_size *= 2;
		rehash(new_size);
		index = bucket(std::get<Key>(data)); // the new index
		internal_insert<false>(*_buckets, data, index, element);
	}


	inline size_t internal_erase(const K & key, size_t index, size_t element, bool multiple) {

		size_t n_removed = 0;

		bucket_type &	b = (*_buckets)[index];

		local_iterator	lit = local_iterator(b, element);
		
		ASSERT(key_equal()(lit->first, key));

		while (1) {
			nc_value_type data = std::move(*lit);
			UNREFERENCED(data); //hmmm might need this to destruct this will work.  evil?
			++n_removed;

			local_iterator	ltail = local_iterator(b, b.size()-1);

			if (lit == ltail) {
				b.pop_back();
				break;
			}

			void * ncv = (void*)&(*ltail);
			nc_value_type & source = *(reinterpret_cast<nc_value_type *>(ncv));
			void * nct = (void*)&(*lit);
			nc_value_type & target = *(reinterpret_cast<nc_value_type *>(nct));
			target = std::move(source);
			//*lit = std::move(*ltail); // in a better world this would work

			b.pop_back();

			if (!multiple)
				break;

			local_iterator	lend = local_iterator(b);

			while (!key_equal()(key, lit->first)) {
				++lit;
				if (lit == lend)
					break;
			}

			if (lit == lend)
				break;
		}

		ASSERT(_size >= n_removed);
		_size -= n_removed;
		return n_removed;
	}

	inline bool internal_equal_range(bucket_type const & b, const K & key, size_t & first_element, size_t & last_element) const
	{
		const_local_iterator	lit = const_local_iterator(b, 0);
		const_local_iterator	lend = const_local_iterator(b);

		first_element = 0;
		while (lit != lend) {
			if (key_equal()(lit->first, key)) {
				last_element = first_element;
				size_t last = first_element;
				++last;
				++lit;
				while (lit != lend) {
					if (key_equal()(lit->first, key))
						last_element = last;
					++last;
					++lit;
				}
				return true;
			}
			++first_element;
			++lit;
		}
		return false;
	}

public:

	class const_local_iterator : public std::iterator<std::forward_iterator_tag, value_type, size_t, value_type const *, value_type const &>
	{

	protected:
		inline const_local_iterator();

		bucket_type const &	_bucket;
		size_t				_element;

	public:
		typedef std::forward_iterator_tag	iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type const *	pointer;
		typedef value_type const &	reference;

		inline const_local_iterator(const_local_iterator const & that) : _bucket(that._bucket), _element(that._element) {}

		inline const_local_iterator(bucket_type const & bucket, size_t element = std::numeric_limits<size_t>::max()) 
			: _bucket(bucket), _element(element) {
			if (_element >= _bucket.size())
				_element = std::numeric_limits<size_t>::max();
		}

		inline reference operator*() const {
			ASSERT(_element < _bucket.size());
			void * ncv = (void*)&_bucket[_element];
			value_type & data = *(reinterpret_cast<value_type *>(ncv));
			return data;
		}

		inline pointer operator->() const {
			return &(**this);
		}

		inline const_local_iterator & operator++() {
			if (_element + 1 >= _bucket.size())
				_element = std::numeric_limits<size_t>::max();
			else
				++_element;
			return *this;
		}

		inline const_local_iterator operator++(int) {
			const_local_iterator old_it = *this;
			++(*this);
			return old_it;
		}

		inline bool operator ==(const_local_iterator const & that) const {
			return &_bucket == &that._bucket && _element == that._element;
		}

		inline bool operator !=(const_local_iterator const & that) const {
			return !(*this == that);
		}

		inline const_local_iterator & operator= (const const_local_iterator & that) {
			_bucket = that._bucket;
			_element = that._element;
			return *this;
		}

	};

	class local_iterator : public const_local_iterator
	{
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef value_type *		pointer;
		typedef value_type &		reference;

		using const_local_iterator::_bucket;
		using const_local_iterator::_element;
		
		inline local_iterator(bucket_type & bucket, size_t element = std::numeric_limits<size_t>::max()) 
			: const_local_iterator(bucket, element) {}

		inline reference operator*() {
			ASSERT(_element < _bucket.size());
// 			value_type & data = const_cast<value_type &>(_bucket[_element]);
// 			return data;
			void * ncv = (void*)&_bucket[_element];
			value_type & data = *(reinterpret_cast<value_type *>(ncv));
			return data;
		}

		inline pointer operator->() {
			return &(**this);
		}

		inline local_iterator & operator++() {
			const_local_iterator::operator++();
			return *this;
		}

		inline local_iterator operator++(int) {
			local_iterator old_it = *this;
			++(*this);
			return old_it;
		}
	};

	class const_iterator : public std::iterator<std::forward_iterator_tag, nc_value_type, size_t, nc_value_type const *, nc_value_type const &>
	{
	protected:
		buckets_type *	_buckets;
		size_t			_index;
		size_t			_element;
		K				_filter;
		bool			_filtered;

		const_iterator & filtered_advance(size_t first_miss, size_t starting_index, bucket_type const &	b) {
			while (1) {
				if (_index != starting_index)
					break;
				if (key_equal()(std::get<Key>(b[_element]), _filter))
					return *this;
				if (first_miss == std::numeric_limits<size_t>::max())
					first_miss = _element;
				++_element;
				if (_element == b.size()) {
					++_index;
					_element = 0;
					adjust_index();
				}
			}
			if (first_miss != std::numeric_limits<size_t>::max()) {
				_index = starting_index;
				_element = first_miss;
			}
			return *this;
		}

		inline void adjust_index() {
			size_t n_buckets = (*_buckets).size();	
			while(_index < n_buckets) {
				bucket_type const &	b = (*_buckets)[_index];
				if (b.size() > _element)
					break;
				_element = 0;
				++_index;
			}
		}

		inline void get(size_t & index, size_t & element) {
			index = _index;
			element = _element;
		}

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef nc_value_type const *	pointer;
		typedef nc_value_type const &	reference;
		
		inline const_iterator() : _buckets(0), _index(0), _element(0), _filtered(false), _filter(K()) {}

		inline const_iterator(buckets_type const * buckets, size_t index, size_t element, bool validate, const K * filter = null)
			: _buckets(Alter(buckets)), _index(index), _element(element), _filter(K()), _filtered(false) {
			if (validate)
				adjust_index();
			if (filter != null) {
				_filtered = true;
				_filter = *filter;
#ifdef HOOPS_DEBUG_BUILD
				bucket_type const &	b = (*_buckets)[_index];
				ASSERT(key_equal()(std::get<Key>(b[_element]), _filter));
#endif
			}
		}

		inline reference operator*() const {
			bucket_type &	b = (*_buckets)[_index];
			local_iterator lit = local_iterator(b, _element);
//			return const_cast<reference>(*lit);
			void * ncv = (void*)&(*lit);
			return *(reinterpret_cast<pointer>(ncv));
		}

		inline pointer operator->() const {
			return &(**this);
		}

		inline const_iterator & operator++() {
			size_t	first_miss = std::numeric_limits<size_t>::max();
			size_t	starting_index = _index;
			bucket_type const &	b = (*_buckets)[_index];
			++_element;
			if (_element == b.size()) {
				++_index;
				_element = 0;
				adjust_index();
			}
			if (_filtered)
				filtered_advance(first_miss, starting_index, b);
			return *this;
		}

		inline const_iterator operator++(int) {
			const_iterator old_it = *this;
			++(*this);
			return old_it;
		}

		inline bool operator ==(const_iterator const & that) const {
			return _buckets == that._buckets && _index == that._index && _element == that._element;
		}

		inline bool operator !=(const_iterator const & that) const {
			return !(*this == that);
		}

		friend class Unordered;
	};

	class iterator : public const_iterator
	{
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t		difference_type;
		typedef nc_value_type *		pointer;
		typedef nc_value_type &		reference;

		using const_iterator::_buckets;
		using const_iterator::_index;
		using const_iterator::_element;

		inline iterator() : const_iterator() {}

		inline iterator(buckets_type * buckets, size_t index, size_t element, bool validate, const K * filter = null)
			: const_iterator(buckets, index, element, validate, filter) {}

		inline reference operator*() {
			bucket_type &	b = (*_buckets)[_index];
			local_iterator lit = local_iterator(b, _element);
			void * ncv = (void*)&(*lit);
			return *(reinterpret_cast<pointer>(ncv));
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

		friend class Unordered;
	};

public:

	inline bool empty() const {
		return (_size == 0);
	}

	inline size_t size() const {
		return _size;
	}

	inline void clear() {

		if (_size > 0 || _n_overflowed > 0)
		{
			char	new_buckets_buffer[sizeof(buckets_type)];
			buckets_type *	new_buckets = place_buckets(new_buckets_buffer, 1);

			_buckets->clear();
			*_buckets = std::move(*new_buckets);

			new_buckets->~buckets_type();

			_size = 0;
			_n_overflowed = 0;
		}
	}

	inline size_t bucket (const K & key) const {
		return bucket(key, _buckets->size());
	}

	inline size_t bucket_count () const {
		return _buckets->size();
	}

	inline local_iterator begin (size_t n) {
		return local_iterator((*_buckets)[n], 0);
	}

	inline local_iterator end (size_t n) {
		return local_iterator((*_buckets)[n]);
	}

	inline const_local_iterator begin (size_t n) const {
		return const_local_iterator((*_buckets)[n], 0);
	}
	
	inline const_local_iterator end (size_t n) const {
		return const_local_iterator((*_buckets)[n]);
	}

	inline const_local_iterator cbegin (size_t n) const {
		return const_local_iterator((*_buckets)[n], 0);
	}

	inline const_local_iterator cend (size_t n) const {
		return const_local_iterator((*_buckets)[n]);
	}

	inline iterator begin() {
		return iterator(_buckets, 0, 0, true);
	}

	inline iterator end() {
		return iterator(_buckets, _buckets->size(), 0, false);
	}

	inline const_iterator begin() const {
		return const_iterator(_buckets, 0, 0, true);
	}

	inline const_iterator end() const {
		return const_iterator(_buckets, _buckets->size(), 0, false);
	}

	inline const_iterator cbegin() const {
		return const_iterator(_buckets, 0, 0, true);
	}

	inline const_iterator cend() const {
		return const_iterator(_buckets, _buckets->size(), 0, false);
	}

	void rehash(size_t new_size) {

		_size = 0;
		_n_overflowed = 0;

		char	new_buckets_buffer[sizeof(buckets_type)];
		buckets_type *	new_buckets = place_buckets(new_buckets_buffer, new_size);

		iterator	b_it = begin();
		iterator	b_end = end();

		for(;b_it != b_end; ++b_it) {
			nc_value_type &		data = *b_it;
			size_t index = bucket(std::get<Key>(data), new_buckets->size());
			size_t element;
			internal_insert<false>(*new_buckets, data, index, element);	
		}

		_buckets->clear();
		*_buckets = std::move(*new_buckets);

		new_buckets->~buckets_type();
	}	

	inline iterator insert (value_type && data) {
		size_t index = bucket(std::get<Key>(data));
		size_t element;
		internal_insert<true>(*_buckets, reinterpret_cast<nc_value_type &>(data), index, element);
		return iterator(_buckets, index, element, false);
	}

	inline iterator find (const K & key) {
		size_t index = bucket(key);
		size_t element;
		if (internal_find((*_buckets)[index], key, element))
			return iterator(_buckets, index, element, false);
		return end();
	}

	inline const_iterator find (const K & key) const {
		size_t index = bucket(key);
		size_t element;
		if (internal_find((*_buckets)[index], key, element))
			return const_iterator(_buckets, index, element, false);
		return cend();
	}

	inline iterator erase (const_iterator it) {

		if (it == end())
			return end();

		size_t index;
		size_t element;
		it.get(index, element);

		size_t n_removed = internal_erase(it->first, index, element, false);
		ASSERT(n_removed == 1);

		return iterator(_buckets, index, element, true);
	}

	inline size_t erase (const K & key) {

		size_t index = bucket(key);
		size_t element;

		if (internal_find((*_buckets)[index], key, element))
			return internal_erase(key, index, element, true);

		return 0;
	}

	std::pair<const_iterator,const_iterator> equal_range (const K & key) const {
		size_t index = bucket(key);

		size_t first_element;
		size_t last_element;

		if (internal_equal_range((*_buckets)[index], key, first_element, last_element)) {
			return std::make_pair(
				const_iterator(_buckets, index, first_element, false, &key),
				const_iterator(_buckets, index, last_element+1, true)); // non inclusive
		}

		const_iterator it = cend();
		return std::make_pair(it, it);
	}

	std::pair<iterator,iterator> equal_range (const K & key) {

		size_t index = bucket(key);

		size_t first_element;
		size_t last_element;

		if (internal_equal_range((*_buckets)[index], key, first_element, last_element)) {
			return std::make_pair(
				iterator(_buckets, index, first_element, false, &key),
				iterator(_buckets, index, last_element+1, true)); // non inclusive
		}

		iterator it = end();
		return std::make_pair(it, it);
	}

};


#endif



