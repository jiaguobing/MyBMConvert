// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "BStream.h"
#include "BOpcodeHandler.h"

#include "assertbreak.h"


//////////////////////////////////////////////////////////////////////////


namespace ooc
{
	// Purpose of this class is to help lift
	//		BBaseOpcodeHandler::GetData
	//		BBaseOpcodeHandler::PutData
	//	to top level functions.
	class GetData_PutData : private BBaseOpcodeHandler {
	private:
		GetData_PutData ();

	public:
		template <typename T>
		inline static TK_Status GetData (BStreamFileToolkit & tk, T & out)
		{
			return BBaseOpcodeHandler::GetData(tk, out);
		}

		template <typename T>
		inline static TK_Status GetData (BStreamFileToolkit & tk, T * out, int n)
		{
			return BBaseOpcodeHandler::GetData(tk, out, n);
		}

		template <typename T>
		inline static TK_Status PutData (BStreamFileToolkit & tk, T const & in)
		{
			return BBaseOpcodeHandler::PutData(tk, in);
		}

		template <typename T>
		inline static TK_Status PutData (BStreamFileToolkit & tk, T const * in, int n)
		{
			return BBaseOpcodeHandler::PutData(tk, in, n);
		}

		inline static TK_Status LookatData (BStreamFileToolkit & tk, unsigned char & b)
		{
			return BBaseOpcodeHandler::LookatData(tk, b);
		}
	};


	template <typename T>
	inline TK_Status GetData (BStreamFileToolkit & tk, T & out)
	{
		return GetData_PutData::GetData(tk, out);
	}


	template <typename T>
	inline TK_Status GetData (BStreamFileToolkit & tk, T * out, int n)
	{
		return GetData_PutData::GetData(tk, out, n);
	}


	template <typename T>
	inline TK_Status PutData (BStreamFileToolkit & tk, T const & in)
	{
		return GetData_PutData::PutData(tk, in);
	}


	template <typename T>
	inline TK_Status PutData (BStreamFileToolkit & tk, T const * in, int n)
	{
		return GetData_PutData::PutData(tk, in, n);
	}


	inline TK_Status LookatData (BStreamFileToolkit & tk, unsigned char & b)
	{
		return GetData_PutData::LookatData(tk, b);
	}
}


