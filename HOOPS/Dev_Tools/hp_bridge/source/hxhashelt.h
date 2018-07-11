#ifndef __HXHASHELT_H_
#define __HXHASHELT_H_

class DLLEXPORT HXHashElt
{
public:
	typedef enum
	{
		KeyIsLess,
		KeyIsGreater,
		KeyEquals
	} CompareResult;
	HXHashElt(void* key, void* data);
	virtual ~HXHashElt();
	void* GetKey() { return m_Key; } // returns the key of the content
	void* GetData() { return m_Data; } // returns the key of the content
	HXHashElt* FindContent( void* key );
	HXHashElt* AddContent( void* key, void* data );
	HXHashElt* DeleteContent( void* key, void* data );
	HXHashElt* DeleteContent( void* key );
	HXHashElt* GetNext( void ) { return m_pNext; } // returns next content (for examining additional content )
	unsigned char GetLevel() {return m_Level;}
 
private:
	HXHashElt* m_pNext; // for additional content with the same key
	void* m_Key;
	void* m_Data;
	unsigned char m_Level;
};

#endif
