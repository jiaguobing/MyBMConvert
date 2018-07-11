// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once


#ifdef H_PACK_8
#	pragma pack(push)
#	pragma pack(8)
#endif


//////////////////////////////////////////////////////////////////////////


/*! The HMutexOOC class represents a mutex.
*/
class HMutexOOC {
public:
	/*! LockGuard locks a HMutexOOC instance until the LockGuard is destructed.
	*/
	class LockGuard {
	public:
		/*! Locks the input mutex.
			\param mutex The mutex to lock for the lifetime of LockGuard.
		*/
		LockGuard (HMutexOOC & mutex)
			: mutex(mutex)
		{
			mutex.Lock();
		}

		/*! Unlocks the mutex locked by the LockGuard constructor.
		*/
		~LockGuard ()
		{
			mutex.Unlock();
		}

	private:
		LockGuard (LockGuard &&); // disable
		LockGuard (LockGuard const &); // disable
		void operator= (LockGuard &&); // disable
		void operator= (LockGuard const &); // disable

	private:
		HMutexOOC & mutex;
	};

public:
	/*! Creates an unlocked mutex.
	*/
	HMutexOOC ();

	~HMutexOOC ();

	/*! Locks the mutex.
	*/
	void Lock ();

	/*! Unlocks the mutex.
	*/
	void Unlock ();

private:
	void * opaque_mutex;
};








