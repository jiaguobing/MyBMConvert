// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HOOPS_MEMORY_DEFINED

#ifdef VALIDATE_MEMORY
template <typename T> struct TypeNameTrait;
#	define	NAME_TYPE(_t_) \
	template <> struct TypeNameTrait<_t_> {static const char * name;}; const char * TypeNameTrait<_t_>::name = #_t_
#	define	HOOPS_MEMORY_FILE __FILE__
#	define	HOOPS_MEMORY_LINE __LINE__
#	define	HOOPS_MEMORY_TYPE(_t_) (#_t_)
#else
#	define	NAME_TYPE(_t_) 
#	define	HOOPS_MEMORY_FILE 0
#	define	HOOPS_MEMORY_LINE 0
#	define	HOOPS_MEMORY_TYPE(_t_) 0
#endif

INLINE bool USING_RAW_MEMORY() {return HOOPS::ETERNAL_WORLD->using_raw_memory;};
INLINE Memory_Pool * DEFAULT_MEMORY_POOL() {return HOOPS::ETERNAL_WORLD->memory_pool;}
INLINE Malloc_Action DEFAULT_MALLOC() {return HOOPS::ETERNAL_WORLD->malloc;}
INLINE Free_Action DEFAULT_FREE() {return HOOPS::ETERNAL_WORLD->free;}


GLOBAL_UTILITY_FUNCTION void HUI_Free_Array(
	void *			p, 
	const char *	file, 
	int				line);

GLOBAL_UTILITY_FUNCTION void* HUI_Alloc_Array(
	size_t			size, 
	bool			try_alloc, 
	bool			cache_big_blocks, 
	Memory_Pool *	mempool, 
	const char *	type_name, 
	const char *	file, 
	int				line);

GLOBAL_UTILITY_FUNCTION void * HUI_System_Alloc (size_t size);
GLOBAL_UTILITY_FUNCTION void HUI_System_Free (void *pntr);

GLOBAL_FUNCTION Memory_Pool * HI_Create_Memory_Pool(char const * id);
GLOBAL_FUNCTION Memory_Pool * HI_Create_Main_Memory_Pool(char const * id);

GLOBAL_FUNCTION bool HI_Destroy_Memory_Pool(Memory_Pool * pool);

GLOBAL_FUNCTION void HI_Use_Memory_Task(bool use_one);

GLOBAL_FUNCTION void HI_Set_System_Memory_Functions();
GLOBAL_FUNCTION void HI_Show_Memory_Usage(size_t & allocated, size_t & used);

GLOBAL_FUNCTION size_t HI_Memory_On_Pool (Memory_Pool *memory_pool);

#define	H_ALLOC_ARRAY(_p, _size, _type, _trying, _cache_big_blocks, _mempool, _type_name, _file, _line) do { \
		size_t _size_ = _size; \
		if (USING_RAW_MEMORY()) { \
			_p = (_type *)(*DEFAULT_MALLOC())(_size_); \
		} else { \
			_p = (_type *)HUI_Alloc_Array(_size_, _trying, _cache_big_blocks, _mempool, _type_name, _file, _line); \
		} \
	} while (0)

#define H_FREE_ARRAY(_p, _file, _line) do { \
	ASSERT(_p != 0); \
	if (USING_RAW_MEMORY()) { \
		(*DEFAULT_FREE())((void*)_p); \
	} else { \
		HUI_Free_Array((void*)_p, _file, _line); \
	} \
} while (0)


#define FREE(p, _type_) \
	H_FREE_ARRAY (p, HOOPS_MEMORY_FILE,HOOPS_MEMORY_LINE)
#define FREE_ARRAY(p, count, _type_) do { \
	H_FREE_ARRAY (p, HOOPS_MEMORY_FILE,HOOPS_MEMORY_LINE); \
	UNREFERENCED(count); \
} while(0)

#define ALLOC(p, _type_) \
	H_ALLOC_ARRAY (p, sizeof(_type_), _type_, false, false, DEFAULT_MEMORY_POOL(), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define ALLOC_ARRAY(p, count, _type_) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), _type_, false, false, DEFAULT_MEMORY_POOL(), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define TRY_ALLOC_ARRAY(p, count, _type_) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), _type_, true, false, DEFAULT_MEMORY_POOL(), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)

