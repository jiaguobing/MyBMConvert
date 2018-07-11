// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef VHEAP_H
#define VHEAP_H

#include "varray.h"

#ifdef HOOPS_DEFINED
#error // this should not happen
#endif
 
#ifdef __cplusplus
extern "C++" {

#define VHEAP_COMPILE_TIME_ASSERT(condition) \
	do { class message { char array[condition ? 1 : -1]; }; } while(0) 

#define VHEAP_PARENT(_i_) (((_i_)-1)/2)
#define VHEAP_LEFT(i) (2*(i)+1)
#define VHEAP_RIGHT(i) (2*(i)+2)


template <typename T1, typename T2, bool largest_out_first> class VHeap 
{

private:

	class Element 
	{
		public:
			Element(T1 item, T2 key) {_item=item;_key=key;_i=~((size_t)0);};
			T1 _item;
			T2 _key;
			size_t _i;
	};

typedef VArray<Element*> ElementsData;
typedef VHash<T1,Element*> UpdateHash;


	ElementsData *	elements;

	void down(size_t i) {

		size_t	c = elements->Count();
		size_t	l = VHEAP_LEFT(i);
		size_t	r = VHEAP_RIGHT(i);

		while (l < c) {

			size_t m;

			if (r >= c)	/* careful: right leaf may not exist */
				m = l;
			else {
				if (largest_out_first) {
					if (elements->GetData(l)->_key > elements->GetData(r)->_key)
						m = l;
					else
						m = r;
				} else {
					if (elements->GetData(l)->_key < elements->GetData(r)->_key)
						m = l;
					else
						m = r;
				}
			}

			if (largest_out_first) {
				if (elements->GetData(m)->_key > elements->GetData(i)->_key) {
					elements->Swap(i, m);
					i = m;
					l = VHEAP_LEFT(i);
					r = VHEAP_RIGHT(i);
				}
				else
					break;
			}
			else {
				if (elements->GetData(m)->_key < elements->GetData(i)->_key) {
					elements->Swap(i, m);
					i = m;
					l = VHEAP_LEFT(i);
					r = VHEAP_RIGHT(i);
				}
				else
					break;
			}
		}
	}

	void up(size_t i) {

		size_t p = VHEAP_PARENT(i);

		if (largest_out_first) {
			while (i > 0 && elements->GetData(i)->_key > elements->GetData(p)->_key) {
				elements->Swap(i, p);
				i = p;
				p = VHEAP_PARENT(i);
			}
		}
		else {
			while (i > 0 && elements->GetData(i)->_key < elements->GetData(p)->_key) {
				elements->Swap(i, p);
				i = p;
				p = VHEAP_PARENT(i);
			}
		}
	}

public:


	VHeap(size_t initial_size=16) : update_hash(0) {
		elements = new ElementsData(initial_size);
		//VHEAP_COMPILE_TIME_ASSERT(sizeof(T1) <= sizeof(void *));	
		//VHEAP_COMPILE_TIME_ASSERT(sizeof(T2) <= sizeof(void *));
	};	
	
	virtual ~VHeap() {

		while(elements->Count()>0)
			delete elements->Pop();

		delete elements;
	}; 

	size_t Count() const {
		return elements->Count();
	};


	Element * GetElement(T1 item, T2 key) {
		return new Element(item, key);
	}

	void Push(T1 item, T2 key) {

		size_t i = elements->Count();

		Element * element = GetElement(item, key);
		elements->Append(element);

		up(i);
	};

	bool Peek(T1 * item, T2 * key=0) {

		size_t c = elements->Count();

		if (c == 0)
			return false;

		Element * element = elements->GetData(0);

		*item = element->_item;
		if (key != null)
			*key = element->_key;

		return true;
	}

	bool PeekKey(T2 * key) {

		size_t c = elements->Count();

		if (c == 0)
			return false;

		Element * element = elements->GetData(0);

		*key = element->_key;

		return true;
	}


	bool Pop(T1 * item=0, T2 * key=0) {

		size_t c = elements->Count();

		if (c == 0)
			return false;

		Element * element = elements->GetData(0);

		if (item)
			*item = element->_item;
		if (key != null)
			*key = element->_key;

		
		if (c > 1) {

			c--;

			elements->Swap(0, c);
			elements->Pop(); 

			down(0);	
		}
		else
			elements->Pop(); 

		delete element;

		return true;
	};

};

} /* extern "C++" */
#endif

#endif 



