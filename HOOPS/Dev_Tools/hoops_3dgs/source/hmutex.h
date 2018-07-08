// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HOOPS_MUTEX_DEFINED
#define HOOPS_MUTEX_DEFINED

/* Note:
if the read/write lock is needed,

#define HMUTEX_IMPLEMENT_LOCK_FUNCTIONS

in one module that includes this header to generate the code components

on Windows,

#define HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS

is similarly used to generate the non-inlined mutex components.


#define HMUTEX_IMPLEMENT_FUNCTIONS

can be used if both are desired
*/

#ifdef HMUTEX_NAMESPACE
#define	Begin_HMUTEX_NAMESPACE_Namespace	namespace HMUTEX_NAMESPACE {
#define	End_HMUTEX_NAMESPACE_Namespace		}
#else 
#define	Begin_HMUTEX_NAMESPACE_Namespace
#define	End_HMUTEX_NAMESPACE_Namespace
#endif


#ifndef intptr_t
#	if defined(_M_IA64) || defined(_M_AMD64) || defined(WIN64) || defined(_WIN64) || defined(_M_X64)
#		define	HMUTEX_POINTER_SIZED_INT __int64
#	else
#		define	HMUTEX_POINTER_SIZED_INT long
#	endif
#else 
#	define HMUTEX_POINTER_SIZED_INT intptr_t
#endif

#ifdef _MSC_VER

#	ifdef HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
#		define POINT WINDOWS_POINT
#			ifndef WIN32_LEAN_AND_MEAN
#				define WIN32_LEAN_AND_MEAN
#			endif
#			include <windows.h>
#		undef max
#		undef POINT
#	endif

#	ifndef _WINDEF_	/* do these if we *haven't* included windows.h */

		typedef	void * CRITICAL_SECTION;
		typedef void * HANDLE;

#		ifdef __cplusplus
			extern "C" {
#		endif
			__declspec(dllimport) unsigned long	__stdcall GetCurrentThreadId ();

			__declspec(dllimport) void 			__stdcall EnterCriticalSection (void * cs);
			__declspec(dllimport) int 			__stdcall TryEnterCriticalSection (void * cs);
			__declspec(dllimport) void 			__stdcall LeaveCriticalSection (void * cs);
			__declspec(dllimport) void 			__stdcall InitializeCriticalSection (void * cs);
			__declspec(dllimport) void 			__stdcall DeleteCriticalSection (void * cs);

			__declspec(dllimport) HANDLE 		__stdcall CreateSemaphoreA (void * sa, long init, long mac, char const * name);
			__declspec(dllimport) int    		__stdcall CloseHandle (HANDLE h);
			__declspec(dllimport) int    		__stdcall ReleaseSemaphore (HANDLE s, long rel, long * prev);
			__declspec(dllimport) int    		__stdcall WaitForSingleObject (HANDLE h, int milli);

			__declspec(dllimport) void			__stdcall Sleep (int dwMilliseconds);
#		ifdef __cplusplus
			}
#		endif
#	endif

	typedef unsigned HMUTEX_POINTER_SIZED_INT	HThreadID;		/* unique thread identifier */			
	typedef HANDLE								HThreadHandle;	/* unique thread handle */
	typedef void *								HMutex;			/* exclusive (single thread) access */
	typedef void *								HRWLock;		/* multiple readers, exclusive writer */
#endif

#if defined(_MSC_VER) && !defined(NUTCRACKER)
#	include <time.h>
#	define MILLI_SECOND_SLEEP(_x_) Sleep((int)(_x_))
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
#endif


#ifdef DISABLE_MUTEX
#	define		THREAD_ID()				0

#	define		CREATE_MUTEX(_m_)		_m_ = (void *)(~0)		/* non-null */
#	define		LOCK_MUTEX(_m_)			(void)(_m_)
#	define		TRY_LOCK_MUTEX(_m_)		(void)(_m_)
#	define		UNLOCK_MUTEX(_m_)		(void)(_m_)
#	define		DESTROY_MUTEX(_m_)		_m_ = 0

#	define		CREATE_LOCK(_l_)		CREATE_MUTEX(_l_)
#	define		READ_LOCK(_l_)			LOCK_MUTEX(_l_)
#	define		WRITE_LOCK(_l_,_r_)		(void)(_r_),LOCK_MUTEX(_l_)
#	define		UNLOCK_LOCK(_l_)		UNLOCK_MUTEX(_l_)
#	define		DESTROY_LOCK(_l_)		DESTROY_MUTEX(_l_)

#	define	HM_Semaphore				HMutex

#	define	HM_CONSTRUCT_SEMAPHORE(_s_, _l_, _v_)	_s_ = (void *)(~0)
#	define	HM_DESTRUCT_SEMAPHORE(_s_)				_s_ = 0
#	define	HM_RELEASE_SEMAPHORE(_s_, _n_)			(void)(_s_)
#	define	HM_WAIT_SEMAPHORE(_s_)					(void)(_s_)

#	define	THREAD_FUNCTION(_funct_) \
				HM_DECLARE_FUNCTION unsigned int __stdcall _hthread_##_funct_(void *); \
				HM_DEFINE_FUNCTION unsigned int __stdcall _hthread_##_funct_(void * _thread_data_)
#	define	THREAD_FUNCTION_DATA(_cast_)		(_cast_)_thread_data_ 
#	define	THREAD_FUNCTION_CALL(_funct_,_data_)		_hthread_##_funct_((void*)_data_)


#	ifndef _WIN32_WCE
#		define		CREATE_THREAD(_id_,_funct_,_data_) #error threading disabled in build
#		define		JOIN_THREAD(_id_) #error threading disabled in build
#	else
#		define		CREATE_THREAD(_id_,_funct_,_data_)	
#		define		JOIN_THREAD(_id_)					
#	endif
#else

#ifdef HMUTEX_IMPLEMENT_FUNCTIONS
#	ifndef HMUTEX_IMPLEMENT_LOCK_FUNCTIONS
#		define HMUTEX_IMPLEMENT_LOCK_FUNCTIONS
#		ifndef HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
#			define HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
#		endif
#	endif
#	ifndef HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
#		define HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
#	endif
#endif

#include <string.h>

#ifndef ALLOCATE_MEMORY		/* simple memory handling if not building inside HOOPS */
#	include <stdlib.h>
#	define		ALLOCATE_MEMORY(p,t)			p = (t *)malloc(sizeof(t))
#	define		FREE_MEMORY(p,t)				(void)sizeof(t),free(p)
#	define		ALLOCATE_MEMORY_ARRAY(p,c,t)	p = (t *)malloc((c)*sizeof(t))
#	define		FREE_MEMORY_ARRAY(p,c,t)		(void)c,(void)sizeof(t),free(p)
#endif


#ifndef HMUTEX_INITIAL_THREADS
#   define	HMUTEX_INITIAL_THREADS	8
#endif
#ifndef HMUTEX_MAX_CONCURRENT_READERS
#   define	HMUTEX_MAX_CONCURRENT_READERS	4096
#endif

