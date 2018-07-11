// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef URANDOM
#  include <stdlib.h>
#  define URANDOM() (unsigned int)rand()
#endif

template<typename T, typename OPS, bool ascending=true>

class template_sort {

public:

	static int const in_order = (ascending?-1:1);

	static bool is_sorted (T * thing, size_t left, size_t right) 
	{
		for (size_t i = left + 1; i <= right; i++) {
			if (OPS::compare(thing,i-1,i)*in_order < 0)
				return false;
		}
		return true;
	}

	static void	insert(T * thing, size_t left, size_t right) 
	{
		for (size_t i = left + 1; i <= right; i++) {
			size_t j = i;
			while (j > left && OPS::compare (thing,j-1,j)*in_order < 0) { 
				OPS::swap (thing, j, j-1);
				j--;
			}
		}
	}

	static void	partition (T * thing, size_t left, size_t right, int recursion_limit=1024) 
	{
		ASSERT(right >= left);

		if (right - left <= 1) {
			if (right <= left) 
				return;
			if (OPS::compare(thing,left,right)*in_order < 0)
				OPS::swap (thing,left,right);
			return;
		}
	
		if (recursion_limit == 0 || (right - left) <= 32) {
			insert(thing, left, right);
			return;
		}

#if 0
		size_t count = right - left;
		size_t sample1 = left + (URANDOM() % count);
		size_t sample2 = left + (URANDOM() % count);
		size_t sample3 = left + (URANDOM() % count);
#else
		size_t sample1 = left;
		size_t sample2 = (left+right) >> 1;
		size_t sample3 = right-1;
#endif

		if (OPS::compare(thing,sample1,sample2) < 0)
			if (OPS::compare(thing,sample1,sample3) < 0)
				if (OPS::compare(thing,sample2,sample3) < 0)
					OPS::swap (thing, sample2, right);
				else
					OPS::swap (thing, sample3, right);
			else
				OPS::swap (thing, sample1, right);
		else
			if (OPS::compare(thing,sample1,sample3) < 0)
				OPS::swap (thing, sample1, right);
			else
				if (OPS::compare(thing,sample2,sample3) < 0)
					OPS::swap (thing, sample3, right);
				else
					OPS::swap (thing, sample2, right);
		

		size_t i=left;
		size_t j=right-1;

		size_t pivot=right;
		size_t lower=left;
		size_t upper=right-1;

		while (1) {

			while (i < right && OPS::compare(thing,i,pivot)*in_order>0)
				i++;

			while (j > left && OPS::compare(thing,pivot,j)*in_order>0)
				j--;

			if (i >= j) 
				break;

			OPS::swap (thing, i, j);

			if (OPS::compare(thing,i,pivot) == 0) { 
				if (lower != i)
					OPS::swap (thing, lower, i); 
				lower++; 
				i++;
			}
			if (OPS::compare(thing,j,pivot) == 0) { 
				if (j != upper)
					OPS::swap (thing, j, upper); 
				upper--; 
				j--;
			}

		}

		if (i != right)
			OPS::swap (thing, i, right);

		if (i > lower) {
			j = i-1;
			for (size_t k=left; k<lower; k++, j--) {
				if (j < lower) {
					j = k;
					break;
				}
				if (k != j)
					OPS::swap (thing, k, j);
			}
			partition (thing, left, j, recursion_limit - 1);
		}
		
		i++;
		ASSERT(right-1 > upper || i <= right);
		for (size_t k=right-1; k>upper; k--, i++) {
			ASSERT(i <= right);
			if (i > upper) {
				i = k + 1;
				break;
			}
			if (i != k)
				OPS::swap (thing, i, k);
		}

		partition (thing, i, right, recursion_limit - 1);
	}

};


