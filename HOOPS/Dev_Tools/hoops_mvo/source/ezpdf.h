// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#if 1

#ifndef _EZPDF_H
#define _EZPDF_H

#include <locale>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <assert.h>
#include "zlib.h"
#include "utf_utils.h"

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

namespace ezpdf
{
	using namespace std;

	int const EZPDF_BUFFER_SIZE = 1024*64;

	class File;
	class Dict;
	class Array;

	namespace common {
		bool LittleEndian(){
			unsigned short lebom = 0xfffe;
			unsigned char const * const first_byte = (unsigned char const * const)&lebom;
			return *first_byte == 0xfe;
		}

		void escape_char(ostream & out, char const c)
		{
			if(c == '\\' || c == '(' || c == ')')
				out << '\\';
		}
	}

	using namespace common;

	void write_string(ostream & out, wstring const & s)
	{
		H_UTF16 utf16(s.c_str());
		/* Write out big endian UTF16 BOM. */
		out << '(' << (char)-2 << (char)-1;
		utf16_char const * next = utf16.encodedText();
		while(*next != 0){
			char const upper = (char)((*next >> 8) & 0xff);
			escape_char(out, upper);
			out << upper;

			char const lower = (char)(*next & 0xff);
			escape_char(out, lower);
			out << lower;

			++next;
		}
		out << ')';
	}

	void write_string(ostream & out, string const & s)
	{
		out << '(';
		char const * next = s.c_str();
		while(*next != 0){
			escape_char(out, *next);
			out << *next;
			++next;
		}
		out << ')';
	}



	class BaseObject
	{
	public:
		BaseObject ()
		{
		};

		virtual ~ BaseObject ()
		{
		};

		friend class File;
		friend class Dict;
		friend class Array;

	private:
		virtual void release ()
		{
		};

		virtual bool managed () const
		{
			return false;
		};

		virtual void write (ostream & out) const = 0;
		virtual BaseObject *clone () const = 0;
	};

	class Boolean:public BaseObject
	{
	private:
		bool m_bool;

		void write (ostream & out) const
		{
			out << (m_bool ? "true" : "false");
		};

		void release ()
		{
			delete this;
		};

		BaseObject *clone () const
		{
			return static_cast < BaseObject * >(new Boolean (m_bool));
		};

	public:
		friend class Dict;

		Boolean (bool b):m_bool (b)
		{
		};

		operator bool () const
		{
			return m_bool;
		};
	};

	class Int:public BaseObject
	{
	private:
		long m_int;

		void write (ostream & out) const
		{
			out << m_int;
		};

		void release ()
		{
			delete this;
		};

		BaseObject *clone () const
		{
			return static_cast < BaseObject * >(new Int (m_int));
		};

	public:
		friend class Dict;

		Int (long i):m_int (i)
		{
		};

		operator int () const
		{
			return static_cast<int>(m_int);
		};

		operator long () const
		{
			return m_int;
		};

		operator size_t () const
		{
			return static_cast<size_t>(m_int);
		};
	};

	class Float:public BaseObject
	{
	private:
		double m_float;

		void write (ostream & out) const
		{
			out << fixed << setprecision(6) << m_float;
		};

		void release ()
		{
			delete this;
		};

		BaseObject *clone () const
		{
			return static_cast < BaseObject * >(new Float (m_float));
		};

	public:
		friend class Dict;

		Float (double f):m_float (f)
		{
		};

		operator float () const
		{
			return static_cast<float>(m_float);
		};

		operator double () const
		{
			return m_float;
		};

	};

	class Name:public BaseObject
	{
	private:
		string m_name;

		void write (ostream & out) const
		{
			out << '/' << m_name;
		};

		void release ()
		{
			delete this;
		};

		BaseObject *clone () const
		{
			return static_cast < BaseObject * >(new Name (m_name));
		};

	public:
		friend class Dict;
		friend class Graphics;

		Name (string const &s):m_name (s)
		{
		};

		Name (char const * s):m_name (s)
		{
		};

		bool operator < (Name const &rhs) const
		{
			return m_name < rhs.m_name;
		};

		bool operator == (Name const &rhs) const
		{
			return m_name == rhs.m_name;
		};

		bool operator != (Name const &rhs) const
		{
			return m_name != rhs.m_name;
		};