#ifndef HMUTEX_ERROR
#   include <stdio.h>
#   define	HMUTEX_ERROR(_tag_)	printf ("%s\n", _tag_)
#endif

/* use -DH_MUTEX_DEBUG_TRACE -DH_RW_LOCK_DEBUG_TRACE */
/*
#define H_RW_LOCK_DEBUG_TRACE
#define H_DEBUG_TRACE_RINGBUFFER
#define H_SEMAPHORE_DEBUG_TRACE
*/


#if defined(H_MUTEX_DEBUG_TRACE) || defined(H_RW_LOCK_DEBUG_TRACE) || defined(H_SEMAPHORE_DEBUG_TRACE)
#  include <stdio.h>

#define	H_DEBUG_MESSAGE_LENGTH	256

#  ifdef H_DEBUG_TRACE_RINGBUFFER
#    define H_DEBUG_RINGBUFFER_COUNT	4096
#    define H_DEBUG_RINGBUFFER_LEN		H_DEBUG_MESSAGE_LENGTH

#    ifdef  HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
		char **			debug_ringbuffers = 0;
		int				debug_ringbuffer_index = 0;
#    else
		extern char **	debug_ringbuffers;
		extern int		debug_ringbuffer_index;
#    endif

#   define	_H_DEBUG_TRACE(_tag_,_id_,_synch_,_lines_)	do { \
		int index = debug_ringbuffer_index++; \
		char *buffer; \
		if (debug_ringbuffer_index >= H_DEBUG_RINGBUFFER_COUNT) \
			debug_ringbuffer_index = 0; \
		if (debug_ringbuffers == 0) { \
			int i; \
			ALLOCATE_MEMORY_ARRAY (debug_ringbuffers, H_DEBUG_RINGBUFFER_COUNT, char *); \
			for (i = 0 ; i < H_DEBUG_RINGBUFFER_COUNT ; i++) { \
				ALLOCATE_MEMORY_ARRAY (debug_ringbuffers[i], H_DEBUG_RINGBUFFER_LEN, char); \
			} \
		} \
		debug_ringbuffers[debug_ringbuffer_index][0] = '\0'; \
		buffer = debug_ringbuffers[index]; \
		sprintf (buffer, "%p %p %s", (void*)_id_, (void*)_synch_, _tag_); \
		buffer += strlen(buffer); \
		if (_lines_) {\
			sprintf (buffer, " %s %d", __FILE__, __LINE__); \
		} \
	} while (0)

#  else

#	ifndef H_MUTEX_DEBUG_TRACE_FILE_NAME
#		define H_MUTEX_DEBUG_TRACE_FILE_NAME "hmutex_debug_trace_file.txt"
#	endif
#   define	_H_DEBUG_TRACE(_tag_,_id_,_synch_,_lines_)	do { \
		FILE *hmutex_debug_trace_file = fopen(H_MUTEX_DEBUG_TRACE_FILE_NAME,"a+"); \
		if (hmutex_debug_trace_file != 0) { \
			fprintf (hmutex_debug_trace_file, "%p %p %s", (void*)_id_, (void*)_synch_, _tag_); \
			if (_lines_) \
				fprintf (hmutex_debug_trace_file, " %s %d", __FILE__, __LINE__); \
			fprintf (hmutex_debug_trace_file, "\n"); \
			fclose (hmutex_debug_trace_file); \
		} else { \
			fprintf(stderr, "opening %s failed.\n", H_MUTEX_DEBUG_TRACE_FILE_NAME); \
		} \
	} while (0)

#  endif
#endif

#ifdef H_MUTEX_DEBUG_TRACE
#   define	_H_MUTEX_DEBUG_TRACE(_tag_,_id_,_synch_) _H_DEBUG_TRACE(_tag_,_id_,_synch_,1)
#else
#   define	_H_MUTEX_DEBUG_TRACE(_tag_,_id_,_synch_) (void)_tag_,(void)_id_,(void)_synch_
#endif

#ifdef H_SEMAPHORE_DEBUG_TRACE
#   define	_H_SEMAPHORE_DEBUG_TRACE(_tag_,_id_,_synch_) _H_DEBUG_TRACE(_tag_,_id_,_synch_,1)
#else
#   define	_H_SEMAPHORE_DEBUG_TRACE(_tag_,_id_,_synch_) (void)_tag_,(void)_id_,(void)_synch_
#endif



#ifdef __cplusplus
#	define HM_DECLARE_FUNCTION	extern "C"
#	define HM_DEFINE_FUNCTION	extern "C"
#else
#	define HM_DECLARE_FUNCTION				/* (default) */
#	define HM_DEFINE_FUNCTION	extern
#endif
#ifndef HM_CDECL
#	define	HM_CDECL
#endif


#if defined(_MSC_VER) && !defined(NUTCRACKER)
#	include <process.h>

#   ifndef HM_CDECL
#		define	HM_CDECL	__cdecl
#   endif

#	define	THREAD_ID()						(HThreadID)GetCurrentThreadId()

#ifndef DEBUG_HM_SEMAPHORES
	typedef	HANDLE	HM_Semaphore;

#	define	HM_CONSTRUCT_SEMAPHORE(_s_, _l_, _v_)	_s_ = CreateSemaphoreA (0, _v_, _l_, 0)
#	define	HM_DESTRUCT_SEMAPHORE(_s_)				CloseHandle (_s_)
#	define	HM_RELEASE_SEMAPHORE(_s_, _n_)			do { \
														_H_SEMAPHORE_DEBUG_TRACE("RELEASE_SEMAPHORE1:",THREAD_ID(),_s_); \
														_H_SEMAPHORE_DEBUG_TRACE("RELEASE_SEMAPHORE2:",THREAD_ID(),_n_); \
														if (!ReleaseSemaphore (_s_, _n_, 0)) { \
															long n = (long)_n_; \
															while (n > 0) { \
																_H_SEMAPHORE_DEBUG_TRACE("RELEASE_SEMAPHORE3:",THREAD_ID(),1); \
																while (!ReleaseSemaphore (_s_, 1, 0)) \
																	MILLI_SECOND_SLEEP(1); \
																n--; \
															} \
														} \
													}while (0)
#	define	HM_WAIT_SEMAPHORE(_s_)					do { \
														_H_SEMAPHORE_DEBUG_TRACE("+WAIT_SEMAPHORE:",THREAD_ID(),_s_); \
														WaitForSingleObject (_s_, 0xFFFFFFFF); \
														_H_SEMAPHORE_DEBUG_TRACE("-WAIT_SEMAPHORE:",THREAD_ID(),_s_); \
													}while (0)  
#endif

#	define  CREATE_THREAD(_id_,_funct_,_data_)	_id_ = (HThreadHandle) _beginthreadex(NULL, 0, _hthread_##_funct_, (void*)_data_, 0, NULL);
#	define	JOIN_THREAD(_id_)					do { \
													WaitForSingleObject ((HANDLE)_id_, 0xFFFFFFFF); \
													CloseHandle ((HANDLE)_id_); \
												} while (0)

