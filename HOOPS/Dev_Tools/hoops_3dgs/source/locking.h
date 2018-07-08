// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef LOCKING_DEFINED
#define LOCKING_DEFINED

End_HOOPS_Namespace

#if defined(CPP11_THREADS)

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4530)	// C++ exception handler used, but unwind semantics are not enabled.
#endif
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

INLINE void MILLI_SECOND_SLEEP(Time milli) {
	long long m = (long long)milli;
	std::this_thread::sleep_for(std::chrono::milliseconds(m));
}

#	define THREAD_YIELD()	std::this_thread::yield()

#elif defined(_MSC_VER) && !defined(NUTCRACKER)
#	include <time.h>
#	define MILLI_SECOND_SLEEP(_x_) ::Sleep((int)(_x_))
#	define THREAD_YIELD() MILLI_SECOND_SLEEP(1)
#else
#	ifndef __USE_POSIX199309
#		define __USE_POSIX199309
#	endif
#	include <time.h>
#	define MILLI_SECOND_SLEEP(_x_) do {					\
		struct timespec		req;						\
		unsigned long		_xx_ = (unsigned long)_x_;	\
		req.tv_sec = _xx_ / 1000;						\
		req.tv_nsec = ((_xx_ % 1000) * 1000000);		\
		nanosleep (&req,0);								\
	} while (0);
#	define THREAD_YIELD() MILLI_SECOND_SLEEP(1)
#endif

//#define DEBUG_LOCKING_TRACE
#ifdef DEBUG_LOCKING_TRACE
extern char **	debug_ringbuffers;
extern int		debug_ringbuffer_index;
#	define	_DEBUG_TRACE(_tag_,_id_,_synch_) (void)_tag_,(void)_id_,(void)_synch_
#else
#	define	_DEBUG_TRACE(_tag_,_id_,_synch_) (void)_tag_,(void)_id_,(void)_synch_
#endif

#if defined(CPP11_THREADS)

INLINE HThreadID THREAD_ID() {
	static_assert(sizeof(std::thread::id) == sizeof(HThreadID), "problems with std::thread::id size");
	return std::this_thread::get_id();
}

Begin_HOOPS_Namespace

struct System_Thread {

	char	_thrd[sizeof(std::thread)];

	INLINE void create(void(*func)(void *), void * data) {
		new (&_thrd[0]) std::thread(func, data);
	}
	INLINE void join() {
		((std::thread *)_thrd)->join();
		((std::thread *)_thrd)->~thread();
	}
	INLINE void detach() {
		((std::thread *)_thrd)->detach();
		((std::thread *)_thrd)->~thread();
	}
};

struct System_Semaphore {

	char	_cv[sizeof(std::condition_variable)];
	char	_mtx[sizeof(std::mutex)];
	int		count;

	INLINE void create(int initial_count) {
		new (&_cv[0]) std::condition_variable();
		new (&_mtx[0]) std::mutex();
		count = initial_count;
	}
	INLINE void destroy() {
		((std::condition_variable *)_cv)->~condition_variable();
	}
	INLINE void release(int in_count) {
		((std::mutex *)_mtx)->lock();
		count += in_count;
		int current_count = count;
		((std::mutex *)_mtx)->unlock();
		while (current_count-- > 0)
			((std::condition_variable *)_cv)->notify_one();
	}
	INLINE void wait() {
		std::unique_lock<std::mutex> lck(*((std::mutex *)_mtx));
		while (count == 0) {
			((std::condition_variable *)_cv)->wait(lck);
		}
		count--;
	}
};

struct System_Mutex {

	char	_mtx[sizeof(std::mutex)];

	INLINE void create() {
		new (&_mtx[0]) std::mutex();
	}
	INLINE void destroy() {
	}
	INLINE void lock() {
		((std::mutex *)_mtx)->lock();
	}
	INLINE void unlock() {
		((std::mutex *)_mtx)->unlock();
	}
	INLINE bool try_lock() {
		return ((std::mutex *)_mtx)->try_lock();
	}
};