		operator string () const
		{
			return m_name;
		};

	};

	class WString : public BaseObject
	{
	private:
		wstring m_string;

		void write (ostream & out) const
		{
			write_string(out, m_string);
		};

		void release ()
		{
			delete this;
		};

		BaseObject *clone () const
		{
			return static_cast < BaseObject * >(new WString (m_string));
		};
	public:
		friend class Dict;

		explicit WString (string const &s):m_string (H_WCS(s.c_str()).encodedText())
		{
		};

		explicit WString (wstring const &s):m_string(s)
		{
		};

		operator wstring () const
		{
			return m_string;
		};

	};

	class String : public BaseObject
	{
	private:
		string m_string;

		void write (ostream & out) const
		{
			write_string(out, m_string);
		};

		void release ()
		{
			delete this;
		};

		BaseObject *clone () const
		{
			return static_cast < BaseObject * >(new String (m_string));
		};
	public:
		friend class Dict;

		explicit String (string const &s):m_string (s)
		{
		};

		operator string () const
		{
			return m_string;
		};

	};



	class Graphics
	{
	private:
		stringstream m_stream;

		void Font(Name const & font_name, double const points) {
			font_name.write(m_stream);
			m_stream << ' ' << points << " Tf" << endl;
		};

	public:

		/*****************************
		* PDF Drawing commands
		****************************

		* B - Fill and stroke the path using nonzero winding number rule
		* B* - Fill and stroke the path using even-odd rule
		* b - Close, fill and stroke the path using nonzero winding number rule
		* b* - Close, fill and stroke the path using even-odd rule
		* c - curve
		* cs - color space
		* d - dash pattern
		* f - fill path using nonzero winding number rule
		* f* - fill path using even-odd rule
		* h - connect last point to first point in path (complete path)
		* J - Line Cap
		* j - Line Join
		* l - line to (from last point in path)
		* m - move to (begin new path)
		* n - end the path without filling or stroking
		* Q - pop color space/clipping state on color space stack
		* q - push color space/clipping state on color space stack
		* re - rectangle
		* RG - set stroke color
		* rg _ set fill color
		* S - Stroke path
		* s - Close and stroke a path (same is "h S")
		* scn - something to do with face patterns
		* Tf - font size (in points)
		* W - marks the current path as the clipping path and intersects it with the current clipping path
		* w - line weight

		*****************************
		* PDF Drawing commands
		****************************/

		Graphics() {};

		Graphics(Graphics const & g)
		{
			m_stream << g.m_stream.str();
		};

		Graphics const operator = (Graphics const & rhs)
		{
			m_stream << rhs.m_stream.str();
			return *this;
		};

		friend class Dict;

		Graphics & Exlicit(string const & cmd)
		{
			m_stream.write(cmd.c_str(), static_cast<streamsize>(cmd.length()));
			m_stream << endl;
			return *this;
		};

		Graphics & PushGraphicsState() {m_stream << 'q' << endl;return *this;};
		Graphics & PopGraphicsState() {m_stream << 'Q' << endl;return *this;};

		Graphics & PenPosition(double const x, double const y)
		{
			m_stream << x << ' ' << y << " m" << endl;return *this;
		};

		Graphics & MovePen(double const x, double const y)
		{
			m_stream << x << ' ' << y << " l" << endl;return *this;
		};

		Graphics & StrokeColor(double const r, double const g, double const b)
		{
			m_stream << r << ' ' << g << ' ' << b << " RG" << endl;return *this;
		};

		Graphics & FillColor(double const r, double const g, double const b)
		{
			m_stream << r << ' ' << g << ' ' << b << " rg" << endl;return *this;
		};

		Graphics & FillAndStrokeWR() {m_stream << "B" << endl;return *this;};
		Graphics & FillAndStrokeEO() {m_stream << "B*" << endl;return *this;};
		Graphics & CloseFillAndStrokeWR() {m_stream << "b*" << endl;return *this;};
		Graphics & CloseFillAndStrokeEO() {m_stream << "b*" << endl;return *this;};
		Graphics & FillWR() {m_stream << "f" << endl;return *this;};
		Graphics & FillEO() {m_stream << "f*" << endl;return *this;};
		Graphics & CompletePath() {m_stream << "h" << endl;return *this;};
		Graphics & Stroke() {m_stream << "S" << endl;return *this;};
		Graphics & CompleteAndStroke() {m_stream << "s" << endl;return *this;};

