// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once


//////////////////////////////////////////////////////////////////////////


namespace ooc
{
	// Based on MSVC 2008 implementation of [rand] and [srand]
	class Rand {
		class CopyForwarder {
			friend class Rand;

		private:
			CopyForwarder (Rand const & r);
			CopyForwarder & operator= (CopyForwarder const &);

		private:
			Rand const & r;
		};

		friend class CopyForwarder;

	public:
		Rand (unsigned int seed);
		void Seed (unsigned int seed);
		int Next ();

	public:
		static int const MAX;

	private:
		// Prevent copy and assignment operators to prevent
		// accidental copy when passing to functions.
		// 
		// Use references instead:
		// Rand & r2 = r1;
		// 
		// Or you can use the clone() method to copy it:
		// Rand r2 = r1.clone();
		//
		Rand (Rand const &);
		Rand & operator= (Rand const & r);

	public:
		CopyForwarder Clone () const;
		Rand (CopyForwarder const & cf);
		Rand & operator= (CopyForwarder const & cf);

	private:
		unsigned long state;
	};
}