#	define USE_CRITICAL_SECTION_MUTEX

#	ifdef USE_CRITICAL_SECTION_MUTEX

// 		HM_DECLARE_FUNCTION void *	HM_CDECL	HM_Create_Mutex ();
// 		HM_DECLARE_FUNCTION void *	HM_CDECL	HM_Destroy_Mutex (void * l);
// 
// 
// #		define		CREATE_MUTEX(_m_)	_m_ = HM_Create_Mutex()
// #		define		LOCK_MUTEX(_m_)		EnterCriticalSection ((CRITICAL_SECTION *)_m_)
// #		define		UNLOCK_MUTEX(_m_)	LeaveCriticalSection ((CRITICAL_SECTION *)_m_)
// #		define		DESTROY_MUTEX(_m_)	_m_ = HM_Destroy_Mutex (_m_)
// #		define		TRY_LOCK_MUTEX(_m_) TryEnterCriticalSection((CRITICAL_SECTION *)_m_)
// 
// 
// #		ifdef HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
// 
// 			HM_DEFINE_FUNCTION	void * HM_CDECL HM_Create_Mutex () {
// 				CRITICAL_SECTION *		cs;
// 				ALLOCATE_MEMORY (cs, CRITICAL_SECTION);
// 				InitializeCriticalSection (cs);
// 				return (void *)cs;
// 			}
// 
// 			HM_DEFINE_FUNCTION	void * HM_CDECL HM_Destroy_Mutex (void * m) {
// 				CRITICAL_SECTION *		cs = (CRITICAL_SECTION *)m;
// 				DeleteCriticalSection (cs);
// 				FREE_MEMORY (cs, CRITICAL_SECTION);
// 				return 0;
// 			}
// #		endif

	typedef struct {
		HThreadID			thread;
		int					use_count;
		CRITICAL_SECTION	cs;
	} HM_Critical_Section_Mutex;


#   define      CREATE_MUTEX(_m_)	_m_ = HM_Create_Mutex ()

#   define      DESTROY_MUTEX(_m_)	_m_ = HM_Destroy_Mutex (_m_)

#   define      LOCK_MUTEX(_m_)                                                 \
				do {                                                            \
					HM_Critical_Section_Mutex * _hm_ = (HM_Critical_Section_Mutex*)_m_;	\
					HThreadID _me_ = THREAD_ID();                               \
					if (_hm_->thread != _me_) {                                 \
						_H_MUTEX_DEBUG_TRACE("LOCK_MUTEX: before",_me_,_hm_);   \
						EnterCriticalSection(&_hm_->cs);			            \
						_hm_->thread = _me_;                                    \
						_H_MUTEX_DEBUG_TRACE("LOCK_MUTEX: after",_me_,_hm_);    \
					}                                                           \
					++_hm_->use_count;                                          \
				} while(0)

#   define      UNLOCK_MUTEX(_m_)                                               \
				do {                                                            \
					HM_Critical_Section_Mutex * _hm_ = (HM_Critical_Section_Mutex*)_m_;	\
					HThreadID _me_ = THREAD_ID();                               \
					_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: before",_me_,_hm_);     \
					if (_hm_->thread != _me_) {                                 \
						_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: failed",_me_,_hm_); \
						HMUTEX_ERROR ("Mutex non-owner release");               \
					} else if (--_hm_->use_count == 0) {                        \
						_hm_->thread = 0;                                       \
						LeaveCriticalSection(&_hm_->cs);						\
						_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: after",_me_,_hm_);  \
					}                                                           \
				} while(0)

#   define		MUTEX_OWNER(_m_)	(((HM_Critical_Section_Mutex *)(_m_))->thread)
#   define		MUTEX_NESTING(_m_)	(((HM_Critical_Section_Mutex *)(_m_))->use_count)

#   define		TRY_LOCK_MUTEX(_m_)	HM_Try_Lock_Mutex((_m_))

#	ifdef HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS

	HM_DEFINE_FUNCTION	void * HM_CDECL HM_Create_Mutex () {
		HM_Critical_Section_Mutex *      _hm_;
		ALLOCATE_MEMORY (_hm_, HM_Critical_Section_Mutex);
		memset (_hm_, 0, sizeof(HM_Critical_Section_Mutex));
		InitializeCriticalSection (&_hm_->cs);
		return (void *)_hm_;
	}

	HM_DEFINE_FUNCTION	void * HM_CDECL HM_Destroy_Mutex (void * m) {
		HM_Critical_Section_Mutex *	_hm_ = (HM_Critical_Section_Mutex*)m;
		DeleteCriticalSection (&_hm_->cs);
		FREE_MEMORY(_hm_, HM_Critical_Section_Mutex);
		return _hm_;
	}

	HM_DEFINE_FUNCTION int HM_Try_Lock_Mutex(void * m) {

		HM_Critical_Section_Mutex * hm = (HM_Critical_Section_Mutex*)m;
		HThreadID me = THREAD_ID();

		if (hm->thread != me) {                        
			if (TryEnterCriticalSection(&hm->cs) == 0)
				return 0;
			hm->thread = me;
		}

		++hm->use_count;
		return 1;
	}
#	else 
	HM_DEFINE_FUNCTION	void * HM_CDECL HM_Create_Mutex ();
	HM_DEFINE_FUNCTION	void * HM_CDECL HM_Destroy_Mutex (void * m);
	HM_DECLARE_FUNCTION int HM_CDECL HM_Try_Lock_Mutex (void * m);
#	endif

#	endif

#	define	THREAD_FUNCTION(_funct_) \
				HM_DECLARE_FUNCTION unsigned int __stdcall	_hthread_##_funct_(void *); \
				HM_DEFINE_FUNCTION unsigned int __stdcall	_hthread_##_funct_(void * _thread_data_)
#	define	THREAD_FUNCTION_DATA(_cast_)					(_cast_)_thread_data_ 
#	define	THREAD_FUNCTION_CALL(_funct_,_data_)			_hthread_##_funct_((void*)_data_)

#else
#   include <pthread.h>
	typedef pthread_t	HThreadID;			/* unique thread identifier */
	typedef pthread_t	HThreadHandle;		/* unique thread handle */

	typedef void *	HMutex;					/* exclusive (single thread) access */
	typedef void *	HRWLock;				/* multiple readers, exclusive writer */

#	define	THREAD_ID()						(HThreadID)pthread_self()

#	define  CREATE_THREAD(_handle_,_funct_,_data_)	pthread_create((pthread_t *)&_handle_, 0, _hthread_##_funct_, (void*)_data_)
#	define	JOIN_THREAD(_handle_)					pthread_join(*(pthread_t*)&_handle_, 0)
#	define	THREAD_FUNCTION(_funct_) \
				HM_DECLARE_FUNCTION void *			_hthread_##_funct_(void *); \
				HM_DEFINE_FUNCTION void *			_hthread_##_funct_(void * _thread_data_)
#	define	THREAD_FUNCTION_DATA(_cast_)			(_cast_)_thread_data_ 
#	define	THREAD_FUNCTION_CALL(_funct_,_data_)	_hthread_##_funct_((void*)_data_)

#	ifdef OSX_SYSTEM
#		define BAD_SEM_INIT
#	endif

#	ifndef BAD_SEM_INIT
#		include <semaphore.h>
		typedef	sem_t	HM_Semaphore;

#		define	HM_CONSTRUCT_SEMAPHORE(_s_, _l_, _v_)	sem_init (&(_s_), 0, _v_)
#		define	HM_DESTRUCT_SEMAPHORE(_s_)				sem_destroy (&(_s_))
#		define	HM_RELEASE_SEMAPHORE(_s_, _n_)			do {					\
															int _ii_ = _n_;		\
															while (_ii_-- > 0)	\
																sem_post (&(_s_));\
														} while (0)