template<typename T, typename OPS>
INLINE void complex_sort(T *a, size_t count, bool ascending=true) 
{
	if (ascending == true) {
		template_sort<T,OPS,true>::partition (a, 0, count-1);
#ifdef HOOPS_DEBUG_BUILD
		bool sorted = template_sort<T,OPS,true>::is_sorted(a, 0, count-1); ASSERT(sorted == true);
#endif
	}
	else {
		template_sort<T,OPS,false>::partition (a, 0, count-1);
#ifdef HOOPS_DEBUG_BUILD
		bool sorted = template_sort<T,OPS,false>::is_sorted(a, 0, count-1); ASSERT(sorted == true);
#endif
	}
}

template<typename T>
class default_sort_ops {
public:
	INLINE static int compare(T * a, int i, int j) {
		return Compare(a[i],a[j]);
	}

	INLINE static void swap(T * a, int i, int j) {
		Swap(a[i],a[j]);
	}
};

template<typename T>
INLINE void simple_sort(T *a, size_t count, bool ascending=true) 
{
	complex_sort<T, default_sort_ops<T> >(a, count, ascending); 
}




/*
// ================================================================================================
// flip a float for sorting
//  finds SIGN of fp number.
//  if it's 1 (negative float), it flips all bits
//  if it's 0 (positive float), it flips the sign only
// ================================================================================================
*/

INLINE uint32_t FloatFlip(uint32_t f)
{
	int32_t t = f>>31;
	uint32_t mask = -t | 0x80000000;
	return f ^ mask;
}

/*
// ================================================================================================
// flip a float back (invert FloatFlip)
//  signed was flipped from above, so:
//  if sign is 1 (negative), it flips the sign bit back
//  if sign is 0 (positive), it flips all bits back
// ================================================================================================
*/
INLINE uint32_t IFloatFlip(uint32_t f)
{
	uint32_t mask = ((f >> 31) - 1) | 0x80000000;
	return f ^ mask;
}

/* ---- utils for accessing 11-bit quantities */
INLINE uint32_t bits_0(uint32_t x) {return (x & 0x7FF);}
INLINE uint32_t bits_1(uint32_t x) {return (x >> 11 & 0x7FF);}
INLINE uint32_t bits_2(uint32_t x) {return (x >> 22);}