#elif defined(_MSC_VER) && !defined(NUTCRACKER)

#	ifndef _WINDEF_	/* do these if we *haven't* included windows.h */

	typedef	void * CRITICAL_SECTION;
	typedef void * HANDLE; //-V677
	typedef void * CONDITION_VARIABLE;
	typedef void * SRWLOCK;

#ifdef STATIC_APP
#define WINAPI_DECL
#else
#define WINAPI_DECL __declspec(dllimport)
#endif

	extern "C" {
		WINAPI_DECL unsigned long	__stdcall GetCurrentThreadId ();

		WINAPI_DECL void 			__stdcall EnterCriticalSection (void * cs);
		WINAPI_DECL int 			__stdcall TryEnterCriticalSection (void * cs);
		WINAPI_DECL void 			__stdcall LeaveCriticalSection (void * cs);
		WINAPI_DECL void 			__stdcall InitializeCriticalSection (void * cs);
		WINAPI_DECL void 			__stdcall DeleteCriticalSection (void * cs);

		WINAPI_DECL int    			__stdcall CloseHandle (HANDLE h);
		WINAPI_DECL int    			__stdcall WaitForSingleObject (HANDLE h, int milli);

		WINAPI_DECL void			__stdcall InitializeConditionVariable(CONDITION_VARIABLE * cv);
		WINAPI_DECL void			__stdcall WakeConditionVariable(CONDITION_VARIABLE * cv);
		WINAPI_DECL int				__stdcall SleepConditionVariableCS(CONDITION_VARIABLE * cv, void * cs, int milli);
		WINAPI_DECL int				__stdcall SleepConditionVariableSRW(CONDITION_VARIABLE * cv, void * cs, int milli, unsigned long flags);
		

		WINAPI_DECL void			__stdcall InitializeSRWLock(SRWLOCK * srw); // vista and above
		WINAPI_DECL void			__stdcall AcquireSRWLockExclusive(SRWLOCK * srw);
		WINAPI_DECL void			__stdcall ReleaseSRWLockExclusive(SRWLOCK * srw);
		WINAPI_DECL int				__stdcall TryAcquireSRWLockExclusive(SRWLOCK * srw); // win7 and above


		WINAPI_DECL void			__stdcall Sleep (int milli);

		WINAPI_DECL uintptr_t		__cdecl _beginthreadex(void *security, unsigned stack_size, unsigned ( __stdcall *start_address )( void * ), void *arglist, unsigned initflag, unsigned *thrdaddr);
	}

#	endif

#	define	THREAD_ID()	(HThreadID)GetCurrentThreadId()

struct System_Thread {

	HANDLE		handle;

	struct bounce_info_t {
		void(*func_)(void *);
		void *				data_;
		HThreadID			thread_id_;
		volatile uint32_t	flag_;
	};
	bounce_info_t bounce_info;

	static unsigned int __stdcall bounce_(void * data) {
		bounce_info_t * bounce_info = (bounce_info_t *)data;
		bounce_info->thread_id_ = THREAD_ID();
		void(*_func)(void *) = bounce_info->func_;
		void * _data = bounce_info->data_;
		locked_increment((const uint32_t*)&bounce_info->flag_);
		_func(_data);
		return 0;
	}

	INLINE void create(void(*func)(void *), void * data) {
		bounce_info.func_ = func;
		bounce_info.data_ = data;
		bounce_info.flag_ = 0;
		handle = (HANDLE) _beginthreadex(NULL, 0, bounce_, &bounce_info, 0, NULL);
		while (bounce_info.flag_ == 0)
			THREAD_YIELD();
	}

	INLINE void join() {
		WaitForSingleObject(handle, 0xFFFFFFFF);
		CloseHandle(handle);
		handle = 0;
	}
	INLINE void detach() {
		CloseHandle(handle);
		handle = 0;
	}

