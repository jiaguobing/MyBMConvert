// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _UTF_UTILS
#define _UTF_UTILS


#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#ifdef _WIN32_WCE
#	define NEEDS_STR_CASE_CMP
#	define NEEDS_WCS_CASE_CMP
#endif // _WIN32_WCE
#ifdef OSX_SYSTEM
#	define NEEDS_WCS_CASE_CMP
#endif // OSX_SYSTEM
#ifdef HPUX_SYSTEM
#	define NEEDS_WCS_CASE_CMP
#endif // HPUX_SYSTEM
#if TARGET_OS_ANDROID
#	define NEEDS_WCS_CASE_CMP
#endif

#ifndef UTF_UTILS_API
#   if defined(LINUX_SYSTEM) || defined(__GNUC__)
#      define UTF_UTILS_API __attribute__ ((visibility ("default")))
#   else
#      define UTF_UTILS_API
#   endif
#endif

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <utility>
#include <wchar.h>

#ifdef V_PREFIX
#	define VNAME_TMP_2(_x2_,_y2_) _x2_##_y2_
#	define VNAME_TMP_1(_x1_,_y1_) VNAME_TMP_2(_x1_,_y1_)
#	define VNAME(_x_) VNAME_TMP_1(V_PREFIX,_x_)
	/* remap the names for special prefix */
#	define H_URI VNAME(H_URI)
#	define H_UTF8 VNAME(H_UTF8)
#	define H_UTF16 VNAME(H_UTF16)
#	define H_UTF32 VNAME(H_UTF32)
#	define H_WCS VNAME(H_WCS)
#	define H_FORMAT_TEXT VNAME(H_FORMAT_TEXT)
#	define HUtilityUnicodeStr VNAME(HUtilityUnicodeStr)
#	define HUtilityAsciiStr VNAME(HUtilityAsciiStr)
#	define KeyToCharPtr VNAME(KeyToCharPtr)
#	define DoubleToCharPtrInCLocale VNAME(DoubleToCharPtrInCLocale)
#	define URI_ENCODE VNAME(URI_ENCODE)
#	ifndef _MSC_VER
#		define wfopen VNAME(wfopen)
#		define wremove VNAME(wremove)
#		define wmkdir VNAME(wmkdir)
#		define wopendir VNAME(wopendir)
#	endif
#	ifdef NEEDS_STR_CASE_CMP
#		define strcasecmp VNAME(hstrcasecmp)
#	endif
#	ifdef NEEDS_WCS_CASE_CMP
#		define wcscasecmp VNAME(hwcscasecmp)
#	endif
#	define h_mbstowcs VNAME(h_mbstowcs)
#	define h_wcstombs VNAME(h_wcstombs)
#else
#	ifdef NEEDS_STR_CASE_CMP
#		define strcasecmp hstrcasecmp
		int strcasecmp (char const *string1, char const *string2);
#	endif
#	ifdef NEEDS_WCS_CASE_CMP
#		define wcscasecmp hwcscasecmp
		int wcscasecmp (wchar_t const *string1, wchar_t const *string2);
#	endif
#endif

#ifndef HOOPS_DEFINED
#	include <cstdlib>
#	include <stdlib.h>
#	define ALLOC_ARRAY(p, count, type)	((p)=(type*)malloc(sizeof(type)*(count)))
#	define FREE_ARRAY(p, count, type)	(free((void*)(p)))
#endif


#ifndef _MSC_VER
#	define __wchar_t wchar_t
#	include <dirent.h>
#	include <sys/types.h>
#else
#	ifndef H_DISABLE_USHORT_OVERLOAD
#		define H_USHORT_OVERLOAD
#	endif
#endif
		
typedef unsigned char utf8_char;

class UTF_UTILS_API H_UTF32;
class UTF_UTILS_API H_UTF16;
class UTF_UTILS_API H_UTF8;
class UTF_UTILS_API H_URI;

#ifdef _MSC_VER
	typedef H_UTF16 H_WCS;
	typedef unsigned int utf32_char;
	typedef wchar_t utf16_char;