// Use only for draw time temps not database
#define ALLOC_CACHED(p, _type_) \
	H_ALLOC_ARRAY (p, sizeof(_type_), _type_, false, true, DEFAULT_MEMORY_POOL(), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define ALLOC_ARRAY_CACHED(p, count, _type_) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), _type_, false, true, DEFAULT_MEMORY_POOL(), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define TRY_ALLOC_ARRAY_CACHED(p, count, _type_) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), _type_, true, true, DEFAULT_MEMORY_POOL(), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)

#define POOL_ALLOC(p, _type_, mempool) \
	H_ALLOC_ARRAY (p, sizeof(_type_), _type_, false, false, mempool, HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define POOL_ALLOC_ARRAY(p, count, _type_, mempool) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), _type_, false, false, mempool, HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define POOL_TRY_ALLOC_ARRAY(p, count, _type_, mempool) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), HOOPS_MEMORY_TYPE(_type_), _type_, true, false, mempool, HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)

#define POOL_ALLOC_CACHED(p, _type_, mempool) \
	H_ALLOC_ARRAY (p, sizeof(_type_), _type_, false, true, mempool, HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define POOL_ALLOC_ARRAY_CACHED(p, count, _type_, mempool) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), _type_, false, true, mempool, HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)
#define POOL_TRY_ALLOC_ARRAY_CACHED(p, count, _type_, mempool) \
	H_ALLOC_ARRAY (p, (count)*sizeof(_type_), _type_, true, true, mempool, HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE)



#define	ZALLOC(_dst_, _type_)	do {					\
			ALLOC (_dst_, _type_);						\
			ZERO_STRUCT ((void *)_dst_, _type_);		\
		} while (0)
#define	ZALLOC_ARRAY(_dst_, _cnt_, _type_)	do {		\
			ALLOC_ARRAY (_dst_, _cnt_, _type_);			\
			ZERO_ARRAY ((void *)_dst_, _cnt_, _type_);	\
		} while (0)

#define	ZALLOC_CACHED(_dst_, _type_)	do {					\
			ALLOC_CACHED (_dst_, _type_);						\
			ZERO_STRUCT ((void *)_dst_, _type_);		\
		} while (0)
#define	ZALLOC_ARRAY_CACHED(_dst_, _cnt_, _type_)	do {		\
			ALLOC_ARRAY_CACHED (_dst_, _cnt_, _type_);			\
			ZERO_ARRAY ((void *)_dst_, _cnt_, _type_);	\
		} while (0)


#define	POOL_ZALLOC(_dst_, _type_, _pool_)	do {					\
			POOL_ALLOC (_dst_, _type_, _pool_);						\
			ZERO_STRUCT ((void *)_dst_, _type_);		\
		} while (0)
#define	POOL_ZALLOC_ARRAY(_dst_, _cnt_, _type_, _pool_)	do {		\
			POOL_ALLOC_ARRAY (_dst_, _cnt_, _type_, _pool_);			\
			ZERO_ARRAY ((void *)_dst_, _cnt_, _type_);	\
		} while (0)

#define	POOL_ZALLOC_CACHED(_dst_, _type_, _pool_)	do {					\
			POOL_ALLOC_CACHED (_dst_, _type_, _pool_);						\
			ZERO_STRUCT ((void *)_dst_, _type_);		\
		} while (0)

#define	POOL_ZALLOC_ARRAY_CACHED(_dst_, _cnt_, _type_, _pool_)	do {		\
			POOL_ALLOC_ARRAY_CACHED (_dst_, _cnt_, _type_, _pool_);			\
			ZERO_ARRAY ((void *)_dst_, _cnt_, _type_);	\
		} while (0)

/* used to derive a 16-byte aligned address (usually to select a safe address from within a larger buffer) */
#define	ALIGNED_PTR_16(x)	(((uintptr_t)(x) + 15) & ~(uintptr_t)0x00F)


class HOOPS_UTILITY_PRIVATE_API CMO {
public:
	virtual ~CMO() {}