#		define	HM_WAIT_SEMAPHORE(_s_)					while (0 != sem_wait (&(_s_)))

#		define	HM_TRY_WAIT_SEMAPHORE(_s_)				sem_trywait(&(_s_)) == 0	

#	else

#   ifdef OSX_SYSTEM
#       include <mach/mach.h>
#       include <mach/semaphore.h>
#       include <mach/task.h>
	
		typedef semaphore_t HM_Semaphore;

#       define  HM_CONSTRUCT_SEMAPHORE(_s_, _l_, _v_)   semaphore_create(mach_task_self(), &_s_, SYNC_POLICY_FIFO, _v_)
#       define  HM_DESTRUCT_SEMAPHORE(_s_)              semaphore_destroy(mach_task_self(), _s_)
#       define  HM_RELEASE_SEMAPHORE(_s_, _n_)          do {					\
															int _ii_ = _n_;		\
															while (_ii_-- > 0)	\
																semaphore_signal ((_s_));\
														} while (0)
#       define  HM_WAIT_SEMAPHORE(_s_)                   while (0 != semaphore_wait((_s_)))


#   else 

		/* semaphore emulation on pthreads */
		typedef struct hm_semaphore_s {
			pthread_mutex_t count_lock;
			int count;
		} HM_Semaphore;

#		define	HM_CONSTRUCT_SEMAPHORE(_s_, _l_, _v_)	do { \
															pthread_mutex_init (&(_s_).count_lock, 0); \
															(_s_).count = _v_; \
														} while (0)

#		define	HM_DESTRUCT_SEMAPHORE(_s_)				do { \
															pthread_mutex_destroy (&(_s_).count_lock); \
														} while (0)

#		define	HM_RELEASE_SEMAPHORE(_s_, _n_)			do { \
															pthread_mutex_lock (&(_s_).count_lock); \
															(_s_).count += _n_; \
															pthread_mutex_unlock (&(_s_).count_lock); \
														} while (0)

#		define	HM_WAIT_SEMAPHORE(_s_)					do { \
															pthread_mutex_lock (&(_s_).count_lock); \
															if ((_s_).count > 0) { \
																(_s_).count--; \
																pthread_mutex_unlock (&(_s_).count_lock); \
																break; \
															} \
															pthread_mutex_unlock (&(_s_).count_lock); \
															MILLI_SECOND_SLEEP(1); \
														} while (1)
#   endif // OSX_SYSTEM

	typedef struct {
		pthread_mutex_t mutex;
		HThreadID       thread;
		int             use_count;
	} HM_PThread_Mutex;


#   define      CREATE_MUTEX(_m_)                                               \
					do {                                                        \
						HM_PThread_Mutex *      _hm_;							\
						ALLOCATE_MEMORY (_hm_, HM_PThread_Mutex);				\
						memset (_hm_, 0, sizeof(HM_PThread_Mutex));				\
						pthread_mutex_init(&_hm_->mutex, 0);       				\
						_m_ = (void*)_hm_;                                      \
						_H_MUTEX_DEBUG_TRACE("CREATE_MUTEX:",THREAD_ID(),_m_);  \
					} while(0)

#   define      DESTROY_MUTEX(_m_)                                              \
					do {                                                        \
						HM_PThread_Mutex *      _hm_ = (HM_PThread_Mutex*)_m_;	\
						_H_MUTEX_DEBUG_TRACE("DESTROY_MUTEX:",THREAD_ID(),_m_); \
						pthread_mutex_destroy(&_hm_->mutex);       				\
						FREE_MEMORY(_hm_, HM_PThread_Mutex);					\
						_m_ = 0;                                                \
					} while(0)

#   define      LOCK_MUTEX(_m_)                                                 \
				do {                                                            \
					HM_PThread_Mutex * _hm_ = (HM_PThread_Mutex*)_m_;			\
					HThreadID _me_ = THREAD_ID();                               \
					if (_hm_->thread != _me_) {                                 \
						_H_MUTEX_DEBUG_TRACE("LOCK_MUTEX: before",_me_,_hm_);   \
						pthread_mutex_lock(&_hm_->mutex);			            \
						_hm_->thread = _me_;                                    \
						_H_MUTEX_DEBUG_TRACE("LOCK_MUTEX: after",_me_,_hm_);    \
					}                                                           \
					++_hm_->use_count;                                          \
				} while(0)