#	define wfopen _wfopen
#	define wremove _wremove
#	define wmkdir _wmkdir
#else
	typedef H_UTF32 H_WCS;
	typedef wchar_t utf32_char;
	typedef unsigned short utf16_char;
	UTF_UTILS_API FILE * wfopen (wchar_t const *, wchar_t const *);
	int wremove (wchar_t const *);
	int wmkdir (wchar_t const *, int);
	DIR * wopendir(wchar_t const *);
#endif

size_t h_mbstowcs(wchar_t *, char const *, size_t);
size_t h_wcstombs(char *, wchar_t const *, size_t);







template <typename Derived, typename Char>
class UTF_UTILS_API HUTF_Base
#ifdef HOOPS_DEFINED
	: public CMO
#endif
{
public:
	typedef Char utf_char;

protected:
	typedef HUTF_Base utf_base_class;
	typedef wchar_t default_coerce_char;


	template <typename T>
	class UTF_UTILS_API Buffer {
	public:
		Buffer ()
			: buffer(nullptr)
			, buffer_size(0)
		{}
		explicit Buffer (size_t size)
			: buffer_size(size)
		{
			ALLOC_ARRAY(buffer, buffer_size, T);
		}
		~Buffer () {
			if (buffer != nullptr) {
				FREE_ARRAY(buffer, buffer_size, T);
			}
		}
		void operator= (Buffer && other) {
			assert(this != &other);

			this->~Buffer();

			buffer = std::move(other.buffer);
			buffer_size = std::move(other.buffer_size);

			other.buffer = nullptr;
		}
		void reserve (size_t size) {
			if (size > buffer_size) {
				T * new_buffer;
				ALLOC_ARRAY(new_buffer, size, T);
				if (buffer) {
					memcpy(new_buffer, buffer, buffer_size * sizeof(T));
					FREE_ARRAY(buffer, buffer_size, T);
				}
				buffer = new_buffer;
				buffer_size = size;
			}
		}
		size_t capacity () const {
			return buffer_size;
		}
		T * data () {
			return capacity() ? buffer : nullptr;
		}
		T const * data () const {
			return capacity() ? buffer : nullptr;
		}
		T & operator[] (size_t idx) {
			assert(idx < capacity());
			return buffer[idx];
		}
		T const & operator[] (size_t idx) const {
			assert(idx < capacity());
			return buffer[idx];
		}
	private:
		T * buffer;
		size_t buffer_size;
	};


	//////////////////////////////////////////////////////


	HUTF_Base ()
		: used_capacity(0)
	{}

	template <typename Coerced_Char, typename Source_Char>
	void construct_from_utf_cstr (Source_Char const * text);

	void construct_from_cstr (char const * text);

	template <typename Source_UTF>
	void construct_from_utf (Source_UTF const & unicode_text);

	void construct_copy (Derived const & other);

	Derived & operator= (Derived && other);

	Derived & operator= (Derived const & other);


	//////////////////////////////////////////////////////


	void format (__wchar_t const * fmt, va_list (&args)[2]);
#ifdef _MSC_VER
	void format (unsigned short const * fmt, va_list (&args)[2]);
#endif
	void format (char const * fmt, va_list (&args)[2]);

	size_t actual_length () const;

	size_t logical_length () const;

	Derived & append (Derived const & utf);

	Derived & append (__wchar_t const * text);
#ifdef _MSC_VER
	Derived & append (unsigned short const * text);
#endif
	Derived & append (char const * text);

	Derived & insert (int loc, Derived const & utf);

	Derived & insert (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	Derived & insert (int loc, unsigned short const * text);
#endif
	Derived & insert (int loc, char const * text);

	Derived & remove (int loc, int count);

	Derived & flush ();

	Derived & trunc (int loc);

	Derived & replace (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	Derived & replace (int loc, unsigned short const * text);
#endif
	Derived & replace (int loc, char const * text);

	int find (__wchar_t const * text) const;
#ifdef _MSC_VER
	int find (unsigned short const * text) const;
#endif
	int find (char const * text) const;

	bool equals (Derived const & other) const;

	Derived const & encodedText (void const * text); // Why o' why does this take a void pointer?

	void reserve (size_t minimum_encoded_size);

	void update_offset_mappings (size_t start_actual_idx);

public:

	bool operator!= (Derived const & other) const {
		return !(*static_cast<Derived const *>(this) == other);
	}

protected:
	Buffer<utf_char> encoded_text;
	Buffer<size_t> actual_to_logical;
	Buffer<size_t> logical_to_actual;
	size_t used_capacity;
};








class UTF_UTILS_API H_UTF32
	: public HUTF_Base<H_UTF32, utf32_char>
{
	friend class HUTF_Base<H_UTF32, utf32_char>;

public:
	H_UTF32 ();
	H_UTF32 (__wchar_t const * text);
#ifdef _MSC_VER
	H_UTF32 (unsigned short const * text);
#endif
	H_UTF32 (char const * text);
	H_UTF32 (H_UTF32 const & other);
	H_UTF32 (H_UTF16 const & unicode_text);
	H_UTF32 (H_UTF8 const & unicode_text);
	H_UTF32 (H_URI const & unicode_text);

	~H_UTF32 ();

	H_UTF32 & operator= (H_UTF32 && other);
	H_UTF32 & operator= (H_UTF32 const & other);

	size_t logical_length () const;

	size_t encoded_length () const;

	size_t length () const {
		return encoded_length();
	}

	utf32_char const * encodedText () const {
		return encoded_text.capacity() ? &encoded_text[0] : nullptr;
	}

	H_UTF32 const & encodedText (void const * text);

	void reserve (size_t minimum_encoded_size);

	H_UTF32 & format (__wchar_t const * fmt, ...);
#ifdef _MSC_VER
	H_UTF32 & format (unsigned short const * fmt, ...);
#endif
	H_UTF32 & format (char const * fmt, ...);

	H_UTF32 & append (__wchar_t const * text);
#ifdef _MSC_VER
	H_UTF32 & append (unsigned short const * text);
#endif
	H_UTF32 & append (char const * text);

	H_UTF32 & insert (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_UTF32 & insert (int loc, unsigned short const * text);
#endif
	H_UTF32 & insert (int loc, char const * text);

	H_UTF32 & remove (int loc, int count);

	H_UTF32 & flush ();

	H_UTF32 & trunc (int loc);

	H_UTF32 & replace (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_UTF32 & replace (int loc, unsigned short const * text);
#endif
	H_UTF32 & replace (int loc, char const * text);

	int find (__wchar_t const * text) const;
#ifdef _MSC_VER
	int find (unsigned short const * text) const;
#endif
	int find (char const * text) const;

	class iterator {
		friend class HUTF_Base<H_UTF32, utf32_char>;
	private:
		utf32_char const * pos;
		static int get_and_advance (utf32_char const *& text);
	public:
		iterator (utf32_char const * pos = nullptr);
		iterator operator++ ();
		iterator operator++ (int);
		int operator* () const;
		operator bool () const;
		bool operator== (iterator const & other) const {
			return pos == other.pos;
		}
		bool operator!= (iterator const & other) const {
			return !(*this == other);
		}
	};

	iterator begin () const;
	iterator end () const;

	bool operator== (H_UTF32 const & other) const;
};








class UTF_UTILS_API H_UTF16
	: public HUTF_Base<H_UTF16, utf16_char>
{
	friend class HUTF_Base<H_UTF16, utf16_char>;

public:
	H_UTF16 ();
	H_UTF16 (__wchar_t const * text);
#ifdef _MSC_VER
	H_UTF16 (unsigned short const * text);
#endif
	H_UTF16 (char const * text);
	H_UTF16 (H_UTF32 const & unicode_text);
	H_UTF16 (H_UTF16 const & other);
	H_UTF16 (H_UTF8 const & unicode_text);
	H_UTF16 (H_URI const & unicode_text);

	~H_UTF16 ();

	H_UTF16 & operator= (H_UTF16 && other);
	H_UTF16 & operator= (H_UTF16 const & other);

	size_t logical_length () const;

	size_t encoded_length () const;

	size_t length () const {
		return encoded_length();
	}

	utf16_char const * encodedText () const {
		return encoded_text.capacity() ? &encoded_text[0] : nullptr;
	}

	H_UTF16 const & encodedText (void const * text);

	void reserve (size_t minimum_encoded_size);

	H_UTF16 & format (__wchar_t const * fmt, ...);
#ifdef _MSC_VER
	H_UTF16 & format (unsigned short const * fmt, ...);
#endif
	H_UTF16 & format (char const * fmt, ...);

	H_UTF16 & append (__wchar_t const * text);
#ifdef _MSC_VER
	H_UTF16 & append (unsigned short const * text);
#endif
	H_UTF16 & append (char const * text);

	H_UTF16 & insert (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_UTF16 & insert (int loc, unsigned short const * text);
#endif
	H_UTF16 & insert (int loc, char const * text);

	H_UTF16 & remove (int loc, int count);

	H_UTF16 & flush ();

	H_UTF16 & trunc (int loc);

	H_UTF16 & replace (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_UTF16 & replace (int loc, unsigned short const * text);
#endif
	H_UTF16 & replace (int loc, char const * text);

	int find (__wchar_t const * text) const;
#ifdef _MSC_VER
	int find (unsigned short const * text) const;
#endif
	int find (char const * text) const;

	class iterator {
		friend class HUTF_Base<H_UTF16, utf16_char>;
	private:
		utf16_char const * pos;
		static int get_and_advance (utf16_char const *& text);
	public:
		iterator (utf16_char const * pos = nullptr);
		iterator operator++ ();
		iterator operator++ (int);
		int operator* () const;
		operator bool () const;
		bool operator== (iterator const & other) const {
			return pos == other.pos;
		}
		bool operator!= (iterator const & other) const {
			return !(*this == other);
		}
	};

	iterator begin () const;
	iterator end () const;

	bool operator== (H_UTF16 const & other) const;
};










class UTF_UTILS_API H_UTF8
	: public HUTF_Base<H_UTF8, utf8_char>
{
	friend class HUTF_Base<H_UTF8, utf8_char>;

public:
	H_UTF8 ();
	H_UTF8 (__wchar_t const * text);
#ifdef _MSC_VER
	H_UTF8 (unsigned short const * text);
#endif
	H_UTF8 (char const * text);
	H_UTF8 (H_UTF32 const & unicode_text);
	H_UTF8 (H_UTF16 const & unicode_text);
	H_UTF8 (H_UTF8 const & other);
	H_UTF8 (H_URI const & unicode_text);

	~H_UTF8 ();

	H_UTF8 & operator= (H_UTF8 && other);
	H_UTF8 & operator= (H_UTF8 const & other);

	size_t logical_length () const;

	size_t encoded_length () const;

	size_t length () const {
		return encoded_length();
	}

	utf8_char const * encodedText () const {
		return encoded_text.capacity() ? &encoded_text[0] : nullptr;
	}

	H_UTF8 const & encodedText (void const * text);

	void reserve (size_t minimum_encoded_size);

	H_UTF8 & format (__wchar_t const * fmt, ...);
#ifdef _MSC_VER
	H_UTF8 & format (unsigned short const * fmt, ...);
#endif
	H_UTF8 & format (char const * fmt, ...);

	H_UTF8 & append (__wchar_t const * text);
#ifdef _MSC_VER
	H_UTF8 & append (unsigned short const * text);
#endif
	H_UTF8 & append (char const * text);

	H_UTF8 & insert (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_UTF8 & insert (int loc, unsigned short const * text);
#endif
	H_UTF8 & insert (int loc, char const * text);

	H_UTF8 & remove (int loc, int count);

	H_UTF8 & flush ();

	H_UTF8 & trunc (int loc);

	H_UTF8 & replace (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_UTF8 & replace (int loc, unsigned short const * text);
#endif
	H_UTF8 & replace (int loc, char const * text);

	int find (__wchar_t const * text) const;
#ifdef _MSC_VER
	int find (unsigned short const * text) const;
#endif
	int find (char const * text) const;

	class iterator {
		friend class HUTF_Base<H_UTF8, utf8_char>;
	private:
		utf8_char const * pos;
		static int get_and_advance (utf8_char const *& text);
	public:
		iterator (utf8_char const * pos = nullptr);
		iterator operator++ ();
		iterator operator++ (int);
		int operator* () const;
		operator bool () const;
		bool operator== (iterator const & other) const {
			return pos == other.pos;
		}
		bool operator!= (iterator const & other) const {
			return !(*this == other);
		}
	};

	iterator begin () const;
	iterator end () const;

	bool operator== (H_UTF8 const & other) const;
};










class UTF_UTILS_API H_URI_Char {
public:
	typedef utf8_char char_type;
	H_URI_Char () {
		typedef char assertion[sizeof(H_URI_Char[2]) == sizeof(char_type[2]) ? 1 : -1];
		(void) (assertion *) nullptr;
	}
	H_URI_Char (char_type ch)
		: c(ch)
	{
		typedef char assertion[sizeof(H_URI_Char[2]) == sizeof(char_type[2]) ? 1 : -1];
		(void) (assertion *) nullptr;
	}
	operator char_type & () {
		return c;
	}
	operator char_type const & () const {
		return c;
	}
private:
	char_type c;
};

/* H_URI encodes a utf8 encoded string. It replaces all non-ascii chars with percent encodings.
 * so the string "AA" could be URI encoded as "%65%65" (but as the chars are ascii, it will be
 * encoded as "AA". */
class UTF_UTILS_API H_URI
	: public HUTF_Base<H_URI, H_URI_Char>
{
	friend class HUTF_Base<H_URI, H_URI_Char>;

public:
	static bool unreserved_char (int c);

public:
	H_URI ();
	H_URI (__wchar_t const * text);
#ifdef _MSC_VER
	H_URI (unsigned short const * text);
#endif
	H_URI (char const * text);
	H_URI (H_UTF32 const & unicode_text);
	H_URI (H_UTF16 const & unicode_text);
	H_URI (H_UTF8 const & unicode_text);
	H_URI (H_URI const & other);

	~H_URI ();

	H_URI & operator= (H_URI && other);
	H_URI & operator= (H_URI const & other);

	size_t logical_length () const;

	size_t encoded_length () const;

	size_t length () const {
		return encoded_length();
	}

	utf8_char const * encodedText () const {
		return reinterpret_cast<H_URI_Char::char_type const *>(
			encoded_text.capacity() ? &encoded_text[0] : nullptr
		);
	}

	H_URI const & encodedText (void const * text);

	void reserve (size_t minimum_encoded_size);

	H_URI & format (__wchar_t const * fmt, ...);
#ifdef _MSC_VER
	H_URI & format (unsigned short const * fmt, ...);
#endif
	H_URI & format (char const * fmt, ...);

	H_URI & append (__wchar_t const * text);
#ifdef _MSC_VER
	H_URI & append (unsigned short const * text);
#endif
	H_URI & append (char const * text);

	H_URI & insert (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_URI & insert (int loc, unsigned short const * text);
#endif
	H_URI & insert (int loc, char const * text);

	H_URI & remove (int loc, int count);

	H_URI & flush ();

	H_URI & trunc (int loc);

	H_URI & replace (int loc, __wchar_t const * text);
#ifdef _MSC_VER
	H_URI & replace (int loc, unsigned short const * text);
#endif
	H_URI & replace (int loc, char const * text);

	int find (__wchar_t const * text) const;
#ifdef _MSC_VER
	int find (unsigned short const * text) const;
#endif
	int find (char const * text) const;

	class iterator {
		friend class HUTF_Base<H_URI, H_URI_Char>;
	private:
		H_URI_Char const * pos;
		static int get_and_advance (H_URI_Char const *& text);
	public:
		iterator (H_URI_Char const * pos = nullptr);
		iterator operator++ ();
		iterator operator++ (int);
		int operator* () const;
		operator bool () const;
		bool operator== (iterator const & other) const {
			return pos == other.pos;
		}
		bool operator!= (iterator const & other) const {
			return !(*this == other);
		}
	};

	iterator begin () const;
	iterator end () const;

	bool operator== (H_URI const & other) const;
};









/*! H_FORMAT_TEXT is used to format text printf style in HOOPS functions that take char* args.
	One example of this is: Set_Rendering_Options(H_FORMAT_TEXT("size = (%f, %f)", width, height")).
*/
class UTF_UTILS_API H_FORMAT_TEXT
#ifdef HOOPS_DEFINED
	: public CMO
#endif
{
public:

	H_FORMAT_TEXT ();

	/*! Use this constructor as a cast for format a string. Do *NOT* set this equal to a
		variable. [char * s = H_FORMAT_TEXT("")] is a bug and [s] will point to freed memory.
		The only acceptable uses of this class is casting in a function parameter or initializing
		a static or automatic H_FORMAT_TEXT variable.
		An example of using this as a function argument:
		HC_Set_Driver_Options(H_FORMAT_TEXT("%d, %f, %s", (int)i, (float)j, "text"));
		An example of creating an automatic variable:
		H_FORMAT_TEXT color("faces = %s, edges = %s", face_color, edge_color);
		HC_Set_Color(color);
	 */
	H_FORMAT_TEXT (char const * const format, ...);

	H_FORMAT_TEXT (H_FORMAT_TEXT const & other);

	~H_FORMAT_TEXT ();

	H_FORMAT_TEXT const & operator= (H_FORMAT_TEXT const & other);

	H_FORMAT_TEXT & Append (char const * const format, ...);

	char const * c_str () const;

	/*! This returns the memory when the class is in scope. */
	operator char const * () const;

private:
	char * str;
	char default_buffer[32];
	size_t allocated;
	size_t used;
};

#define HFT(...) ((char const *)H_FORMAT_TEXT(__VA_ARGS__))







/*! KeyToCharPtr is used to format HC_KEY as a string for better portability (64-bit Windows treats %lx
	differently than all the other 64-bit systems). Do not call KeyToCharPtr directly, instead use the
	casting macro ktoa. The return value will ALWAYS be lower case, will ALWAYS be hex and will NEVER
	be prepending with 0x.
	Example: HC_Insert_Text(0, 0, 0, H_FORMAT_TEXT("%s", ktoa(m_pHView->GetModelKey()));
*/
class UTF_UTILS_API KeyToCharPtr
#ifdef HOOPS_DEFINED
	: public CMO
#endif
{
private:
	char * format_key () const;

public:
	KeyToCharPtr (void * k, bool trim_0x);
	operator char const * () const;
	operator char * () const;

private:
	void * key;
	mutable char * p;
	mutable char str[24];
	bool trim;
};

#define ktoa(x) ((char const *)(KeyToCharPtr((void *)(x), true)))
#define kto0xa(x) ((char const *)(KeyToCharPtr((void *)(x), false)))







class UTF_UTILS_API DoubleToCharPtrInCLocale {
private:
	char * format_double () const;

public:
	DoubleToCharPtrInCLocale (double f);
	operator char const * () const;
	operator char * () const;

private:
	double value;
	mutable char str[317]; // long enough to handle 1.797693e+308 (which is DBL_MAX on some machines)
};

#define hftoa(f) ((char const *)(DoubleToCharPtrInCLocale(f)))







// HUtilityUnicodeStr ////////////////////////////////////////////////////////
// Converts both 8-bit (foreign) and 16-bit (native) strings to 16-bit wideness

/*!
	HUtilityUnicodeStr takes a regular (8bit - char*) or UNICODE (16 bit - wchar*) string and converts
	to a UNICODE string. This UNICODE string can be used for later operations. The converted UNICODE
	string can only be used as long as the HUtilityUnicodeStr object exists.
*/
class UTF_UTILS_API HUtilityUnicodeStr
#ifdef HOOPS_DEFINED
	: public CMO
#endif
{
public:

	/*! Constructor
		\param p8    char* pointing to an 8-bit string */
	HUtilityUnicodeStr (char const * p8);

	/*! Constructor
		\param p16   wchar_t* pointing to an 16-bit string */
	HUtilityUnicodeStr (__wchar_t const * p16);

#ifdef _MSC_VER
	HUtilityUnicodeStr (unsigned short const * p16);
#endif

	/*! destructor - A Non-virtual destructor (this class is concrete) */
	~HUtilityUnicodeStr ();

	/*! conversion operator to UNICODE */
	operator __wchar_t const * () const;

#ifdef _MSC_VER
	operator unsigned short const * () const;
#endif

private:
	// protect against assignment
	HUtilityUnicodeStr (HUtilityUnicodeStr const & str2);

	// protect against assignment
	HUtilityUnicodeStr & operator= (HUtilityUnicodeStr const & str2);

private:
	H_WCS wcs;
};









/*!
	HUtilityAsciiStr takes a regular (8bit - char*) or UNICODE (16 bit - wchar*) string and converts
	to a regular 8-bit string. This string can be used for later operations. The converted
	string can only be used as long as the HUtilityAsciiStr object exists.
*/
class UTF_UTILS_API HUtilityAsciiStr
#ifdef HOOPS_DEFINED
	: public CMO
#endif
{
public:
	/*! Constructor
		\param p8    char* pointing to an 8-bit string */
	HUtilityAsciiStr (char const * p8);

	/*! Constructor
		\param p16   wchar_t* pointing to an 16-bit string */
	HUtilityAsciiStr (__wchar_t const * p16);

#ifdef _MSC_VER
	HUtilityAsciiStr (unsigned short const * p16);
#endif

	/*! destructor - A Non-virtual destructor (this class is concrete) */
	~HUtilityAsciiStr ();

	/*! conversion operator to ASCII */
	operator char const * () const;

private:
	// protect against assignment!
	HUtilityAsciiStr (const HUtilityAsciiStr &);
	HUtilityAsciiStr & operator= (const HUtilityAsciiStr &);

private:
	char * str;		/*!< The 8-bit string */
	size_t size;
};






#define URI_ENCODE_BUFFER_SIZE 256

class UTF_UTILS_API URI_ENCODE
#ifdef HOOPS_DEFINED
	: public CMO
#endif
{
private:
	static bool unreserved_char (int const c);
	static size_t decode_utf16 (wchar_t const * text, size_t output_len, int * unicode);
	static size_t encode_uri (int const * unicode, size_t output_len, char * uri);
	static size_t uri_encode (wchar_t const * prefix, wchar_t const * input, size_t output_len, char * output);
public:
	URI_ENCODE (wchar_t const * input, wchar_t const * prefix);
	~URI_ENCODE ();
	operator char const * () const;
private:
	char * encoded_text;
	char default_buffer[URI_ENCODE_BUFFER_SIZE];
};






/*! \def H_TEXT - use this macro where-ever the api itself takes different type of argument
		during compilation depending upon preprocessor
 */
#ifdef _UNICODE
typedef HUtilityUnicodeStr H_TEXT;
#else
typedef HUtilityAsciiStr H_TEXT;
#endif

/*! \def H_UNICODE_TEXT Converts any string (UNICODE or ASCII) to UNICODE string */
#define H_UNICODE_TEXT(_x_) ((wchar_t const *)HUtilityUnicodeStr(_x_))

/*! \def H_ASCII_TEXT Converts any string (UNICODE or ASCII) to ASCII string */
#define H_ASCII_TEXT(_x_) ((char const *)HUtilityAsciiStr(_x_))


#ifdef H_PACK_8
#pragma pack(pop)
#endif




#endif

