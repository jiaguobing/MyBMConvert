#pragma once 

#include <vector>
#include <algorithm>

namespace vector_utils {

	template <typename T, typename Alloc>
	INLINE bool contains(std::vector<T, Alloc> const & vector, T const & value) {

		typename std::vector<T,Alloc>::const_iterator const beg = vector.begin();
		typename std::vector<T, Alloc>::const_iterator const end = vector.end();

		return std::find(beg, end, value) != end;
	}

	template <typename T, typename Alloc>
	INLINE void remove(std::vector<T, Alloc> & vector, T const & value) {

		typename std::vector<T, Alloc>::iterator const it = std::find(vector.begin(), vector.end(), value);
		ASSERT(it != vector.end());
		vector.erase(it);
	}

	template <typename T, typename Alloc>
	INLINE void remove_at(std::vector<T, Alloc> & vector, uint32_t index) {
		ASSERT(index < vector.size());
		vector.erase(vector.begin() + index);		
	}

	template <typename T, typename Alloc>
	INLINE int index_of(std::vector<T, Alloc> const & vector, T const & item) {

		typename std::vector<T>::const_iterator const beg = vector.begin();
		typename std::vector<T>::const_iterator const end = vector.end();
		typename std::vector<T>::const_iterator const it = std::find(beg, end, item);

		return it != end ? it - beg : -1;
	}	

};
