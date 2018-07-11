// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef VARRAY_H
#define VARRAY_H

#include <stdlib.h>
#include <string.h>

#include <new>

//#include <type_traits>

#ifdef HOOPS_DEFINED
#error // this should not happen
#endif
 
#ifdef __cplusplus
extern "C++" {

template <typename T> class VArray
{
public:
	
	VArray(size_t size=1):count(0),allocated(0),data(0) {
		//static_assert(std::is_pod<T>::value || std::is_same<HPoint, T>::value, "POD only");
		EnsureSize(size, true);
	}

	VArray(VArray<T> &original):count(0),allocated(0),data(0) {
		//static_assert(std::is_pod<T>::value || std::is_same<HPoint, T>::value, "POD only");
		EnsureSize(original.Count(), true);
		count = original.Count();
		memcpy(data, original.data, count*sizeof(T));
	}

	VArray(VArray<T> *original):count(0),allocated(0),data(0) {
		//static_assert(std::is_pod<T>::value || std::is_same<HPoint, T>::value, "POD only");
		EnsureSize(original->Count(), true);
		count = original->Count();
		memcpy(data, original->data, count*sizeof(T));
	}
	
	virtual ~VArray() {
		TrimSize(0);
	}

	VArray<T> & operator= (VArray<T> const & other) {
		EnsureSize(other.Count(), true);
		count = other.Count();
		memcpy(data, other.data, count * sizeof(T));
		return *this;
	}
	
	inline size_t Count() const { return count; }
	inline size_t Size() const { return allocated; }

	/* If T is a char, you can use this class like so...
	 *    VCharArray buf(BUFFER_SIZE);
	 *    HC_Show_Rendering_Options(buf);
	 */
	operator T * () {
		return this->data;
	}

	T & operator [] (size_t i) {
		if(i >= count) {
			EnsureSize(i+1);
			count=i+1;
		}
		return data[i];
	}

	T const & operator [] (size_t i) const {
		if(i >= count) {
			EnsureSize(i+1);
			count=i+1;
		}
		return data[i];
	};

	T & operator [] (int i) {return operator []((size_t)i);}
	T const & operator [] (int i) const {return operator []((size_t)i);}

	T & Append() {
		EnsureSize(count+1);
		return data[count++];
	}

	T & Append(T const & item) {
		EnsureSize(count+1);
		data[count] = item;
		return data[count++];
	}

	void AppendArray(T const *item_array, size_t n_items) {
		if(n_items == 0) 
			return;
		EnsureSize(n_items+count);

		memcpy(&data[count], item_array, n_items*sizeof(T));
		count+=n_items;
	}

	T & GetData(size_t i) {
		return data[i];
	}

	T const & GetData(size_t i) const {
		return data[i];
	}

	void GetArrayCopy(T *new_item_array) {
	
		memcpy(new_item_array,data,count*sizeof(T));
	}


	T Pop() {
		return data[--count];
	}

	void InsertAt(T const & item, size_t i) {
		if(i >= count) {
			EnsureSize(i+1);
			count=i+1;
		}else{
			EnsureSize(count+1);
			memmove(&data[i+1], &data[i], (count-i)*sizeof(T));
			count++;
		}
		data[i] = item;
	}

	void InsertAt(size_t n_items, T const *item_array, size_t i) {
		if(n_items <= 0)
			return;
		if(i >= count) {
			EnsureSize(i+n_items);
			count=i+n_items;
		}else{
			EnsureSize(n_items+count);
			memmove(&data[i+1], &data[i], (count-i)*sizeof(T));
			count += n_items;
		}

		memcpy(&data[i], item_array, n_items*sizeof(T));
	}

	bool RemoveAt(size_t i, T * old_item=0) {
		if(i>=count) 
			return false;
		count--;
		if(old_item)
			*old_item = data[i];
	
		if(i!=count)
			memmove(&data[i], &data[i+1], (count-i)*sizeof(T)); 
		return true;
	}

	bool RemoveAt(size_t in_count, size_t i) {
		if(i>=count)
			return false;
		bool removing_to_end = false;
		if(i + in_count >= count) {
			in_count = count - i;
			removing_to_end = true;
		}
		count -= in_count;

		if(!removing_to_end)
			memmove(&data[i], &data[i+in_count], (count-i)*sizeof(T));
		return true;
	}

	bool RemoveAt(size_t i, T & old_item) {
		return RemoveAt(i,&old_item);
	}

	bool ReplaceAt(T const & item, size_t i, T * old_item=0) {
		if(i>=count){
			EnsureSize(i+1);
			count=i+1;
			data[i]=item;
			return false;
		}
		if(old_item) *old_item = data[i];
		data[i].~T();
		data[i]=item;
		return true;
	}

	bool ReplaceAt(size_t n_items, T const * item_array, size_t i) {
		size_t j;
		if(i+n_items>=count)
			EnsureSize(i+n_items);
		if(i>=count){
			count=i+n_items;
			for(j=0; j<n_items; ++j)
				data[i+j]=item_array[j];
			return false;
		}

		for(j=0; j<n_items && i+j<count; ++j) {
			data[i+j].~T();
			new(&data[i+j]) T();
			data[i+j]=item_array[j];
		}
		for(; j<n_items; ++j)
			data[i+j]=item_array[j];

		if(count<i+n_items) count=i+n_items;

		return true;
	}

	bool ReplaceAt(T const & item, size_t i, T & old_item) {
		return ReplaceAt(item,i,&old_item);
	}

	bool Swap(size_t i, size_t j) {
		if (i>=count||j>=count)
			return false;

		T temp = data[i];
		data[i] = data[j];
		data[j] = temp;

		return true;
	}

	void SetCount(size_t in_count) { 
		EnsureSize(in_count, true);
		count=in_count;
	}

	void Reverse() {
		T   temp;
		T * start;
		T * end;

		start = data;
		end = data + count - 1;
		while (start < end) {
			temp = *start;
			*start = *end;
			*end = temp;
			start++;
			end--;
		}
	}

	void MapFunction(void(*function)(T, void*), void * user_data) const{
		for(size_t i=0;i<count;i++)
			(*function)(data[i],user_data);
	}

	void MapFunction(void(*function)(T&, void*), void * user_data) {
		for(size_t i=0;i<count;i++)
			(*function)(data[i],user_data);
	}

	void MapFunction(void(*function)(T const &, void*), void * user_data) const{
		for(size_t i=0;i<count;i++)
			(*function)(data[i],user_data);
	}

	void TrimSize(size_t size = ~0) {
		if (size == ~(size_t)0)
			size = count;
		MakeSize(size);
	}

	void EnsureSize(size_t size, bool exact_size=false) const {
		if(size<=allocated) 
			return;
		size_t extra = 0;
		if (!exact_size)
			extra = (size>>2) + 4;
		MakeSize(size + extra);
	}
	
protected:

	void MakeSize(size_t size) const {

		allocated = size;
	
		T * new_data = 0;
		if (allocated > 0) {
			new_data = new T[allocated];
		}

		if (size < count)
			count = size;

		if (data != 0) {
			if(allocated > 0)
				memcpy(new_data, data, count*sizeof(T));
			delete[] data;
		}
		data=new_data;
	}

	mutable size_t count;   
	mutable size_t allocated;
	mutable T *data;
}; 

typedef VArray<char> VCharArray;
typedef VArray<int> VIntArray;
typedef VArray<float> VFloatArray;

} /* extern "C++" */
#endif

#endif 