	INLINE void set_name(const char * name) {
		HI_Set_Thread_Name(bounce_info.thread_id_, name);
	}
};


Begin_HOOPS_Namespace

	struct System_Semaphore {
		
		CONDITION_VARIABLE	cv;
		SRWLOCK				srw;
		int					count;

		INLINE void create(int initial_count) {
			InitializeConditionVariable(&cv);
			InitializeSRWLock(&srw);
			count = initial_count;
		}
		INLINE void destroy() {
		}
		INLINE void release(int in_count) {
			AcquireSRWLockExclusive(&srw);
			count += in_count;
			int current_count = count;
			ReleaseSRWLockExclusive(&srw);
			while (current_count-- > 0)
				WakeConditionVariable(&cv);
		}
		INLINE void wait() {
			AcquireSRWLockExclusive(&srw);
			while (count == 0) {
				while (SleepConditionVariableSRW(&cv, &srw, 0xFFFFFFFF, 0) == 0)
					AcquireSRWLockExclusive(&srw);
			}
			count--;
			ReleaseSRWLockExclusive(&srw);
		}
	};

	struct System_Mutex {
		CRITICAL_SECTION	cs;

		INLINE void create() {
			InitializeCriticalSection (&cs);
		}
		INLINE void destroy() {
			DeleteCriticalSection (&cs);
		}
		INLINE void lock() {
			EnterCriticalSection(&cs);
		}
		INLINE void unlock() {
			LeaveCriticalSection(&cs);
		}
		INLINE bool try_lock() {
			return (TryEnterCriticalSection(&cs) != 0);
		}
	};

#else

#   include <pthread.h>
Begin_HOOPS_Namespace

#	define	THREAD_ID()	(HThreadID)pthread_self()

struct System_Thread {

	pthread_t	thrd;

	struct bounce_info_t {
		void(*func_)(void *);
		void * data_;
		HThreadID	thread_id_;
		volatile uint32_t	flag_;
	};
	bounce_info_t bounce_info;

	static void * bounce_(void * data) {
		bounce_info_t * bounce_info = (bounce_info_t *)data;
		bounce_info->thread_id_ = THREAD_ID();
		void(*_func)(void *) = bounce_info->func_;
		void * _data = bounce_info->data_;
		locked_increment((const uint32_t*)&bounce_info->flag_);
		_func(_data);
		return 0;
	}

	INLINE void create(void(*func)(void *), void * data) {
		bounce_info.func_ = func;
		bounce_info.data_ = data;
		bounce_info.flag_ = 0;
		pthread_create(&thrd, 0, bounce_, &bounce_info);
		while (bounce_info.flag_ == 0)
			THREAD_YIELD();
	}

	INLINE void join() {
		pthread_join(thrd, 0);
	}
	INLINE void detach() {
		pthread_detach(thrd);
	}

	INLINE void set_name(const char * name) {
		HI_Set_Thread_Name(bounce_info.thread_id_, name);
	}
};

#ifdef OSX_SYSTEM
End_HOOPS_Namespace
#include <dispatch/dispatch.h>
Begin_HOOPS_Namespace
	
	struct System_Semaphore {
        
		dispatch_semaphore_t semaphore;

		INLINE void create(int initial_count) {
			semaphore = dispatch_semaphore_create(initial_count);
		}
		INLINE void destroy() {
			dispatch_release(semaphore);
		}
		INLINE void release(int count) {
			while (count-- > 0)
				dispatch_semaphore_signal(semaphore);
		}
		INLINE void wait() {
			 dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
		}
	};

#else

End_HOOPS_Namespace
#	include <semaphore.h>
Begin_HOOPS_Namespace

	struct System_Semaphore {
		sem_t	sm;

		INLINE void create(int initial_count) {
			sem_init (&sm, 0, initial_count);
		}
		INLINE void destroy() {
			sem_destroy (&sm);
		}
		INLINE void release(int count) {
			while (count-- > 0)
				sem_post (&sm);
		}
		INLINE void wait() {
			while (0 != sem_wait (&sm));
		}
	};