#   define      UNLOCK_MUTEX(_m_)                                               \
				do {                                                            \
					HM_PThread_Mutex * _hm_ = (HM_PThread_Mutex*)_m_;			\
					HThreadID _me_ = THREAD_ID();                               \
					_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: before",_me_,_hm_);     \
					if (_hm_->thread != _me_) {                                 \
						_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: failed",_me_,_hm_); \
						HMUTEX_ERROR ("Mutex non-owner release");               \
					} else if (--_hm_->use_count == 0) {                        \
						_hm_->thread = 0;                                       \
						pthread_mutex_unlock(&_hm_->mutex);						\
						_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: after",_me_,_hm_);  \
					}                                                           \
				} while(0)

#   define		MUTEX_OWNER(_m_)	(((HM_PThread_Mutex *)(_m_))->thread)
#   define		MUTEX_NESTING(_m_)	(((HM_PThread_Mutex *)(_m_))->use_count)

#   define		TRY_LOCK_MUTEX(_m_)	HM_Try_Lock_Mutex((_m_))

#	ifdef HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
	HM_DEFINE_FUNCTION int HM_Try_Lock_Mutex(void * m) {

		HM_PThread_Mutex * hm = (HM_PThread_Mutex*)m;
		HThreadID me = THREAD_ID();

		if (hm->thread != me) {                        
			if (pthread_mutex_trylock(&hm->mutex) != 0)
				return 0;
			hm->thread = me;
		}

		++hm->use_count;
		return 1;
	}
#	else 
	HM_DECLARE_FUNCTION int HM_CDECL HM_Try_Lock_Mutex (void * m);
#	endif

#endif

#ifndef CREATE_MUTEX

	/* Mutex is a simple single (but recursive) access mechanism */
	typedef	struct {
		HM_Semaphore	control;		/* single-access "mutex" */
		HThreadID		thread;			/* thread which has use of this mutex */
		int				use_count;		/* recursive reference by this thread */
	}	HM_Semaphore_Mutex;

				/* small enough to use macros */
#	define		CREATE_MUTEX(_m_)														\
					do {																\
						HM_Semaphore_Mutex *		_hm_;								\
						ALLOCATE_MEMORY (_hm_, HM_Semaphore_Mutex);						\
						memset (_hm_, 0, sizeof (HM_Semaphore_Mutex));					\
						HM_CONSTRUCT_SEMAPHORE (_hm_->control, 1, 1);					\
						_m_ = (void *)_hm_;												\
						_H_MUTEX_DEBUG_TRACE("CREATE_MUTEX:",THREAD_ID(),_m_);			\
					} while (0)
#	define		DESTROY_MUTEX(_m_)														\
					do {																\
						HM_Semaphore_Mutex *		_hm_ = (HM_Semaphore_Mutex *)_m_;	\
						_H_MUTEX_DEBUG_TRACE("DESTROY_MUTEX:",THREAD_ID(),_m_);			\
						HM_DESTRUCT_SEMAPHORE (_hm_->control);							\
						FREE_MEMORY (_hm_, HM_Semaphore_Mutex);							\
						_m_ = 0;														\
					} while (0)
#	define		LOCK_MUTEX(_m_)															\
					do {																\
						HM_Semaphore_Mutex *		_hm_ = (HM_Semaphore_Mutex *)_m_;	\
						HThreadID		_me_ = THREAD_ID();								\
						if (_hm_->thread != _me_) {										\
							_H_MUTEX_DEBUG_TRACE("LOCK_MUTEX: before",_me_,_hm_);		\
							HM_WAIT_SEMAPHORE (_hm_->control);							\
							_hm_->thread = _me_;										\
							_H_MUTEX_DEBUG_TRACE("LOCK_MUTEX: after",_me_,_hm_);		\
						}																\
						++_hm_->use_count;												\
					} while (0)
#	define		UNLOCK_MUTEX(_m_)														\
					do {																\
						HM_Semaphore_Mutex *		_hm_ = (HM_Semaphore_Mutex *)_m_;	\
						HThreadID		_me_ = THREAD_ID();								\
						_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: before",_me_,_hm_);			\
						if (_hm_->thread != _me_) {										\
							_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: failed",_me_,_hm_);		\
							HMUTEX_ERROR ("Mutex non-owner release");					\
						} else if (--_hm_->use_count == 0) {							\
							_hm_->thread = 0;											\
							HM_RELEASE_SEMAPHORE (_hm_->control, 1);					\
							_H_MUTEX_DEBUG_TRACE("UNLOCK_MUTEX: after",_me_,_hm_);		\
						}																\
					} while (0)

#   define		MUTEX_OWNER(_m_)	(((HM_Semaphore_Mutex *)(_m_))->thread)
#   define		MUTEX_NESTING(_m_)	(((HM_Semaphore_Mutex *)(_m_))->use_count)

#   define		TRY_LOCK_MUTEX(_m_)	HM_Try_Lock_Mutex((_m_))

#	ifdef HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
	HM_DEFINE_FUNCTION int HM_Try_Lock_Mutex(void * m) {

		HM_Semaphore_Mutex * hm = (HM_Semaphore_Mutex*)m;
		HThreadID me = THREAD_ID();

		if (hm->thread != me) {                        
			if (!HM_TRY_WAIT_SEMAPHORE(hm->control))
				return 0;
			hm->thread = me;
		}

		++hm->use_count;
		return 1;
	}
#	else 
	HM_DECLARE_FUNCTION int HM_CDECL HM_Try_Lock_Mutex (void * m);
#	endif

#endif
#endif


/* Lock is a complex, many reader, single (nested) writer mechanism */
/* need to simulate complex read/write lock using available objects */

#define HM_Per_Thread_Flag_NONE  0x0000
#define HM_Per_Thread_Flag_SHARE 0x0001

typedef struct {
		HThreadID	id;			/* thread with any active or pending locks */
		int			state;		/* each bit on if corresponding lock is for write */
		short		flags;
		short		use_count;	/* number of locks for this thread */
}	HM_Per_Thread;

typedef	struct {
	HM_Per_Thread *	threads;

	HMutex		control;		/* single-access mutex */
	HM_Semaphore	read, write;			/* semaphores */
	int			active_readers, pending_readers;
	int			active_writers, pending_writers;
	int			active_threads, max_threads;
	int			active_shares;
}		HM_Lock;


#define		HM_LOCK_STATE_NONE			0x0000		// no locks
#define		HM_LOCK_STATE_READ			0x0001		// current thread has read access
#define		HM_LOCK_STATE_WRITE			0x0002		// current thread has write access
#define		HM_LOCK_STATE_SHARED		0x0004		// current thread has shared access
#define		HM_LOCK_STATE_OTHER_READ	0x0010		// read access on other thread(s)
#define		HM_LOCK_STATE_OTHER_WRITE	0x0020		// write access on another thread
#define		HM_LOCK_STATE_OTHER_SHARED	0x0040		// shared access on other thread(s)



#define		CREATE_LOCK(_l_)				_l_ = HM_Create_Lock (HMUTEX_INITIAL_THREADS,	__FILE__, __LINE__)
#define		DESTROY_LOCK(_l_)				_l_ = HM_Destroy_Lock ((HM_Lock *)_l_,			__FILE__, __LINE__)
#define		CHECK_LOCK(_l_)					HM_Check_Lock ((HM_Lock *)_l_,					__FILE__, __LINE__)

#define		READ_LOCK(_l_, _r_)				HM_Read_Lock ((HM_Lock *)_l_, _r_,				__FILE__, __LINE__)
#define		SHARE_READ_LOCK(_l_, thread_id)	HM_Share_Read_Lock ((HM_Lock *)_l_,	thread_id,	__FILE__, __LINE__)
#define		WRITE_LOCK(_l_, _r_)			HM_Write_Lock ((HM_Lock *)_l_, _r_,				__FILE__, __LINE__)
#define		UNLOCK_LOCK(_l_)				HM_Unlock_Lock ((HM_Lock *)_l_,					__FILE__, __LINE__)

//Begin_HMUTEX_NAMESPACE_Namespace

HM_DECLARE_FUNCTION void *	HM_CDECL	HM_Create_Lock (int m, char const * f, int l);
HM_DECLARE_FUNCTION void *	HM_CDECL	HM_Destroy_Lock (HM_Lock * hl, char const * f, int l);
HM_DECLARE_FUNCTION int		HM_CDECL	HM_Check_Lock (HM_Lock * hl, char const * f, int l);
HM_DECLARE_FUNCTION int		HM_CDECL	HM_Read_Lock (HM_Lock * hl, int r, char const * f, int l);
HM_DECLARE_FUNCTION void	HM_CDECL	HM_Share_Read_Lock (HM_Lock * hl, HThreadID id, char const * f, int l);
HM_DECLARE_FUNCTION int		HM_CDECL	HM_Write_Lock (HM_Lock * hl, int r, char const * f, int l);
HM_DECLARE_FUNCTION int		HM_CDECL	HM_Unlock_Lock (HM_Lock * hl, char const * f, int l);

