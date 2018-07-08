

#ifndef UNROLLED_LIST_H
#define UNROLLED_LIST_H

// adapted JB 2013

#ifdef _MSC_VER
#	pragma warning(push)   // Save the current warning state
#	pragma warning(disable:4530)
#endif
#include <iterator>
#ifdef _MSC_VER
#	pragma warning(pop)   // Restore all warnings to the previous state
#endif

template <typename T, class Alloc = std::allocator<T>, int NODE_BYTES=64>
class unrolled_list
{
public:
	typedef Alloc allocator_type;
	inline allocator_type get_allocator() const {return allocator_type(_the_allocator);}

	typedef T		value_type;

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

	struct node
	{
		static const intptr_t values_size = (NODE_BYTES - 2*sizeof(node*) - sizeof(size_t))/sizeof(T);

		node *	next;
		node *	prev;
		size_t	count;
		T		values[values_size];

		node() : next(0), prev(0), count(0) {};
	};

	node *	_head;
	node *	_tail;
	size_t	_size;
	node	_node;

	INLINE void merge_node_with_next(node * merge_node)
	{
		node *			next = merge_node->next;
		const size_t	count = merge_node->count;

		for(size_t i=0; i<next->count; i++)
			merge_node->values[i + count] = next->values[i];

		merge_node->count += next->count;
		merge_node->next = next->next;

		if (merge_node->next == null)
			_tail = merge_node;
		else
			merge_node->next->prev = merge_node;

		freeate((char*)next, sizeof(node));
	}

	void move_object_into_this(unrolled_list& other)
	{
		_the_allocator = other._the_allocator;
		_head = &_node;
		_tail = &_node;
		_size = other._size;
		_node = other._node;

		if(_node.next != null) {
			_node.next->prev = &_node;
		}

		while (_tail->next != null)
			_tail = _tail->next;
	
		other._head = &other._node;
		other._tail = &other._node;
		other._size = 0;
		other._node.next = 0;
		other._node.count = 0;
	}

public:
	class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T, size_t, T const *, T const &>
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef T				value_type;
		typedef std::ptrdiff_t	difference_type;
		typedef T const *		pointer;
		typedef T const &		reference;

		INLINE const_iterator(node * cur_node = null, size_t cur_index = 0) : _curnode(cur_node), _curindex(cur_index) {}

		INLINE reference operator*() {
			return _curnode->values[_curindex];
		}

		INLINE pointer operator->() {
			return &(**this);
		}

		INLINE const_iterator & operator++() {
			_curindex++;
			if (_curindex >= _curnode->count && _curnode->next != null) {
				_curnode = _curnode->next;
				_curindex = 0;
			}
			return *this;
		}

		INLINE const_iterator & operator--() {
			
			if (_curindex == 0) {
				_curnode = _curnode->prev;
				_curindex = _curnode->count - 1;
			}
			else
				_curindex--;
			return *this;
		}

		INLINE const_iterator operator++(int) {
			const_iterator old_it = *this;
			++(*this);
			return old_it;
		}

		INLINE const_iterator operator--(int) {
			const_iterator old_it = *this;
			--(*this);
			return old_it;
		}

		INLINE bool operator ==(const_iterator const & that) {
			return _curnode == that._curnode && _curindex == that._curindex;
		}