#	endif

	struct System_Mutex {
		pthread_mutex_t mutex;
		INLINE void create() {
			pthread_mutex_init(&mutex, 0);
		}
		INLINE void destroy() {
			pthread_mutex_destroy(&mutex);
		}
		INLINE void lock() {
			pthread_mutex_lock(&mutex);
		}
		INLINE void unlock() {
			pthread_mutex_unlock(&mutex);
		}
		INLINE bool try_lock() {
			return (pthread_mutex_trylock(&mutex) == 0);
		}
	};

#endif

	class HOOPS_UTILITY_PRIVATE_API HM_Thread {

	public:
		typedef void(*thread_func_t)(void *);

		// if data is not null what it points at should survive until the thread exits
		static HM_Thread * Create(thread_func_t	func, void * data, const char * name = "HOOPS");

		// A thread that is created must be have either Join or Detach called
		// the thread handle is not valid after the call

		// returns true if the thread had started before being joined
		// returns false if it had not started - this only can happen before main
		bool Join();

		void Detach();

		static void Launch_Delayed_Threads();

		static void Abort_Delayed_Threads();

		const char * Get_Name() { return &name[0]; }
		void Set_Name(const char * name);

	private:

		System_Thread	thread;
		thread_func_t	func;
		void *			data;
		char			name[32];
		bool			started;
		bool			detached;

		HM_Thread(HM_Thread const &);
		void operator= (HM_Thread const &);
	};

	typedef HM_Thread * HThreadHandle;

	class HOOPS_UTILITY_PRIVATE_API HM_Semaphore {
	private:
		mutable	int32_t				count;
		mutable	System_Semaphore	system_semaphore;

	public:

		static HM_Semaphore * Create(int32_t initial_count);

		static void Destroy (HM_Semaphore *& s);

		INLINE int32_t Count() {
			return count;
		}

		INLINE void Release(int32_t release_count = 1) {
			_DEBUG_TRACE("RELEASE_SEMAPHORE1:",THREAD_ID(),this);
			_DEBUG_TRACE("RELEASE_SEMAPHORE2:",THREAD_ID(),count);
			int32_t prev = locked_add(&count, release_count);
			if (prev < 0) {
				int num_waiters = -prev;
				int num_to_wake = Min(num_waiters, release_count);
				system_semaphore.release(num_to_wake);
			}
		}

		INLINE void Wait() {
			_DEBUG_TRACE("+WAIT_SEMAPHORE:",THREAD_ID(),this);
			if (locked_add(&count, -1) <= 0)
				system_semaphore.wait();
			_DEBUG_TRACE("-WAIT_SEMAPHORE:",THREAD_ID(),this);
		}

		INLINE void ReleaseAll() {
			if (count < 0) {
				int num_waiters = -count;
				Release(num_waiters);
			}
		}

	private:
		HM_Semaphore (HM_Semaphore const &);
		void operator= (HM_Semaphore const &);
	};

	class HOOPS_UTILITY_PRIVATE_API HM_Mutex {
	private:
		mutable HThreadID		owner;
		mutable	uint32_t		nesting;
		mutable	System_Mutex	system_mutex;

	public:
		static HM_Mutex * Create();

		static void Destroy (HM_Mutex *& m);

		INLINE void Lock() const {
			HThreadID _me_ = THREAD_ID();
			if (owner != _me_) {
				_DEBUG_TRACE("LOCK_MUTEX: before",_me_,this);
				system_mutex.lock();
				owner = _me_;
				_DEBUG_TRACE("LOCK_MUTEX: after",_me_,this);
			}
			++nesting;
		}

		INLINE void Unlock() const {
			HThreadID _me_ = THREAD_ID();
			_DEBUG_TRACE("UNLOCK_MUTEX: before",_me_,this);
			if (owner != _me_) {
				ASSERT(0);
				_DEBUG_TRACE("UNLOCK_MUTEX: failed",_me_,this);
			} else if (--nesting == 0) {
				owner = 0;
				system_mutex.unlock();
				_DEBUG_TRACE("UNLOCK_MUTEX: after",_me_,this);
			}
		}

		INLINE bool TryLock() const {
			HThreadID me = THREAD_ID();
			if (owner != me) {
				if (owner != 0)
					return false;
				if (system_mutex.try_lock() == false) 
					return false;
				owner = me;
			}
			++nesting;
			return true;
		}

		INLINE HThreadID Owner() const {
			return owner;
		}

		INLINE uint32_t Nesting() const {
			return nesting;
		}

	private:
		HM_Mutex (HM_Mutex const &);
		void operator= (HM_Mutex const &);
	};