	INLINE void * operator new (size_t size, const char* type_name, const char* file, int line) {
		void * p;
		H_ALLOC_ARRAY (p, size,  unsigned char, false, false, DEFAULT_MEMORY_POOL(), type_name, file, line);
		SET_MEMORY (p, size, 0);
		return p;
	}

	INLINE void operator delete (void * p, const char* type_name, const char* file, int line) {
		UNREFERENCED(type_name);
		H_FREE_ARRAY(p, file, line);
	}

	INLINE void operator delete (void * p, size_t size) throw () {
		UNREFERENCED(size);
		H_FREE_ARRAY(p, HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE);
	}

private:
	static void *	operator new [] (size_t size);
	static void *	operator new (size_t size);
};

// use only for draw time temps like renditions, not for database
class HOOPS_UTILITY_PRIVATE_API CMO_Pooled {
public:
	virtual ~CMO_Pooled() {}

	INLINE void * operator new (size_t size, Memory_Pool * pool, const char* _type_, const char* file, int line) {
		void * p;
		H_ALLOC_ARRAY (p, size, unsigned char, false, true, pool, _type_, file, line);
		SET_MEMORY (p, size, 0);
		return p;
	}

	INLINE void * operator new (size_t size, void * p) {
		SET_MEMORY (p, size, 0);
		return p;
	}

	INLINE void operator delete (void * p, void *) throw () {
		UNREFERENCED(p);
		ASSERT(0);
	}

	INLINE void operator delete (void * p, Memory_Pool * pool, const char* _type_, const char* file, int line) {
		UNREFERENCED(_type_);
		UNREFERENCED(pool);
		H_FREE_ARRAY(p, file, line);
	}

	INLINE void operator delete (void * p, size_t size) throw () {
		UNREFERENCED(size);
		H_FREE_ARRAY(p, HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE);
	}

private:
	static void *	operator new [] (size_t size);
	static void *	operator new (size_t size);
};

// use only for draw time temps like renditions, not for database
class HOOPS_UTILITY_PRIVATE_API CMO_Pooled_Copyable : public CMO_Pooled {
public:
	Memory_Pool *	memory_pool;

	CMO_Pooled_Copyable (Memory_Pool * pool) : memory_pool (pool) {}
	CMO_Pooled_Copyable (CMO_Pooled_Copyable const & x) : memory_pool (x.memory_pool) {}
	virtual ~CMO_Pooled_Copyable() {}

private:
	CMO_Pooled_Copyable();
	CMO_Pooled_Copyable const & operator= (CMO_Pooled_Copyable const & that);
};

class HOOPS_UTILITY_PRIVATE_API CMO_Cached {
public:
	virtual ~CMO_Cached() {}

	INLINE void * operator new (size_t size, const char* type_name, const char* file, int line) {
		void * p;
		H_ALLOC_ARRAY (p, size,  unsigned char, false, true, DEFAULT_MEMORY_POOL(), type_name, file, line);
		SET_MEMORY (p, size, 0);
		return p;
	}

	INLINE void operator delete (void * p, const char* type_name, const char* file, int line) {
		UNREFERENCED(type_name);
		H_FREE_ARRAY(p, file, line);
	}

	INLINE void operator delete (void * p, size_t size) throw () {
		UNREFERENCED(size);
		H_FREE_ARRAY(p, HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE);
	}

private:
	static void *	operator new [] (size_t size);
	static void *	operator new (size_t size);
};




#	define NEW(_type_) new (HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE) _type_
#	define POOL_NEW(_pool_, _type_) new ((_pool_), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE) _type_
#	define DEFAULT_POOL_NEW(_type_) new (DEFAULT_MEMORY_POOL(), HOOPS_MEMORY_TYPE(_type_), HOOPS_MEMORY_FILE, HOOPS_MEMORY_LINE) _type_

#define SYSTEM_NEW new

template<typename T>
class Auto_Buffer {
public:
	T *		buffer;
	size_t	size;

	INLINE Auto_Buffer(size_t size, Memory_Pool * memory_pool=0):buffer(0), size(size) {
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		if (size > 0)
			POOL_ALLOC_ARRAY_CACHED(buffer, size, T, memory_pool);
	}

	INLINE ~Auto_Buffer() {
		if (size > 0)
			FREE_ARRAY(buffer, size, T);
	}