		Graphics & BeginText(double const x, double const y, Name const & font_name, double const points)
		{
			Font(font_name, points);
			m_stream << "BT " << x << ' ' << y << " Td ";return *this;
		};

		template <typename T>
		Graphics & InsertText(T const & text)
		{
			write_string(m_stream, text);
			m_stream << " Tj " << endl;
			return *this;
		}

		Graphics & InsertText(double const x, double const y, string const & text)
		{
			m_stream << x << ' ' << y << " Td ";
			InsertText(text);
			return *this;
		};

		Graphics & InsertText(double const x, double const y, wstring const & text)
		{
			m_stream << x << ' ' << y << " Td ";
			InsertText(text);
			return *this;
		};

		Graphics & EndText()
		{
			m_stream << "ET" << endl;return *this;
		}

		Graphics & InsertImageRGB(double left, double right, double bottom, double top,
			int const w, int const h, void const * image)
		{
			m_stream << "q " << right-left << " 0 0 " << top-bottom << ' ' << left
				<< ' ' << bottom << " cm ";
			m_stream << "BI /W " << w << " /H " << h << " /CS /RGB /BPC 8 ID ";
			m_stream.write(static_cast<char const*>(image), static_cast<streamsize>(w*h*3));
			m_stream << "EI Q" << endl;return *this;
		};

	};

	class Dict : public BaseObject
	{
	private:
		int m_id;
		File * m_file;
		stringstream *m_stream;
		map < Name, BaseObject * >m_items;

		void *operator   new (size_t size)
		{
			return malloc (size);
		};

		void operator   delete (void *p)
		{
			free (p);
		};

		void write (ostream & out) const
		{
			out << m_id << " 0 R";
		};

		void write_direct (ostream & out) const;
		void really_write (ostream & out) const;

		bool managed () const
		{
			return true;
		};

		BaseObject *clone () const
		{
			// TODO: this isn't really a clone.
			return static_cast < BaseObject * >(const_cast < Dict * >(this));
		};

		void release ()
		{
			map < Name, BaseObject * >::iterator iter = m_items.begin ();
			while (iter != m_items.end ())
			{
				if (!iter->second->managed ())
					iter->second->release ();
				++iter;
			}
		};

	public:
		friend class File;

		Dict (File * pdf_file, int id):m_id (id), m_file (pdf_file), m_stream (0)
		{
		};

		~Dict ()
		{
			delete m_stream;
		};

		Array *newArray () const;
		Array *newArray (Name const &);
		template <typename T>
		Array *newArray (T const *, T const *) const;
		template <typename T>
		Array *newArray (Name const &, T const *, T const *);
		Dict *newDict () const;
		Dict *newDict (Name const &);

		template <typename T1> bool get(Name const & key, Array *& value_out)
		{
			bool retval = false;
			map < Name, BaseObject * >::const_iterator iter = m_items.find (key);
			if(iter != m_items.end())
			{
				//We really need to use a dynamic_cast here, but no RTTI yet in HOOPS.
				//T1 value = dynamic_cast<T1>(iter->second);
				T1 value = static_cast<T1>(iter->second);
				if(value)
				{
					value_out = value;
					retval = true;
				}
			}
			return retval;
		}

		template <typename T1> bool get(Name const & key, Dict *& value_out)
		{
			bool retval = false;
			map < Name, BaseObject * >::const_iterator iter = m_items.find (key);
			if(iter != m_items.end())
			{
				//We really need to use a dynamic_cast here, but no RTTI yet in HOOPS.
				//T1 value = dynamic_cast<T1>(iter->second);
				T1 value = static_cast<T1>(iter->second);
				if(value)
				{
					value_out = value;
					retval = true;
				}
			}
			return retval;
		}

		template <typename T1, typename T2> bool get(Name const & key, T2 & value_out) const
		{
			bool retval = false;
			map < Name, BaseObject * >::const_iterator iter = m_items.find (key);
			if(iter != m_items.end())
			{
				//We really need to use a dynamic_cast here, but no RTTI yet in HOOPS.
				//T1 const * value = dynamic_cast<T1*>(iter->second);
				T1 const * value = static_cast<T1*>(iter->second);
				if(value)
				{
					value_out = *value;
					retval = true;
				}
			}
			return retval;
		}