		INLINE bool operator !=(const_iterator const & that) {
			return !(*this == that);
		}
	protected:
		node *	_curnode;
		size_t	_curindex;
	};

	class iterator : public const_iterator
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef T		value_type;
		typedef size_t	difference_type;
		typedef T *		pointer;
		typedef T &		reference;

		using const_iterator::_curnode;
		using const_iterator::_curindex;

		INLINE iterator(node * iter_node = null, size_t iter_index = 0) : const_iterator(iter_node, iter_index) {}

		INLINE reference operator*() {
			return _curnode->values[_curindex];
		}

		INLINE pointer operator ->() {
			return &(**this);
		}

		INLINE iterator & operator++() {
			const_iterator::operator++();
			return *this;
		}

		INLINE iterator & operator--() {
			const_iterator::operator--();
			return *this;
		}

		INLINE iterator operator++(int) {
			iterator old_it = *this;
			++(*this);
			return old_it;
		}

		INLINE iterator operator--(int) {
			iterator old_it = *this;
			--(*this);
			return old_it;
		}
	};

	friend class const_iterator;
	friend class iterator;

	INLINE unrolled_list()
		: _the_allocator(Alloc()), _head(&_node), _tail(&_node), _size(0) {

		H_COMPILE_TIME_ASSERT(node::values_size > 0);
		H_COMPILE_TIME_ASSERT(node::values_size < 128); // is that reasonable
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
	}

	INLINE unrolled_list(Alloc const & a_allocator)
		: _the_allocator(a_allocator), _head(&_node), _tail(&_node), _size(0) {

		H_COMPILE_TIME_ASSERT(node::values_size > 0);
		H_COMPILE_TIME_ASSERT(node::values_size < 128); // is that reasonable
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
	}

	INLINE unrolled_list(unrolled_list const & that)
		: _the_allocator(that.get_allocator()), _tail(&_node), _head(&_node), _size(0)
	{	
		H_COMPILE_TIME_ASSERT(node::values_size > 0);
		H_COMPILE_TIME_ASSERT(node::values_size < 128); // is that reasonable
		static_assert(std::is_same<typename Alloc::value_type, value_type>::value, "allocator types mismatched!");
		*this = that;
	}

	INLINE unrolled_list & operator= (unrolled_list const & that)
	{
		clear();

		const_iterator it = that.begin();
		for (it = that.begin(); it != that.end(); ++it)
			push_back(*it);

		return *this;
	}

	INLINE unrolled_list(unrolled_list&& other)
	{
		move_object_into_this(other);
	}

	INLINE unrolled_list & operator=(unrolled_list && other)
	{
		if (_size > 0)
			clear();

		move_object_into_this(other);

		return *this;
	}

	INLINE void clear()
	{
		node *	n_next;
		for (node *	n = _head->next; n != null; n = n_next) {
			n_next = n->next;
			freeate((char*)n, sizeof(node));
		}
		_head->count = 0;
		_head->next = null;
		_tail = _head;
		_size = 0;
	}

	INLINE ~unrolled_list() {
		clear();
	}

	INLINE T & front() {
		ASSERT(!empty());
		return *begin();
	}

	INLINE T & back() {
		ASSERT(!empty());
		return *--end();
	}

	INLINE T const & front() const {
		ASSERT(!empty());
		return *begin();
	}

	INLINE T const & back() const {
		ASSERT(!empty());
		return *--end();
	}

	INLINE iterator begin() {
		return iterator(_head, 0);
	}

	INLINE iterator end() {
		return iterator(_tail, _tail->count);
	}

	INLINE const_iterator begin() const {
		return const_iterator(_head, 0);
	}

	INLINE const_iterator end() const {
		return const_iterator(_tail, _tail->count);
	}

	INLINE const_iterator cbegin() const {
		return const_iterator(_head, 0);
	}

	INLINE const_iterator cend() const {
		return const_iterator(_tail, _tail->count);
	}

	INLINE void push_front(T const & item) {
		insert(begin(), item);
	}

	INLINE void push_back(T const & item) {
		insert(end(), item);
	}

	INLINE void pop_front() {
		erase(begin());
	}

	INLINE void pop_back() {
		erase(--(end()));
	}

	INLINE size_t size() const {
		return _size;
	}

	INLINE bool empty() const {
		return _size == 0;
	}

	inline iterator insert(iterator where, T const & elt)
	{
		const size_t num_values = unrolled_list<T, Alloc, NODE_BYTES>::node::values_size;
		if (where._curnode->count == num_values) { // if node is full

			node *	n = (node *)mallocate(sizeof(node));
			new(n)node();

			if (where._curindex != where._curnode->count) {

				for(size_t i=0; i < (num_values+1)/2; i++)
					n->values[i] = where._curnode->values[i + num_values/2];

				where._curnode->count = num_values/2;
				n->count = (num_values+1)/2;
			}

			n->next = where._curnode->next;
			n->prev = where._curnode;

			if (where._curnode->next != null)
				where._curnode->next->prev = n;

			where._curnode->next = n;

			if (n->next == null)
				_tail = n;

			if (where._curindex >= where._curnode->count) {
				where._curindex -= where._curnode->count;
				where._curnode = n;
			}
		}

		for(size_t i=where._curnode->count; i > where._curindex; i--)
			where._curnode->values[i] = where._curnode->values[i-1];

		where._curnode->values[where._curindex] = elt;
		where._curnode->count++;
		_size++;

		return where;
	}

	INLINE iterator erase(iterator where)
	{
		for(size_t i=where._curindex; i < where._curnode->count - 1; i++)
			where._curnode->values[i] = where._curnode->values[i+1];

		where._curnode->values[where._curnode->count-1] = 0; // DEBUG
		where._curnode->count--;
		_size--;

		const size_t num_values = unrolled_list<T, Alloc, NODE_BYTES>::node::values_size;
		const size_t threshold = num_values >= 5 ? num_values*4/5 : num_values - 1;

		if (where._curnode->prev != null &&
			(where._curnode->count == 0 ||
				where._curnode->count + where._curnode->prev->count <= threshold)) {

			where._curnode = where._curnode->prev;
			where._curindex += where._curnode->count;
			merge_node_with_next(where._curnode);
		}
		else if (where._curnode->next != null &&
					(where._curnode->count == 0 ||
						where._curnode->count + where._curnode->next->count <= threshold)) {

			merge_node_with_next(where._curnode);
		}

		if (where._curindex >= where._curnode->count && where._curnode->next != null) {
			where._curindex -= where._curnode->count;
			where._curnode = where._curnode->next;
		}
		return where;
	}
};

#endif