//#define H_DEBUG_SPINLOCK_TRACE_RINGBUFFER
#ifndef H_DEBUG_SPINLOCK_TRACE_RINGBUFFER
#define	_H_DEBUG_SPINLOCK_TRACE(_tag_,_lock_,_me_,_owner_,_nesting_) (void)(_tag_),(void)(_lock_),(void)(_me_),(void)(_owner_),(void)(_nesting_)
#else
#  include <stdio.h>

#define	H_DEBUG_SPINLOCK_MESSAGE_LENGTH	128
#define H_DEBUG_SPINLOCK_RINGBUFFER_COUNT	(4096*16)

#ifdef  HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
	char debug_spinlock_ringbuffers[H_DEBUG_SPINLOCK_RINGBUFFER_COUNT][H_DEBUG_SPINLOCK_MESSAGE_LENGTH];
	uint32_t debug_spinlock_ringbuffer_index = 0;
#else
	extern char debug_spinlock_ringbuffers[H_DEBUG_SPINLOCK_RINGBUFFER_COUNT][H_DEBUG_SPINLOCK_MESSAGE_LENGTH];
	extern uint32_t debug_spinlock_ringbuffer_index;
#endif

#define	_H_DEBUG_SPINLOCK_TRACE(_tag_,_lock_,_me_,_owner_,_nesting_)	do { \
	int index = locked_increment(&debug_spinlock_ringbuffer_index)%H_DEBUG_SPINLOCK_RINGBUFFER_COUNT; \
	debug_spinlock_ringbuffers[index][0] = '\0'; \
	sprintf (debug_spinlock_ringbuffers[index], "%s %p %d %p %p ", _tag_, (void*)_lock_, (int)_nesting_, (void*)_me_, (void*)_owner_); \
	} while (0)

#endif

class Spinlock {
private:
	mutable HThreadID			owner;
	mutable volatile uint32_t	flag;
	mutable uint32_t			nesting;

public:
	Spinlock() : owner(0), flag(0), nesting(0) {}

	INLINE bool SpinUp(HThreadID me, bool try_once_only=false) const {

		if (owner == me) {
			++nesting;		// ours anyway
			_H_DEBUG_SPINLOCK_TRACE("=", this, me, owner, nesting);
			return true;
		}

		int	retries = 0;

		while (flag != 0 || locked_test_and_set ((uint32_t *)&flag) != 0) {
			if (try_once_only) {
				_H_DEBUG_SPINLOCK_TRACE("*", this, me, owner, nesting);
#ifdef H_DEBUG_SPINLOCK_TRACE_RINGBUFFER
				THREAD_YIELD();
#endif
				return false;
			}
			ThreadPause();
			if (++retries == 64) {
				THREAD_YIELD();
				retries = 0;
			}
		}

		owner = me;
		nesting = 1;
		_H_DEBUG_SPINLOCK_TRACE("+", this, me, owner, nesting);
		return true;
	}