	INLINE T const *	operator-> () const	{ return buffer; }
	INLINE T *			operator-> () { return buffer; }

	INLINE operator T * ()	{ return buffer; }
};

class ErrBuffer : public Auto_Buffer<char> {
public:
	ErrBuffer():Auto_Buffer(SPRINTF_BUFFER_SIZE) {};
};

//#define STD_IS_SAME_SUPPORTED

#ifdef STD_IS_SAME_SUPPORTED
#define AUTO_BUFFERS_ROUND_TO_ALIGNMENT(_size) (((_size) + (1<<HOOPS_MEMORY_ALIGNMENT) - 1) & ~((1<<HOOPS_MEMORY_ALIGNMENT) - 1))
#define AUTO_BUFFERS_SIZER(_N_) if(!std::is_same<T##_N_,AS_Null_Type>::value) { \
	rsize##_N_ = AUTO_BUFFERS_ROUND_TO_ALIGNMENT(sizeof(T##_N_ ) * size##_N_); size += rsize##_N_; }
#define AUTO_BUFFERS_ASSIGNER(_N_) if(!std::is_same<T##_N_,AS_Null_Type>::value) { \
	if (rsize##_N_ > 0) {*t##_N_ = (T##_N_*)p; p += rsize##_N_;} else {t##_N_ = null;} }
#define AUTO_BUFFERS_ALIGNMENT_ASSERT(_N_) if(!std::is_same<T##_N_,AS_Null_Type>::value) { \
	ASSERT(((intptr_t)t##_N_ & ((1<<HOOPS_MEMORY_ALIGNMENT) - 1)) == 0); } // bad alignment
#else
#define AUTO_BUFFERS_ROUND_TO_ALIGNMENT(_size) (((_size) + (1<<HOOPS_MEMORY_ALIGNMENT) - 1) & ~((1<<HOOPS_MEMORY_ALIGNMENT) - 1))
#define AUTO_BUFFERS_SIZER(_N_) if (t##_N_ != null) { \
	rsize##_N_ = AUTO_BUFFERS_ROUND_TO_ALIGNMENT(sizeof(T##_N_ ) * size##_N_); size += rsize##_N_; }
#define AUTO_BUFFERS_ASSIGNER(_N_) if (t##_N_ != null) { \
	if (rsize##_N_ > 0) {*t##_N_ = (T##_N_*)p; p += rsize##_N_;} else {*t##_N_ = null;} }
#define AUTO_BUFFERS_ALIGNMENT_ASSERT(_N_) if (t##_N_ != null) { \
	ASSERT(((intptr_t)*t##_N_ & ((1<<HOOPS_MEMORY_ALIGNMENT) - 1)) == 0); } // bad alignment
#endif

struct AS_Null_Type {
	;
};

template <typename T0, typename T1,
	typename T2=AS_Null_Type, typename T3=AS_Null_Type, 
	typename T4=AS_Null_Type, typename T5=AS_Null_Type,
	typename T6=AS_Null_Type, typename T7=AS_Null_Type,
	typename T8=AS_Null_Type, typename T9=AS_Null_Type,
	typename T10=AS_Null_Type, typename T11=AS_Null_Type>
class Auto_Slabber {
public:
	static INLINE void slabber(
		Memory_Pool *		memory_pool,
		bool				clear_memory,
		unsigned char *&	buffer,
		size_t &			size,
		T0 ** t0,			size_t size0,
		T1 ** t1,			size_t size1,
		T2 ** t2=0,			size_t size2=0,
		T3 ** t3=0,			size_t size3=0, 
		T4 ** t4=0,			size_t size4=0,
		T5 ** t5=0,			size_t size5=0, 
		T6 ** t6=0,			size_t size6=0, 
		T7 ** t7=0,			size_t size7=0,
		T8 ** t8=0,			size_t size8=0,
		T9 ** t9=0,			size_t size9=0,
		T10 ** t10=0,		size_t size10=0,
		T11 ** t11=0,		size_t size11=0
		)
	{
		size_t	rsize0 = 0;
		size_t	rsize1 = 0;
		size_t	rsize2 = 0;
		size_t	rsize3 = 0;
		size_t	rsize4 = 0;
		size_t	rsize5 = 0;
		size_t	rsize6 = 0;
		size_t	rsize7 = 0;
		size_t	rsize8 = 0;
		size_t	rsize9 = 0;
		size_t	rsize10 = 0;
		size_t	rsize11 = 0;

		AUTO_BUFFERS_SIZER(0);
		AUTO_BUFFERS_SIZER(1);
		AUTO_BUFFERS_SIZER(2);
		AUTO_BUFFERS_SIZER(3);
		AUTO_BUFFERS_SIZER(4);
		AUTO_BUFFERS_SIZER(5);
		AUTO_BUFFERS_SIZER(6);
		AUTO_BUFFERS_SIZER(7);
		AUTO_BUFFERS_SIZER(8);
		AUTO_BUFFERS_SIZER(9);
		AUTO_BUFFERS_SIZER(10);
		AUTO_BUFFERS_SIZER(11);

		ASSERT(size>0);
		if (size>0) {
			if (clear_memory)
				POOL_ZALLOC_ARRAY_CACHED(buffer, size, unsigned char, memory_pool);
			else
				POOL_ALLOC_ARRAY_CACHED(buffer, size, unsigned char, memory_pool);
		}
		unsigned char *	p = buffer;

		AUTO_BUFFERS_ASSIGNER(0);
		AUTO_BUFFERS_ASSIGNER(1);
		AUTO_BUFFERS_ASSIGNER(2);
		AUTO_BUFFERS_ASSIGNER(3);
		AUTO_BUFFERS_ASSIGNER(4);
		AUTO_BUFFERS_ASSIGNER(5);
		AUTO_BUFFERS_ASSIGNER(6);
		AUTO_BUFFERS_ASSIGNER(7);
		AUTO_BUFFERS_ASSIGNER(8);
		AUTO_BUFFERS_ASSIGNER(9);
		AUTO_BUFFERS_ASSIGNER(10);
		AUTO_BUFFERS_ASSIGNER(11);

		AUTO_BUFFERS_ALIGNMENT_ASSERT(0);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(1);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(2);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(3);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(4);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(5);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(6);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(7);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(8);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(9);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(10);
		AUTO_BUFFERS_ALIGNMENT_ASSERT(11);
	}
};


class Auto_Buffers {
public:
	unsigned char *	buffer;
	size_t			size;

	template <typename T0, typename T1>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1>::slabber(memory_pool, clear_memory, buffer, size, &t0, size0, &t1, size1);
	}

	template <typename T0, typename T1, typename T2>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2>::slabber(memory_pool, clear_memory, buffer, size, &t0, size0, &t1, size1, &t2, size2);
	}

	template <typename T0, typename T1, typename T2, typename T3>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3>::slabber(memory_pool, clear_memory, buffer, size, &t0, size0, &t1, size1, &t2, size2, &t3, size3);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3,
		T4 *& t4, size_t size4,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4>::slabber(memory_pool, clear_memory, buffer, size, &t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3,
		T4 *& t4, size_t size4,
		T5 *& t5, size_t size5,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4, T5>::slabber(memory_pool, clear_memory, buffer, size, &t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4, &t5, size5);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3,
		T4 *& t4, size_t size4,
		T5 *& t5, size_t size5,
		T6 *& t6, size_t size6,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4, T5, T6>::slabber(memory_pool, clear_memory, buffer, size, &t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4, &t5, size5, &t6, size6);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3,
		T4 *& t4, size_t size4,
		T5 *& t5, size_t size5,
		T6 *& t6, size_t size6, 
		T7 *& t7, size_t size7,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4, T5, T6, T7>::slabber(memory_pool, clear_memory, buffer, size, 
			&t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4, &t5, size5, &t6, size6, &t7, size7);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3,
		T4 *& t4, size_t size4,
		T5 *& t5, size_t size5,
		T6 *& t6, size_t size6, 
		T7 *& t7, size_t size7,
		T8 *& t8, size_t size8,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4, T5, T6, T7, T8>::slabber(memory_pool, clear_memory, buffer, size, 
			&t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4, &t5, size5, &t6, size6, &t7, size7, &t8, size8);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3, 
		T4 *& t4, size_t size4,
		T5 *& t5, size_t size5, 
		T6 *& t6, size_t size6, 
		T7 *& t7, size_t size7,
		T8 *& t8, size_t size8,
		T9 *& t9, size_t size9,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::slabber(memory_pool, clear_memory, buffer, size, 
			&t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4, &t5, size5, &t6, size6, &t7, size7, &t8, size8, &t9, size9);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3, 
		T4 *& t4, size_t size4,
		T5 *& t5, size_t size5, 
		T6 *& t6, size_t size6, 
		T7 *& t7, size_t size7,
		T8 *& t8, size_t size8,
		T9 *& t9, size_t size9,
		T10 *& t10, size_t size10,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::slabber(memory_pool, clear_memory, buffer, size, 
			&t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4, &t5, size5, &t6, size6, &t7, size7, &t8, size8,
			&t9, size9, &t10, size10);
	}

	template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
	INLINE Auto_Buffers(
		T0 *& t0, size_t size0,
		T1 *& t1, size_t size1,
		T2 *& t2, size_t size2,
		T3 *& t3, size_t size3, 
		T4 *& t4, size_t size4,
		T5 *& t5, size_t size5, 
		T6 *& t6, size_t size6, 
		T7 *& t7, size_t size7,
		T8 *& t8, size_t size8,
		T9 *& t9, size_t size9,
		T10 *& t10, size_t size10,
		T11 *& t11, size_t size11,
		Memory_Pool *	memory_pool = 0,
		bool			clear_memory = false
		) : size(0)
	{
		if (memory_pool == 0) memory_pool = DEFAULT_MEMORY_POOL();
		Auto_Slabber<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::slabber(memory_pool, clear_memory, buffer, size, 
			&t0, size0, &t1, size1, &t2, size2, &t3, size3, &t4, size4, &t5, size5, &t6, size6, &t7, size7, &t8, size8,
			&t9, size9, &t10, size10, &t11, size11);
	}

	~Auto_Buffers() {
		if (size > 0)
			FREE_ARRAY(buffer, size, unsigned char);
	}

};


template <typename T>
class CMO_Allocator
{
public:
	INLINE Memory_Pool *		get_memory_pool() const {return DEFAULT_MEMORY_POOL();}
	typedef T					value_type;
	typedef value_type *		pointer;
	typedef value_type const *	const_pointer;
	typedef value_type &		reference;
	typedef value_type const &	const_reference;
	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;

	CMO_Allocator() {}
	CMO_Allocator(CMO_Allocator<T> const & in_that) { UNREFERENCED(in_that); }
	~CMO_Allocator() {}

	template <typename U> CMO_Allocator(CMO_Allocator<U> const &) {}

	template <typename U>
	struct rebind
	{
		typedef CMO_Allocator<U> other;
	};

	pointer address(reference x) const { return &x; }
	const_pointer address(const_reference x) const { return &x; }

	pointer allocate(size_type size, void * v = 0) {
		UNREFERENCED(v);
		Memory_Pool *	memory_pool = DEFAULT_MEMORY_POOL();
		pointer p = null;
		if (size > 0) {
			POOL_ALLOC_ARRAY_CACHED(p, size, T, memory_pool);
			SET_MEMORY (p, size, 0);
		}
		return p;
	}
	void deallocate(pointer p, size_type n) {
		UNREFERENCED(n);
		H_FREE_ARRAY(p, __FILE__, __LINE__);
	}
	
#ifdef OLD_GCC    
	template<typename... Args>
	void construct(T* p, Args&&... args) {
		::new((void *)p) T(std::forward<Args>(args)...);
	}
#else
	template<typename U, typename Arg>
	void construct(U* p, Arg && arg) {
		::new((void *)p) U(std::forward<Arg>(arg));
	}
#endif

	void destroy(pointer p) { UNREFERENCED(p); p->~T(); }

	size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); }
};

template <typename T, typename U>
bool operator==(const CMO_Allocator<T> &, const CMO_Allocator<U> &) { return true; }

template <typename T, typename U>
bool operator!=(const CMO_Allocator<T> &, const CMO_Allocator<U> &) { return false; }



template <typename T>
class POOL_Allocator
{
public:
	INLINE Memory_Pool *		get_memory_pool() const {return memory_pool;}
	Memory_Pool *				memory_pool;
	typedef T					value_type;
	typedef value_type *		pointer;
	typedef value_type const *	const_pointer;
	typedef value_type &		reference;
	typedef value_type const &	const_reference;
	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;

	POOL_Allocator(Memory_Pool * memory_pool) : memory_pool(memory_pool) {}

	POOL_Allocator(POOL_Allocator<T> const & in_that) {
		memory_pool = in_that.memory_pool;
	}
	~POOL_Allocator() {}

	template <typename U> POOL_Allocator(POOL_Allocator<U> const & in_that) {
		memory_pool = in_that.memory_pool;
	}

	template <typename U>
	struct rebind
	{
		typedef POOL_Allocator<U> other;
	};

	pointer address(reference x) const { return &x; }
	const_pointer address(const_reference x) const { return &x; }

	pointer allocate(size_type size, void * v = 0) {
		UNREFERENCED(v);
		pointer p = null;
		if (size > 0) {
			POOL_ALLOC_ARRAY_CACHED(p, size, T, memory_pool);
			SET_MEMORY (p, size, 0);
		}
		return p;
	}
	void deallocate(pointer p, size_type n) {
		UNREFERENCED(n);
		H_FREE_ARRAY(p, __FILE__, __LINE__);
	}

	//void construct(pointer p, const_reference x) { new(p) T(x); }

#ifdef OLD_GCC    
	template<typename... Args>
	void construct(T* p, Args&&... args) {
		::new((void *)p) T(std::forward<Args>(args)...);
	}

#else
    template<typename Arg>
    void construct(T* p, Arg && arg) {
		::new((void *)p) T(std::forward<Arg>(arg));
    }
#endif

	void destroy(pointer p) { UNREFERENCED(p); p->~T(); }

	size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); }

private:
	POOL_Allocator() {}
};