#ifdef HMUTEX_IMPLEMENT_LOCK_FUNCTIONS

#	ifdef H_RW_LOCK_DEBUG_TRACE
		static void lock_trace (
			char const *	tag,
			HThreadID		thread_id,
			HM_Lock *		lock,
			int				use_count,
			char const *	source_file,
			int				source_line) {
			char			message[H_DEBUG_MESSAGE_LENGTH];
			char *			buffer = message;

#ifdef H_DEBUG_TRACE_RINGBUFFER		// prepare to put message into memory
			int				index = debug_ringbuffer_index++;

			if (debug_ringbuffer_index >= H_DEBUG_RINGBUFFER_COUNT)
				debug_ringbuffer_index = 0;
			if (debug_ringbuffers == 0) {
				int			i;

				ALLOCATE_MEMORY_ARRAY (debug_ringbuffers, H_DEBUG_RINGBUFFER_COUNT, char *);
				for (i = 0; i < H_DEBUG_RINGBUFFER_COUNT; i++) {
					ALLOCATE_MEMORY_ARRAY (debug_ringbuffers[i], H_DEBUG_RINGBUFFER_LEN, char);
				}
			}

			debug_ringbuffers[debug_ringbuffer_index][0] = '\0';
			buffer = debug_ringbuffers[index];
#endif

			sprintf (buffer,  "%p %p %s", (void*)thread_id, (void*)lock, tag); 
			buffer += strlen(buffer);
			if (use_count >= 0) {
				HM_Lock * ll = (HM_Lock *)lock;
				if (use_count == 999)
					sprintf (buffer," (use: - ");
				else
					sprintf (buffer," (use: %d ", use_count);
				buffer += strlen(buffer);
				sprintf (buffer,
						"readers: %d / %d / %d  writers : %d / %d  active threads: %d)", 
						 ll->active_readers, ll->pending_readers, ll->active_shares,
						 ll->active_writers, ll->pending_writers, ll->active_threads);
				buffer += strlen(buffer);
			}
			if (source_file)
				sprintf (buffer, " %s %d", source_file, source_line);
			else if (source_line)
				sprintf (buffer, " [%d]", source_line);
			//buffer += strlen(buffer);


#ifndef H_DEBUG_TRACE_RINGBUFFER	// append message to file
			FILE *			hmutex_debug_trace_file;
			static HMutex	file_synch = 0;

			if (file_synch == 0)
				 CREATE_MUTEX (file_synch);

			LOCK_MUTEX (file_synch);

			hmutex_debug_trace_file = fopen (H_MUTEX_DEBUG_TRACE_FILE_NAME, "a+");

			if (hmutex_debug_trace_file != 0) {
				fprintf (hmutex_debug_trace_file, "%s\n", message);
				fclose (hmutex_debug_trace_file);
			}
			else
				fprintf(stderr, "opening %s failed.\n", H_MUTEX_DEBUG_TRACE_FILE_NAME);

			UNLOCK_MUTEX (file_synch);
#endif
		}

#		define	_H_RW_LOCK_DEBUG_TRACE(_tag_,_id_,_synch_,_d_,_f_,_l_) lock_trace(_tag_,_id_,_synch_,_d_,_f_,_l_)
#	else
#		define	_H_RW_LOCK_DEBUG_TRACE(_tag_,_id_,_synch_,_d_,_f_,_l_)	(void)_tag_,(void)_id_,(void)_synch_,(void)_d_,(void)_f_,(void)_l_
#	endif



	HM_DEFINE_FUNCTION	void * HM_CDECL HM_Create_Lock (
		int				max_threads,
		char const *	f,
		int				l) {
		HM_Lock *		hl;

		ALLOCATE_MEMORY (hl, HM_Lock);
		memset (hl, 0, sizeof (HM_Lock));
		hl->max_threads = max_threads;
		ALLOCATE_MEMORY_ARRAY (hl->threads, max_threads, HM_Per_Thread);
		memset (hl->threads, 0, max_threads * sizeof (HM_Per_Thread));

		CREATE_MUTEX (hl->control);	/* "mutex" */
		HM_CONSTRUCT_SEMAPHORE (hl->read, HMUTEX_MAX_CONCURRENT_READERS, 0);
		HM_CONSTRUCT_SEMAPHORE (hl->write, 1, 0);

		_H_RW_LOCK_DEBUG_TRACE("HM_Create_Lock:",THREAD_ID(),hl,999, f, l);

		return (void *)hl;
	}

	HM_DEFINE_FUNCTION	void * HM_CDECL HM_Destroy_Lock (
		HM_Lock *		hl,
		char const *	f,
		int				l) {

		_H_RW_LOCK_DEBUG_TRACE("HM_Destroy_Lock:",THREAD_ID(),hl,999, f, l);

		DESTROY_MUTEX (hl->control);
		HM_DESTRUCT_SEMAPHORE (hl->read);
		HM_DESTRUCT_SEMAPHORE (hl->write);

		FREE_MEMORY_ARRAY (hl->threads, hl->max_threads, HM_Per_Thread);
		FREE_MEMORY (hl, HM_Lock);

		return 0;
	}