		Dict * appendStream (Graphics g)
		{
			if (!m_stream)
				m_stream = new stringstream(stringstream::binary |
				stringstream::in |
				stringstream::out);
			else
				*m_stream << ' ';

			char buffer[EZPDF_BUFFER_SIZE];
			g.m_stream.read (buffer,
				static_cast < streamsize > (EZPDF_BUFFER_SIZE));
			while (g.m_stream.gcount () > 0)
			{
				m_stream->write (buffer, g.m_stream.gcount ());
				g.m_stream.read (buffer,
					static_cast < streamsize > (EZPDF_BUFFER_SIZE));
			}
			return this;
		}

		Dict * appendStream (void const *begin, size_t size)
		{
			if (!m_stream)
				m_stream = new stringstream(stringstream::binary |
				stringstream::in |
				stringstream::out);
			m_stream->write (static_cast<char const*>(begin), static_cast<streamsize>(size));
			return this;
		};

		int streamSize () const
		{
			int retval = 0;
			if (m_stream)
				retval = m_stream->tellp ();
			return retval;
		};

		Dict * remove (Name const &n)
		{
			map < Name, BaseObject * >::iterator iter = m_items.find (n);
			if(iter != m_items.end()){
				if(!iter->second->managed()){
					iter->second->release();
				}
				m_items.erase(n);
			}
			return this;
		};

		Dict * insert (Name const &n, Array * value);