template <typename T>
local void radix_sort_11(
	float *		fpriorities,
	float *		fpriorities_tmp,
	T *			payload,
	T *			payload_tmp,
	uint32_t *	b0, // must be kHist * 3 length workspace 6144
	size_t		count,
	bool		ascending)
{
	uint32_t *	priorities = (uint32_t*)fpriorities;
	uint32_t *	priorities_tmp = (uint32_t*)fpriorities_tmp;

	/* 3 histograms on the stack: */
	const size_t	kHist = 2048;
	//uint32_t		b0[6144]; /* kHist * 3  */
	uint32_t *		b1 = b0 + kHist;
	uint32_t *		b2 = b1 + kHist;

	for (size_t i = 0; i < kHist * 3; i++)
		b0[i] = 0;

	/* 1.  parallel histogramming pass */
	for (size_t i = 0; i < count; i++) {

		Prefetch(&priorities[i+32]);

		uint32_t fi = FloatFlip(priorities[i]);

		b0[bits_0(fi)] ++;
		b1[bits_1(fi)] ++;
		b2[bits_2(fi)] ++;
	}
	
	/* 2.  Sum the histograms -- each histogram entry records the number of values preceding itself. */
	{
		uint32_t sum0 = 0;
		uint32_t sum1 = 0;
		uint32_t sum2 = 0;

		for (size_t i = 0; i < kHist; i++) {

			uint32_t	tsum = b0[i] + sum0;
			b0[i] = sum0 - 1;
			sum0 = tsum;

			tsum = b1[i] + sum1;
			b1[i] = sum1 - 1;
			sum1 = tsum;

			tsum = b2[i] + sum2;
			b2[i] = sum2 - 1;
			sum2 = tsum;
		}
	}

	// pass 0: floatflip entire value, read/write histogram, write out flipped
	for (size_t i = 0; i < count; i++) {

		uint32_t	fi = FloatFlip(priorities[i]);
		uint32_t	pos = bits_0(fi);
		uint32_t	offset = ++b0[pos];

		ASSERT(offset<count);
		
		Prefetch(&priorities[i+32]);

		priorities_tmp[offset] = fi;

		T *	sour = &payload[i];
		T *	dest = &payload_tmp[offset]; 
		memcpy(dest, sour, sizeof(T));
	}

	// pass 1: read/write histogram, copy sorted -> array
	for (size_t i = 0; i < count; i++) {

		uint32_t si = priorities_tmp[i];
		uint32_t pos = bits_1(si);
		uint32_t offset = ++b1[pos];

		ASSERT(offset<count);
		
		Prefetch(&priorities_tmp[i+32]);

		priorities[offset] = si;

		T *	sour = &payload_tmp[i];
		T *	dest = &payload[offset]; 
		memcpy(dest, sour, sizeof(T));
	}

	// pass 2: read/write histogram, copy & flip out array -> sorted
	for (size_t i = 0; i < count; i++) {

		uint32_t ai = priorities[i];
		uint32_t pos = bits_2(ai);
		uint32_t offset = ++b2[pos];
		
		if (!ascending)
			offset = (uint32_t)count-1-offset;

		ASSERT(offset<count);

		Prefetch(&priorities[i+32]);

		priorities_tmp[offset] = IFloatFlip(ai);

		T *	sour = &payload[i];
		T *	dest = &payload_tmp[offset]; 
		memcpy(dest, sour, sizeof(T));
	}

	/* to write original: */
	memcpy(priorities, priorities_tmp, count * sizeof(float));
	memcpy(payload, payload_tmp, count * sizeof(T));
}


template <int size> INLINE void Sized_Swap(void * a, void * b) {
	char tmp[size];
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
}

template <> INLINE void Sized_Swap<1>(void * a, void * b) {
	Swap(*((char*)a), *((char*)b));
}

template <> INLINE void Sized_Swap<2>(void * a, void * b) {
	Swap(*((short*)a), *((short*)b));
}

template <> INLINE void Sized_Swap<4>(void * a, void * b) {
	Swap(*((int32_t*)a), *((int32_t*)b));
}

#if	HOOPS_MEMORY_ALIGNMENT == 3 // 64 bit system
template <> INLINE void Sized_Swap<8>(void * a, void * b) {
	Swap(*((intptr_t*)a), *((intptr_t*)b));
}
#endif


template<typename T>
struct sort_array_container {
	float * priorities;
	T * payload;
};


template<typename T>
class sort_array_container_ops {
public:
	INLINE static int compare(sort_array_container<T> * a, size_t i, size_t j) {
		return Compare(a->priorities[i],a->priorities[j]);
	}

	INLINE static void swap(sort_array_container<T> * a, size_t i, size_t j) {

		Swap(a->priorities[i],a->priorities[j]);

		if (a->payload != null)
			Sized_Swap<sizeof(T)>(&a->payload[i],&a->payload[j]);
	}
};


template <typename T> 
void sort_array(float * priorities, T * payload, size_t count, bool ascending=true)
{

	ASSERT(priorities != null);
	ASSERT(payload != null);

	if (count < 512) {

		sort_array_container<T>	sc;

		sc.priorities = priorities;
		sc.payload = payload;

		complex_sort<sort_array_container<T>, sort_array_container_ops<T> > (&sc, count, ascending);
	}
	else {
		float *		priorities_tmp;
		T *			payload_tmp;
		uint32_t *	b0; // 6144

		Auto_Buffers auto_locals(
			priorities_tmp, count,
			payload_tmp, count,
			b0, 6144);

		radix_sort_11(priorities, priorities_tmp, payload, payload_tmp, b0, count, ascending); //-V614
	}
	
}