	INLINE void SpinDown() const {
		_H_DEBUG_SPINLOCK_TRACE("-", this, THREAD_ID(), owner, nesting);
		if (owner != 0 && --nesting == 0) {
			owner = 0;
			locked_clear ((uint32_t *)&flag);
		}
		if (nesting > 0) {
			_H_DEBUG_SPINLOCK_TRACE("$", this, THREAD_ID(), owner, nesting);
		}
	}

	INLINE bool Mine(HThreadID me) const {
		return(owner == me);
	}

	INLINE HThreadID Owner() const {
		return owner;
	}

private:
	// don't really want or need these, except to shut up the compiler
	Spinlock (Spinlock const &);
	Spinlock const & operator= (Spinlock const &);
};


class Spinner {
private:
	Spinlock const &		sld1;
	Spinlock const *		sld2;
	HThreadID				id;
	uint32_t				spun;

public:
	INLINE Spinner (Spinlock const & sld, HThreadID sid, bool spin_it=true) : sld1 (sld), sld2 (0), id(sid), spun(0) {
		if (spin_it)
			Up();	
	}

	INLINE Spinner (Spinlock const & sld_one, Spinlock const & sld_two, HThreadID sid, bool spin_it=true) : sld1 (sld_one), sld2(&sld_two), id(sid),spun(false) {
		if (spin_it)
			Up();	
	}

	INLINE ~Spinner () {
		if (spun)
			Down();
	}

	INLINE void Up() {
		ASSERT(!spun);
		for (;;) {
			sld1.SpinUp(id);
			if (sld2 == 0 || sld2->SpinUp(id, true))
				break;
			sld1.SpinDown();
			THREAD_YIELD();
		}
		spun = 1;
	}

	INLINE void Down() {
		ASSERT(spun);
		sld1.SpinDown();
		if (sld2)
			sld2->SpinDown();
		spun = 0;
	}

private:
	Spinner (Spinner const &);
	void operator= (Spinner const &);
};


class HOOPS_UTILITY_PRIVATE_API Benaphore
{ //-V802
private:
	mutable uint32_t			count;
	uint32_t					exists;
	mutable	System_Semaphore	system_semaphore;
	

public:

	INLINE	Benaphore() {
		count = 0;
		exists = 1;
		system_semaphore.create(0);
	}

	INLINE	~Benaphore() {
		if (Exists())
			system_semaphore.destroy();
	}

	INLINE void Lock() const
	{
		if (locked_increment(&count) > 1)
			system_semaphore.wait();
	}

	INLINE bool TryLock() const
	{
		return locked_CAS(&count, 1, 0);
	}

	INLINE void Unlock() const
	{
		if (locked_decrement(&count) > 0)
			system_semaphore.release(1);
	}

	INLINE bool Exists() const {return (exists & 1);}
};

class HOOPS_UTILITY_PRIVATE_API Embedaphore
{ //-V802
private:
	mutable uint32_t			count;
	uint32_t					exists;
	mutable	System_Semaphore	system_semaphore;
	

public:

	INLINE void	Init() {
		count = 0;
		exists = 1;
		system_semaphore.create(0);
	}

	INLINE void Free() {
		if (Exists())
			system_semaphore.destroy();
	}

	INLINE void Lock() const
	{
		if (locked_increment(&count) > 1)
			system_semaphore.wait();
	}

	INLINE bool TryLock() const
	{
		return locked_CAS(&count, 1, 0);
	}

	INLINE void Unlock() const
	{
		if (locked_decrement(&count) > 0)
			system_semaphore.release(1);
	}

	INLINE bool Exists() const {return (exists & 1);}
};

class HOOPS_UTILITY_PRIVATE_API RecursiveBenaphore
{
private:
	mutable HThreadID	owner;
	mutable uint32_t	count;
	mutable uint32_t	nesting;
	uint32_t			exists;

	mutable	System_Semaphore	system_semaphore;

	
public:

	INLINE	RecursiveBenaphore() {
		count = 0;
		nesting = 0;
		owner = 0;
		system_semaphore.create(0);
		exists = 1;
	}

	INLINE	~RecursiveBenaphore() {
		if (Exists())
			system_semaphore.destroy();
	}

	INLINE void Lock(HThreadID me) const
	{
		if (locked_increment(&count) > 1) {
			if (me != owner)
				system_semaphore.wait();
		}
		owner = me;
		nesting++;
	}

	INLINE bool TryLock(HThreadID me) const
	{
		if (me == owner) {
			locked_increment(&count);
		}
		else if (owner != 0) {
			return false;
		}
		else {
			bool result = locked_CAS(&count, 1, 0);
			if (result == false)
				return false;
			owner = me;
		}
		nesting++;
		return true;
	}

	INLINE void Unlock() const
	{
		ASSERT(owner == THREAD_ID());
		uint32_t last_nesting = --nesting;
		if (last_nesting == 0)
			owner = 0;
		if (locked_decrement(&count) > 0) {
			if (last_nesting == 0)
				system_semaphore.release(1);
		}
	}

	INLINE bool Exists() const {return (exists & 1);}

	INLINE bool Mine(HThreadID me) const {
		return(owner == me);
	}

	INLINE HThreadID Owner() const {
		return owner;
	}
};

template <typename T>
class HOOPS_UTILITY_PRIVATE_API Autophore {
private:
	T const &	lock;

public:
	INLINE Autophore (T const & l) : lock(l) {
		lock.Lock();
	}

	INLINE ~Autophore () {
		lock.Unlock();
	}
private:
	// don't really want or need these, except to shut up the compiler
	Autophore (Autophore const &);
	Autophore const & operator= (Autophore const &);
};




/* Lock is a complex, many reader, single (nested) writer mechanism */
/* need to simulate complex read/write lock using available objects */

#define HM_Per_Thread_Flag_NONE  0x0000
#define HM_Per_Thread_Flag_SHARE 0x0001

struct HM_Per_Thread {
	HThreadID	id;			/* thread with any active or pending locks */
	int			state;		/* each bit on if corresponding lock is for write */
	short		flags;
	short		use_count;	/* number of locks for this thread */
};

class HOOPS_UTILITY_PRIVATE_API HM_RWLock {
private:
	HM_Per_Thread *	threads;
	Embedaphore		control; /* single-access mutex */
	HM_Semaphore *	read;
	HM_Semaphore *	write;
	int				active_readers, pending_readers;
	int				active_writers, pending_writers;
	int				active_threads, max_threads;
	int				active_shares;

public:

	static HM_RWLock * Create (int inital_max_threads=8);
	static void Destroy (HM_RWLock *& hl);

	bool Read (bool required=true);
	INLINE bool TryRead () {return Read(false);}

	bool Write (bool required=true);
	INLINE bool TryWrite () {return Write(false);}

	void Release ();
	void ShareRead (HThreadID id);

#define		HM_LOCK_STATE_NONE			0x0000		// no locks
#define		HM_LOCK_STATE_READ			0x0001		// current thread has read access
#define		HM_LOCK_STATE_WRITE			0x0002		// current thread has write access
#define		HM_LOCK_STATE_SHARED		0x0004		// current thread has shared access
#define		HM_LOCK_STATE_OTHER_READ	0x0010		// read access on other thread(s)
#define		HM_LOCK_STATE_OTHER_WRITE	0x0020		// write access on another thread
#define		HM_LOCK_STATE_OTHER_SHARED	0x0040		// shared access on other thread(s)

	int CheckState ();

#ifdef DEBUG_LOCKING_TRACE
	static void lock_trace (
		char const *	tag,
		HThreadID		thread_id,
		HM_RWLock *		lock,
		int				use_count);
#endif

private:

	void expand();

	HM_RWLock (HM_RWLock const &);
	void operator= (HM_RWLock const &);
};




#endif // LOCKING_DEFINED