		Dict * insert (Name const &n, int value)
		{
			remove(n);
			BaseObject *b = new Int (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, float value)
		{
			remove(n);
			BaseObject *b = new Float (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, double value)
		{
			remove(n);
			BaseObject *b = new Float (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, long value)
		{
			remove(n);
			BaseObject *b = new Int (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, size_t value)
		{
			remove(n);
			BaseObject *b = new Int (static_cast<long>(value));
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, bool value)
		{
			remove(n);
			BaseObject *b = new Boolean (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, WString const &value)
		{
			remove(n);
			BaseObject *b = new WString (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, String const &value)
		{
			remove(n);
			BaseObject *b = new String (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, Name const &value)
		{
			remove(n);
			BaseObject *b = new Name (value);
			m_items.insert (make_pair (n, b));
			return this;
		};

		Dict * insert (Name const &n, Dict const * value)
		{
			remove(n);
			Dict * d = const_cast<Dict*>(value);
			m_items.insert (make_pair (n, static_cast < BaseObject * >(d)));
			return this;
		};

	};

	class Array:public BaseObject
	{
	private:
		vector < BaseObject * >m_items;

		void clear ()
		{
			for (unsigned int i = 0; i < m_items.size (); ++i)
			{
				if (!m_items[i]->managed ())
				{
					m_items[i]->release ();
				}
			}
			m_items.clear ();
		};

		void *operator   new (size_t size)
		{
			return malloc (size);
		};

		void operator   delete (void *p)
		{
			free (p);
		};


		BaseObject *clone () const
		{
			Array *tmp = new Array;
			*tmp = *this;
			return tmp;
		};

		void write (ostream & out) const
		{
			out << '[';
			if (!m_items.empty ()){
				m_items[0]->write (out);
				for (unsigned int i = 1; i < m_items.size (); ++i)
				{
					out << ' ';
					m_items[i]->write (out);
				}
			}
			out << ']';
		};

		void release ()
		{
			clear();
		};

		bool managed () const
		{
			return true;
		};

		~Array ()
		{
			clear ();
		};

		Array ()
		{
		};

		Array (int const *begin, int const *end)
		{
			insert (begin, end);
		};

		Array (float const *begin, float const *end)
		{
			insert (begin, end);
		};

		Array (bool const *begin, bool const *end)
		{
			insert (begin, end);
		};

		Array (WString const *begin, WString const *end)
		{
			insert (begin, end);
		};

		Array (String const *begin, String const *end)
		{
			insert (begin, end);
		};

		Array (Name const *begin, Name const *end)
		{
			insert (begin, end);
		};

		Array (Dict ** begin, Dict ** end)
		{
			insert (begin, end);
		};

		Array (Array * begin, Array * end)
		{
			insert (begin, end);
		};
	public:
		friend class File;

		template <typename T1> bool get(unsigned int index, Dict *& value_out)
		{
			bool retval = false;
			if(index < m_items.size())
			{
				BaseObject * bo = m_items[index];
				//We really need to use a dynamic_cast here, but no RTTI yet in HOOPS.
				//T1 value = dynamic_cast<T1>(bo);
				T1 value = static_cast<T1>(bo);
				if(value)
				{
					value_out = value;
					retval = true;
				}
			}
			return retval;
		}

		template <typename T1, typename T2> bool get(unsigned int index, T2 & value_out) const
		{
			bool retval = false;
			if(index < m_items.size())
			{
				BaseObject * bo = m_items[index];
				//We really need to use a dynamic_cast here, but no RTTI yet in HOOPS.
				//T1 * value = dynamic_cast<T1*>(bo);
				T1 * value = static_cast<T1*>(bo);
				if(value)
				{
					value_out = *value;
					retval = true;
				}
			}
			return retval;
		}

		size_t size () const
		{
			return m_items.size ();
		};

		void remove (unsigned int const index)
		{
			if(index < m_items.size()){

				if(!m_items[index]->managed())
					m_items[index]->release();

				m_items.erase(m_items.begin() + index);
			}
		};

		Array & insert (int value)
		{
			m_items.push_back (new Int (value));
			return *this;
		};

		Array & insert (float value)
		{
			m_items.push_back (new Float (value));
			return *this;
		};

		Array & insert (double value)
		{
			m_items.push_back (new Float (value));
			return *this;
		};

		Array & insert (bool value)
		{
			m_items.push_back (new Boolean (value));
			return *this;
		};

		Array & insert (WString const &value)
		{
			m_items.push_back (new WString (value));
			return *this;
		};

		Array & insert (String const &value)
		{
			m_items.push_back (new String (value));
			return *this;
		};

		Array & insert (Name const &value)
		{
			m_items.push_back (new Name (value));
			return *this;
		};

		Array & insert (Dict * value)
		{
			m_items.push_back (value);
			return *this;
		};

		Array & insert (Array * value)
		{
			m_items.push_back (value);
			return *this;
		};

		Array & insert (int const *begin, int const *end)
		{
			while (begin != end)
			{
				m_items.push_back (new Int (*begin));
				++begin;
			}
			return *this;
		};

		Array & insert (float const *begin, float const *end)
		{
			while (begin != end)
			{
				m_items.push_back (new Float (*begin));
				++begin;
			}
			return *this;
		};

		Array & insert (bool const *begin, bool const *end)
		{
			while (begin != end)
			{
				m_items.push_back (new Boolean (*begin));
				++begin;
			}
			return *this;
		};

		Array & insert (WString const *begin, WString const *end)
		{
			while (begin != end)
			{
				m_items.push_back (new WString (*begin));
				++begin;
			}
			return *this;
		};

		Array & insert (String const *begin, String const *end)
		{
			while (begin != end)
			{
				m_items.push_back (new String (*begin));
				++begin;
			}
			return *this;
		};

		Array & insert (Name const *begin, Name const *end)
		{
			while (begin != end)
			{
				m_items.push_back (new Name (*begin));
				++begin;
			}
			return *this;
		};

		Array & insert (Dict ** begin, Dict ** end)
		{
			while (begin != end)
			{
				m_items.push_back (*begin);
				++begin;
			}
			return *this;
		};

		Array & insert (Array *begin, Array *end)
		{
			while (begin != end)
			{
				m_items.push_back (begin);
				++begin;
			}
			return *this;
		};
	};

	Dict * Dict::insert (Name const &n, Array * value)
	{
		remove(n);
		m_items.insert (make_pair (n, static_cast<BaseObject*>(value)));
		return this;
	};

	class File : public BaseObject
	{
	private:
		int m_object_count;
		mutable Dict * m_ptrailer;
		float m_version;
		map < int, long > m_xrefs;
		vector < Dict * > m_dictionaries;
		vector < Array * > m_arrays;
		ofstream m_out;

		BaseObject *clone () const
		{
			return 0;
		};

		void write (ostream & out) const
		{
			out << "%PDF-" << m_version;

			for (unsigned int i = 0; i < m_dictionaries.size (); ++i)
			{
				m_dictionaries[i]->really_write (out);
			}

			out << "\n\n\n";
			long xref_fpos = out.tellp ();
			out << "xref" << endl;
			out << "0 " << m_object_count << endl;

			char cur_fill = out.fill ();
			out.fill ('0');

			for (int i = 0; i < m_object_count; ++i)
			{
				map < int, long >::const_iterator iter = m_xrefs.find (i);
				if (iter == m_xrefs.end ())
					out << "0000000000 65535 f " << endl;
				else
					out << setw (10) << iter->second << " 00000 n " << endl;
			}
			out.fill (cur_fill);

			m_ptrailer->insert("Size", m_object_count);
			out << "\n\n" << "trailer" << endl;
			m_ptrailer->write_direct(out);
			out << "\n\n\n";

			out << "startxref" << endl << xref_fpos << endl << "%%EOF";

			if(out.fail())
				throw false;
		};

		void release ()
		{
		};

	public:
		friend class Dict;

		explicit File (string const & filename):m_object_count (1), m_ptrailer(0),
			m_version (1.7f), m_out (filename.c_str(), ios::out | ios::binary)
		{
		};

		explicit File (wstring const & filename):m_object_count (1), m_ptrailer(0),
			m_version (1.7f)
		{
#if _MSC_VER
			m_out.open(filename.c_str(), ios::out | ios::binary);
#else
			int len = wcstombs(0, filename.c_str(), 0) +1;
			char * tmp = new char[len];
			char const * const old_loc = setlocale(LC_CTYPE, "");
			wcstombs(tmp, filename.c_str(), len);
			setlocale(LC_CTYPE, old_loc);
			m_out.open(tmp, ios::out | ios::binary);
			delete [] tmp;
#endif
		};

		~File ()
		{
			if(m_ptrailer)
				m_ptrailer->release ();
			delete m_ptrailer;

			for (unsigned int i = 0; i < m_dictionaries.size (); ++i)
			{
				m_dictionaries[i]->release ();
			}

			for (unsigned int i = 0; i < m_arrays.size(); ++i)
			{
				m_arrays[i]->release();
			}

			for (unsigned int i = 0; i < m_dictionaries.size (); ++i)
			{
				delete m_dictionaries[i];
			}

			for (unsigned int i = 0; i < m_arrays.size(); ++i)
			{
				delete m_arrays[i];
			}
		};

		void close () {
			if(m_out.is_open()){
				write(m_out);
			}
			m_out.close();
		};

		template <typename T1> bool get(Name const & key, Dict *& value_out)
		{
			return m_ptrailer->get<T1> (key, value_out);
		}

		template <typename T1, typename T2> bool get(Name const & key, T2 & value_out) const
		{
			return m_ptrailer->get<T1> (key, value_out);
		}

		Array * newArray ()
		{
			Array *retval = new Array;
			m_arrays.push_back (retval);
			return retval;
		};

		template <typename T>
		Array * newArray (T const * begin, T const * end)
		{
			Array *retval = new Array(begin, end);
			m_arrays.push_back (retval);
			return retval;
		}

		Dict *newDict ()
		{
			Dict *retval = new Dict (this, m_object_count++);
			m_dictionaries.push_back (retval);
			return retval;
		}

		Dict *newDict (Name const & n)
		{
			Dict *retval = new Dict (this, m_object_count++);
			m_dictionaries.push_back (retval);
			if(!m_ptrailer)
				m_ptrailer = new Dict(this, 0);
			m_ptrailer->insert(n, retval);
			return retval;
		}
	};

	Array * Dict::newArray() const
	{
		return m_file->newArray();
	};

	Array * Dict::newArray(Name const & n)
	{
		Array * retval = newArray();
		insert(n, retval);
		return retval;
	};

	template <typename T>
	Array * Dict::newArray(T const * begin, T const * end) const
	{
		return m_file->newArray(begin, end);
	}

	template <typename T>
	Array * Dict::newArray(Name const & n, T const * begin, T const * end)
	{
		Array * retval = newArray(begin, end);
		insert(n, retval);
		return retval;
	}

	Dict * Dict::newDict() const
	{
		Dict *retval = new Dict (m_file, m_file->m_object_count++);
		m_file->m_dictionaries.push_back (retval);
		return retval;
	};

	Dict * Dict::newDict(Name const & n)
	{
		Dict *retval = newDict();
		insert(n, retval);
		return retval;
	};

	void Dict::write_direct (ostream & out) const
	{
		out << "<<";

		map < Name, BaseObject * >::const_iterator iter = m_items.begin ();
		while (iter != m_items.end ())
		{
			out << endl;
			iter->first.write (out);
			out << ' ';
			iter->second->write (out);
			++iter;
		}

		out << endl << ">>";
	};

	void Dict::really_write (ostream & out) const
	{
		if (m_file->m_xrefs.find (m_id) == m_file->m_xrefs.end ())
		{

			out << "\n\n\n";
			long dict_fpos = out.tellp ();
			m_file->m_xrefs.insert (make_pair (m_id, dict_fpos));
			out << m_id << " 0 obj\n";

			write_direct(out);

			out << endl;

			if (m_stream)
			{
				out << "stream" << endl;
				char buffer[EZPDF_BUFFER_SIZE];
				m_stream->read (buffer,
					static_cast < streamsize > (EZPDF_BUFFER_SIZE));
				while (m_stream->gcount () > 0)
				{
					out.write (buffer, m_stream->gcount ());
					m_stream->read (buffer,
						static_cast < streamsize >
						(EZPDF_BUFFER_SIZE));
				}
				out << endl << "endstream" << endl;
			}

			out << "endobj";
		}
	};

	namespace filters {
		class filter {
		public:
			virtual ~filter() {};
			virtual filter & read(void*, streamsize) = 0;
			virtual filter & write(void const *, streamsize) = 0;
			virtual streamsize gcount() const = 0;
			virtual char const * name() const = 0;
		};

		class zlib : public filter {
		public:
			zlib() : m_reading(false){
				m_zstrm.zalloc = Z_NULL;
				m_zstrm.zfree = Z_NULL;
				m_zstrm.data_type = Z_ASCII;
				m_zstrm.next_out = 0;
				m_zstrm.avail_out = 0;
				m_zstrm.next_in = 0;
				m_zstrm.avail_in = 0;
				if(deflateInit(&m_zstrm, Z_BEST_COMPRESSION) != Z_OK){
					assert(0);
				}
			};

			~zlib() {
				deflateEnd(&m_zstrm);
			};

			filter & read(void * out, streamsize howmuch) {
				if(!m_reading){
					char buffer[EZPDF_BUFFER_SIZE];
					int rc;

					do{
						m_zstrm.next_out = (Bytef*)(&buffer[0]);
						m_zstrm.avail_out = EZPDF_BUFFER_SIZE;
						rc = deflate(&m_zstrm, Z_FINISH);

						int have = EZPDF_BUFFER_SIZE - m_zstrm.avail_out;
						if(have){
							m_buf.write(buffer, have);
						}
					}while(rc != Z_STREAM_END);
					m_reading = true;
				}

				m_buf.read((char*)out, howmuch);
				return *this;
			};

			filter & write(void const * in, streamsize howmuch) {
				if(!m_reading){
					m_zstrm.next_in = const_cast<Bytef*>(static_cast<Bytef const*>(in));
					m_zstrm.avail_in = howmuch;

					char buffer[EZPDF_BUFFER_SIZE];

					m_zstrm.next_out = (Bytef*)(&buffer[0]);
					m_zstrm.avail_out = EZPDF_BUFFER_SIZE;

					while(m_zstrm.avail_in > 0){
						int rc = deflate(&m_zstrm, Z_NO_FLUSH);
						if(rc != Z_OK)
							assert(0);

						int have = EZPDF_BUFFER_SIZE - m_zstrm.avail_out;
						if(have){
							m_buf.write(buffer, have);
							m_zstrm.next_out = (Bytef*)(&buffer[0]);
							m_zstrm.avail_out = EZPDF_BUFFER_SIZE;
						}
					}
					int have = EZPDF_BUFFER_SIZE - m_zstrm.avail_out;
					if(have){
						m_buf.write(buffer, have);
					}
				}

				return *this;
			};

			streamsize gcount() const {
				return m_buf.gcount();
			};

			char const * name() const {return "FlateDecode";};

		private:
			std::stringstream m_buf;
			struct z_stream_s m_zstrm;
			bool m_reading;
		};
	}
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif

#endif
