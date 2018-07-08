// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#ifndef OOC_LIB_BUILD
#	error "Do not include this file."
#endif

#include "hc.h"

#include "assertbreak.h"
#include "PointCloudAPI.h"

#include "BStream.h"
#include "BOpcodeHandler.h"

#include "hmutex.h"
#include "utf_utils.h"

#include <list>
#include <string>
#include <type_traits>
#include <vector>

#include <stdint.h>


//////////////////////////////////////////////////////////////////////////


# if !defined(__APPLE__) && defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8))
#	define OLD_GCC
#	define OVERRIDE 
# else
#	define OVERRIDE override
# endif


#ifdef _WIN32
#	if _MSC_VER < 1700
#		pragma warning(disable: 4480)	// Language extension: Specifying underlying enum type
#		pragma warning(disable: 4481)	// Language extension: Using OVERRIDE specifier
#	endif
#endif


class NodeData;
class PointCloud;
class PointCloudWorld;


namespace ooc
{
	int big_fseek (FILE * file, int64_t offset, int origin);
	OOC_API std::wstring get_working_directory ();
	std::wstring to_wstring (H_UTF8 const & file);
	std::wstring normalize_directory (wchar_t const * dir);
	std::list<std::wstring> split_any (std::wstring const & s, wchar_t const * delims);
	PointCloud & get_point_cloud (Env env);
	PointCloudWorld & get_point_cloud_world (Env env);
	

	std::wstring relativize (
		std::wstring const & path_to_make_relative,
		std::wstring const & reference_path);

	template <typename Iter>
	std::wstring join (Iter const begin, Iter const end, wchar_t const * joint)
	{
		std::wstring whole;
		Iter it = begin;
		while (it != end) {
			if (it != begin) {
				whole += joint;
			}
			whole += *it++;
		}
		return whole;
	}


	template <typename Iter>
	OOC_API bool is_absolute (Iter it, Iter end)
	{
		if (it == end) {
			return false;
		}
		int const first_code_point = *it;
		if (first_code_point == '/') {
			return true;
		}
#if _WIN32
		if (first_code_point == '\\') {
			return true;
		}
		++it;
		if (it == end) {
			return false;
		}
		int const second_code_point = *it;
		if (second_code_point == ':') {
			return true;
		}
#endif
		return false;
	}


	template <typename Iter>
	bool is_strictly_increasing (Iter it, Iter end)
	{
		ASSERT(it != end);
		auto prev = *it++;
		while (it != end) {
			if (prev >= *it) {
				return false;
			}
			prev = *it++;
		}
		return true;
	}


	inline bool size_t_lossy (uint64_t n)
	{
		static_assert(sizeof(uint32_t) <= sizeof(size_t), "size_t too small. code needs a new technique to work.");
		return sizeof(uint64_t) <= sizeof(size_t) ? false : n > 0xFFFFFFFFu;
	}


	template <typename T>
	struct HashKeyAdapter
	{
		inline size_t operator() (T const & item) const
		{
			return item.Hash();
		}
	};


	class Mutex {
	public:
		Mutex ();
		~Mutex ();
		void Lock ();
		void Unlock ();

	private:
		HMutex mutex;
	};


	template <typename T>
	class Buffer {
	public:
		Buffer ()
			: size(0)
		{}

		void Resize (size_t size) {
			this->size = size;
			buffer.reserve(size);
		}

		T * Data () {
			return buffer.data();
		}

		T const * Data () const {
			return buffer.data();
		}

		T & operator[] (size_t index) {
			ASSERT(index < size);
			return Data()[index];
		}

		T const & operator[] (size_t index) const {
			ASSERT(index < size);
			return Data()[index];
		}

		size_t Size () const {
			return size;
		}

	private:
		std::vector<T> buffer;
		size_t size;
	};


	class CompressingToolkit : public BStreamFileToolkit {
	public:
		CompressingToolkit ();
		virtual void Restart () OVERRIDE;
		virtual TK_Status WriteBuffer (char * buffer, int size) OVERRIDE;

		TK_Status StartCompression ();
		TK_Status StopCompression ();
		TK_Status StartDecompression ();
		TK_Status StopDecompression ();
		void UseFile (FILE * file);
		TK_Status SeekFromBeginning (uint64_t offset);
		TK_Status SeekFromEnd (uint64_t offset);
		int AmountWrittenToBuffer () const;
		uint64_t AmountWrittenToDisk () const;
		TK_Status FlushPendingData ();

	private:
		uint64_t amount_written_to_disk;
	};


	class TK_Hijack : public TK_Default {
	public:
		virtual TK_Status Execute (BStreamFileToolkit &) OVERRIDE;
		virtual TK_Status Read (BStreamFileToolkit & tk) OVERRIDE = 0;
		virtual void Reset () OVERRIDE = 0;
	};


	class HijackedToolkit : public CompressingToolkit {
	public:
		HijackedToolkit (TK_Hijack & handler);
		~HijackedToolkit ();

		virtual void Restart () OVERRIDE;

	private:
		HijackedToolkit (HijackedToolkit const &); // disable
		HijackedToolkit & operator= (HijackedToolkit const &); // disable

	private:
		BBaseOpcodeHandler * const default_object_save;
	};


	class MemoryUsage {
	public:
		MemoryUsage ()
			: memory_used(0)
		{}

		void operator+= (size_t amount)
		{
			ASSERT(memory_used + amount >= memory_used); // for overflow
			memory_used += amount;
		}

		void operator-= (size_t amount)
		{
			ASSERT(amount <= memory_used); // for underflow
			memory_used -= amount;
		}

		operator size_t () const
		{
			return memory_used;
		}

	private:
		size_t memory_used;
	};

	enum FileType {
		FileType_No_Extension,
		FileType_Unknown_Extension,
		FileType_PTX,
		FileType_PTS,
		FileType_XYZ,
	};


	enum LoadType {
		LoadType_Immediate,		// No populate callback -- shells loaded directly into segments
		LoadType_Synchronous,	// Uses populate callback -- once triggered => shell is loaded into segment
		LoadType_Asynchronous,	// Uses populate callback -- once triggered => shell is queued for loading (default)
	};


	enum LoadStatus {
		LoadStatus_Success,
		LoadStatus_Failure,
	};
}


#ifdef _MSC_VER
#	define USE_MSVC_INTRINSIC 1
#	ifdef _M_X64
extern "C" void __cdecl __faststorefence(void);
#		pragma intrinsic (__faststorefence)
#	else
extern "C" void __cdecl _mm_mfence(void);
#		pragma intrinsic (_mm_mfence)
#	endif
#endif


#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2)
#	define USE_GCC_INTRINSICS 1
#endif


inline void ReadWriteBarrier ()
{
#if defined (USE_MSVC_INTRINSIC)
#	ifdef _M_X64
	__faststorefence();
#	else
	_mm_mfence();
#endif
#elif defined (USE_GCC_INTRINSICS)
	__sync_synchronize();
#else
#	error "ReadWriteBarrier is not defined for this architecture."
#endif
}