#	define	EXPAND_PER_THREAD_DATA(hl)	do {										\
			HM_Per_Thread *			old_threads = hl->threads;						\
			int						old_max = hl->max_threads;						\
			hl->max_threads *= 2;													\
			ALLOCATE_MEMORY_ARRAY (hl->threads, hl->max_threads, HM_Per_Thread);	\
			memset (hl->threads, 0, hl->max_threads*sizeof(HM_Per_Thread));			\
			memcpy (hl->threads, old_threads, old_max*sizeof(HM_Per_Thread));		\
			FREE_MEMORY_ARRAY (old_threads, old_max, HM_Per_Thread);				\
		} while (0)


	HM_DEFINE_FUNCTION	int HM_CDECL HM_Read_Lock (
		HM_Lock *		hl,
		int				required,
		char const *	f,
		int				l) {
		HThreadID		me = THREAD_ID();
		char const *	error = 0;
		int				mine = 0;
		int				use_count = 0;
		int				ii;

		LOCK_MUTEX (hl->control);	/**********************************************/
		_H_RW_LOCK_DEBUG_TRACE((required ? "HM_Read_Lock: grab control" : "HM_Try_R_Lock: grab control") , me, hl, 999, f,l);

		for (ii=0; ii < hl->active_threads; ii++) {
			if (hl->threads[ii].id == me) {		/* already have an entry */
				if (hl->threads[ii].state != 0 ||	/* this thread has a write lock or */
					hl->active_readers > 0)	{		/*   it must be a reader */
					_H_RW_LOCK_DEBUG_TRACE("HM_Read_Lock: already mine", me, hl, -1, 0,0);
					mine = 1;	/* the thread already has a sufficient lock */
				}
				else
					error = "Read: Impossible state";

				/* nested read flag is zero -- no change to the state */
				/* but record another nested reference on this thread */
				use_count = ++hl->threads[ii].use_count;

				if (use_count >= 32)
					error = "Read: Nested too deep";

				break;
			}
		}

		if (ii == hl->active_threads) {	/* new thread */

			int record = 0;

			if (hl->active_writers + hl->pending_writers == 0) {
				_H_RW_LOCK_DEBUG_TRACE("HM_Read_Lock: immediate acquire", me, hl, -1, 0,0);
				++hl->active_readers;
				mine = 1;
				record = 1;
			}
			else if (required) {
				++hl->pending_readers;
				record = 1;
			}

			if (record) {

				if (ii == hl->max_threads)
					EXPAND_PER_THREAD_DATA (hl);

				hl->threads[ii].id = me;
				use_count = 1;
				hl->threads[ii].use_count = 1;
				hl->threads[ii].flags = HM_Per_Thread_Flag_NONE;
				/* hl->threads[ii].state = 0; */	/* initial read lock */
				++hl->active_threads;
			}
		}

		_H_RW_LOCK_DEBUG_TRACE("HM_Read_Lock: drop control", me, hl, use_count, 0,0);
		UNLOCK_MUTEX (hl->control);	/**********************************************/

		if (error)
			HMUTEX_ERROR (error);

		if (!mine) {
			if (required) {
				_H_RW_LOCK_DEBUG_TRACE("HM_Read_Lock: wait read", me, hl, -1, 0,0);
				HM_WAIT_SEMAPHORE (hl->read);
				_H_RW_LOCK_DEBUG_TRACE("HM_Read_Lock: have read", me, hl, -1, 0,0);
			}
			else
				_H_RW_LOCK_DEBUG_TRACE("HM_Read_Lock: not acquired", me, hl, -1, 0,0);
		}

		return mine;
	}


	HM_DEFINE_FUNCTION	void HM_CDECL HM_Share_Read_Lock (
		HM_Lock *		hl,
		HThreadID		it,
		char const *	f,
		int				l) {
		HThreadID		me = THREAD_ID();
		char const *	error = 0;
		int				its = 0;
		int				mine = 0;
		int				use_count = 0;
		int				ii;

		LOCK_MUTEX (hl->control);	/**********************************************/
		_H_RW_LOCK_DEBUG_TRACE("HM_Share_Read_Lock: grab control", me, hl, 999, f,l);

		for (ii=0; ii < hl->active_threads; ii++) {
			if (hl->threads[ii].id == it) {		/* find its entry */
				if (hl->threads[ii].state != 0 ||	/* that thread has a write lock or */
					hl->active_readers > 0)	{		/*   it must be a reader */
					_H_RW_LOCK_DEBUG_TRACE("HM_Share_Read_Lock: found it", it, hl, -1, 0,0);
					its = 1;	/* the thread has a sufficient lock */
				}
			}
			else if (hl->threads[ii].id == me) {		/* find my entry */
				if (hl->threads[ii].state != 0 ||	/* that thread has a write lock or */
					hl->active_readers > 0)	{		/*   it must be a reader */
					_H_RW_LOCK_DEBUG_TRACE("HM_Share_Read_Lock: have one???", me, hl, -1, 0,0);
					mine = 1;	/* the thread has a sufficient lock */
				}
			}
		}

		if (its == 0)
			error = "it has no lock";
		else if (mine == 1)
			error = "can't share because we have a lock already";
		else {
			if (ii == hl->max_threads)
				EXPAND_PER_THREAD_DATA (hl);

			hl->threads[ii].id = me;
			use_count = 1;
			hl->threads[ii].use_count = 1;
			hl->threads[ii].flags = HM_Per_Thread_Flag_SHARE;
			/* hl->threads[ii].state = 0; */	/* initial read lock */
			++hl->active_shares;
			++hl->active_threads;
			++hl->active_readers;
		}

		_H_RW_LOCK_DEBUG_TRACE("HM_Share_Read_Lock: drop control", me, hl, use_count, 0,0);
		UNLOCK_MUTEX (hl->control);	/**********************************************/

		if (error)
			HMUTEX_ERROR (error);
	}


	HM_DEFINE_FUNCTION	int HM_CDECL HM_Write_Lock (
		HM_Lock *		hl,
		int				required,
		char const *	f,
		int				l) {
		HThreadID		me = THREAD_ID();
		char const *	error = 0;
		int				mine = 0;
		int				simple = 0;
		int				ii;
		int				use_count = 0;

		LOCK_MUTEX (hl->control);	/**********************************************/
		_H_RW_LOCK_DEBUG_TRACE((required ? "HM_Write_Lock: grab control" : "HM_Try_W_Lock: grab control"), me,hl,999,f,l);

		for (ii=0; ii < hl->active_threads; ii++) {
			if (hl->threads[ii].id == me) {
				int		writer = 1;		/* assume we get it */

				if (hl->threads[ii].state != 0) {
					_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: already mine", me,hl,-1,0,0);
					mine = 1;			/* already have a write lock */
				}
				else {	/* upgrade */

					/* unlock the current read to avoid deadlock */
					if (--hl->active_readers - hl->active_shares == 0) {
						/* only reader -- immediate upgrade */
						_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: immediate upgrade", me,hl,-1,0,0);
						++hl->active_writers;
						mine = 1;
					}
					else if (!required) {
						_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: upgrade failed", me,hl,-1,0,0);
						++hl->active_readers;	/* restore */
						writer = 0;
					}
					else
						++hl->pending_writers;
				}
				if (writer) {
					
					/* record the nested write flag */
					hl->threads[ii].state |= 1 << hl->threads[ii].use_count;
					/* and record another nested reference on this thread */

					use_count = ++hl->threads[ii].use_count;

					if (use_count >= 32)
						error = "Write: Nested too deep";
				}

				break;
			}
		}

		if (ii == hl->active_threads) {	/* new thread */

			int record = 0;

			if (hl->active_writers + hl->pending_writers + hl->active_readers == 0) {
				_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: immediate acquire", me,hl,-1,0,0);
				++hl->active_writers;
				mine = 1;
				record = 1;
			}
			else if (required) {
				++hl->pending_writers;
				record = 1;
			}

			if (record) {
				if (ii == hl->max_threads)
					EXPAND_PER_THREAD_DATA (hl);

				hl->threads[ii].id = me;
				use_count = 1;
				hl->threads[ii].use_count = 1;
				hl->threads[ii].flags = HM_Per_Thread_Flag_NONE;
				hl->threads[ii].state = 1;	/* initial write lock */
				simple = 1;
				++hl->active_threads;
			}
		}

		_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: drop control", me, hl, use_count, 0,0);
		UNLOCK_MUTEX (hl->control);	/**********************************************/

		if (error)
			HMUTEX_ERROR (error);

		if (!mine) {
			if (required) {
				_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: wait write", me,hl,-1,0,0);
				HM_WAIT_SEMAPHORE (hl->write);
				_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: have write", me,hl,-1,0,0);
			}
			else
				_H_RW_LOCK_DEBUG_TRACE("HM_Write_Lock: not acquired", me,hl,-1,0,0);
		}

		return mine | simple;	/* already had the write lock (or immediately upgraded), or safely acquired it */
	}

	HM_DEFINE_FUNCTION	int HM_CDECL HM_Unlock_Lock (
		HM_Lock *		hl,
		char const *	f,
		int				l) {
		HThreadID		me = THREAD_ID();
		char const *	error = 0;
		int				reread = 0;
		int				ii;
		int				release_reader_limit = 0;
		int				use_count = 0;

		LOCK_MUTEX (hl->control);	/**********************************************/
		_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: grab control", me,hl,999,f,l);

		for (ii=0; ii<hl->active_threads; ii++) {
			if (hl->threads[ii].id == me) {
				int		old_state = hl->threads[ii].state;

				use_count = --hl->threads[ii].use_count;

				_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: decrement", me,hl,use_count,0,0);

				if (use_count == 0) {	/* if this thread done with this lock */
					--hl->active_threads;

					if (hl->threads[ii].flags & HM_Per_Thread_Flag_SHARE)
						--hl->active_shares;

					/* shift top entry down to fill space */
					if (ii < hl->active_threads) {
						hl->threads[ii].id = hl->threads[hl->active_threads].id;
						hl->threads[ii].use_count = hl->threads[hl->active_threads].use_count;
						hl->threads[ii].flags = hl->threads[hl->active_threads].flags;
						hl->threads[ii].state = hl->threads[hl->active_threads].state;
					}
										
					/* and clear out unused top entry */
					hl->threads[hl->active_threads].id = 0;
					hl->threads[hl->active_threads].use_count = 0;
					hl->threads[hl->active_threads].flags = HM_Per_Thread_Flag_NONE;
					hl->threads[hl->active_threads].state = 0;

					/* now do a "normal" release */
					if (old_state != 0) {		/* should only be "== 1" in any case */
						--hl->active_writers;	/* this thread is no longer the writer */

						if (hl->pending_writers > 0) {	/* unleash a waiting writer */
							_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: writer, unleash write", me,hl,-1,0,0);
							--hl->pending_writers;
							++hl->active_writers;
							HM_RELEASE_SEMAPHORE (hl->write, 1);
						}
						else							/* or unleash a bunch of readers */
							release_reader_limit = HMUTEX_MAX_CONCURRENT_READERS;
					}
					else {
						/* if releasing the last active reader, see if we have a writer to unleash */
						if (--hl->active_readers - hl->active_shares == 0 && hl->pending_writers > 0) {
							_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: reader, unleash write", me,hl,-1,0,0);
							--hl->pending_writers;
							++hl->active_writers;
							HM_RELEASE_SEMAPHORE (hl->write, 1);
						}
					}
				}
				else {
					hl->threads[ii].state &= ~(1 << hl->threads[ii].use_count);

					if (old_state != 0 && hl->threads[ii].state == 0) {
						/* downgrade  - change this thread into a reader */
						--hl->active_writers;

						_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: downgrade", me,hl,-1,0,0);

						if (hl->pending_writers > 0) {	/* unleash a waiting writer */
							_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: unleash write", me,hl,-1,0,0);
							--hl->pending_writers;
							++hl->active_writers;
							++hl->pending_readers;		/* this thread will have to wait now */
							HM_RELEASE_SEMAPHORE (hl->write, 1);
							reread = 1;
						}
						else {
							++hl->active_readers;		/* this thread can go ahead */

							/* limited release -- let's share if we can */
							if (hl->pending_writers == 0)
								release_reader_limit = HMUTEX_MAX_CONCURRENT_READERS-1; /* less 1 for this thread */
						}
					}
				}

				break;
			}
			else if (hl->threads[ii].id == 0) {
				error = "Unlock: thread not found";
				break;
			}
		}

		/* unleash some waiting readers if we can */
		if (release_reader_limit > 0 && hl->pending_readers > 0) {
			int		number_to_release = hl->pending_readers;

			if (number_to_release > release_reader_limit)
				number_to_release = release_reader_limit;

			hl->pending_readers -= number_to_release;
			hl->active_readers += number_to_release;
			_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: unleash read(s)", me,hl,-1,0,number_to_release);	/* Note: number, not lock */
			HM_RELEASE_SEMAPHORE (hl->read, number_to_release);
		}

		_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: drop control", me,hl,use_count,0,0);
		UNLOCK_MUTEX (hl->control);	/**********************************************/

		if (error)
			HMUTEX_ERROR (error);

		if (reread) {
			_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: wait read", me,hl,-1,0,0);
			HM_WAIT_SEMAPHORE (hl->read);
			_H_RW_LOCK_DEBUG_TRACE("HM_Unlock_Lock: have read", me,hl,-1,0,0);
		}

		return !reread;
	}

	HM_DEFINE_FUNCTION	int HM_CDECL HM_Check_Lock (
		HM_Lock *		hl,
		char const *	f,
		int				l) {
		HThreadID		me = THREAD_ID();
		int				state = HM_LOCK_STATE_NONE;
		int				ii;
		int				use_count = 0;

		LOCK_MUTEX (hl->control);	/**********************************************/
		_H_RW_LOCK_DEBUG_TRACE("HM_Check_Lock: grab control", me,hl,-1,f,l);

		for (ii=0; ii<hl->active_threads; ii++) {
			int			mask = ~0 & ((1U << hl->threads[ii].use_count) - 1);

			if (hl->threads[ii].id == me) {
				if (hl->threads[ii].flags & HM_Per_Thread_Flag_SHARE)
					state |= HM_LOCK_STATE_SHARED;
				if ((hl->threads[ii].state & mask) != mask)
					state |= HM_LOCK_STATE_READ;
				if (hl->threads[ii].state != 0)
					state |= HM_LOCK_STATE_WRITE;
				use_count = hl->threads[ii].use_count;
				state |= (use_count << 16);
			}
			else {
				if (hl->threads[ii].flags & HM_Per_Thread_Flag_SHARE)
					state |= HM_LOCK_STATE_OTHER_SHARED;
				if ((hl->threads[ii].state & mask) != mask)
					state |= HM_LOCK_STATE_OTHER_READ;
				if (hl->threads[ii].state != 0)
					state |= HM_LOCK_STATE_OTHER_WRITE;
			}
		}

		_H_RW_LOCK_DEBUG_TRACE("HM_Check_Lock: drop control", me,hl,use_count,0,0);
		UNLOCK_MUTEX (hl->control);	/**********************************************/

		return state;
	}

#endif	/* ifdef HMUTEX_IMPLEMENT_LOCK_FUNCTIONS */

//End_HMUTEX_NAMESPACE_Namespace

#endif	/* ifndef DISABLE_MUTEX */

#endif /* HOOPS_MUTEX_DEFINED */