template <typename T, typename U>
bool operator==(const POOL_Allocator<T> & a, const POOL_Allocator<U> & b) { return (a.memory_pool == b.memory_pool); }

template <typename T, typename U>
bool operator!=(const POOL_Allocator<T> & a, const POOL_Allocator<U> & b) { return !(a == b); }


template <typename T>
inline
void Destruct(T *& item)
{
	if (item != null) {
		item->~T();
		FREE(item, T);
		item = null;
	}
}

template<typename T>
struct Destruct_Deleter
{
	void operator()(T * item) const
	{
		ASSERT(item != null);
		Destruct (item);
	}
};

template <typename T>
struct Destructible {
	typedef std::unique_ptr<T, Destruct_Deleter<T>> unique_ptr;
};

template <typename T>
inline typename Destructible<T>::unique_ptr Construct(T const & that)
{
	T * item;
	ALLOC(item, T);
	new(item)T(that);
	return typename Destructible<T>::unique_ptr(item);
}

template <typename T>
inline typename Destructible<T>::unique_ptr Construct()
{
	T * item;
	POOL_ALLOC(item, T, DEFAULT_MEMORY_POOL());
	new(item)T();
	return typename Destructible<T>::unique_ptr(item);
}

// if 2012 supported VA templates this would be one
template <typename T, typename U>
inline typename std::enable_if<std::is_same<T, U>::value == false, typename Destructible<T>::unique_ptr>::type
Construct(U const & args)
{
	T * item;
	POOL_ALLOC(item, T, DEFAULT_MEMORY_POOL());
	new(item)T(args);
	return typename Destructible<T>::unique_ptr(item);
}

template <typename T>
inline typename Destructible<T>::unique_ptr Construct(Memory_Pool *	memory_pool)
{
	T * item;
	POOL_ALLOC(item, T, memory_pool);
	new(item)T(POOL_Allocator<typename T::value_type>(memory_pool));
	return typename Destructible<T>::unique_ptr(item);
}

// if 2012 supported VA templates this would be one
template <typename T, typename U>
inline typename Destructible<T>::unique_ptr Construct(Memory_Pool *	memory_pool, U const & args)
{
	T * item;
	POOL_ALLOC(item, T, memory_pool);
	new(item)T(POOL_Allocator<typename T::value_type>(memory_pool), args);
	return typename Destructible<T>::unique_ptr(item);
}



#define HOOPS_MEMORY_DEFINED
#endif








