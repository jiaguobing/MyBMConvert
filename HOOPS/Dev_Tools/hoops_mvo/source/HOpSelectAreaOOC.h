// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once


#ifdef HMFC_STATIC_LIB
#	error "Code cannot be statically linked."
#endif


#ifdef H_PACK_8
#	pragma pack(push)
#	pragma pack(8)
#endif


//////////////////////////////////////////////////////////////////////////


#include "HOpSelectArea.h"
#include "HSelectionSetOOC.h"


//////////////////////////////////////////////////////////////////////////


/*! The HOpSelectAreaOOC class computes a selection list for OOC points inside a rectangular area.
	If this class is used by an HBaseView, the view must have an HSelectionSetOOC<SelectionSet>
	instance for its selection set.
	\param SelectionSet The selection set type used by HSelectionSetOOC<SelectionSet>
*/
template <typename SelectionSet>
class HOpSelectAreaOOC : public HOpSelectArea {
public:
	/*! Constructs an HOpSelectAreaOOC.
		\param view A reference to an HBaseView object.
	*/
	HOpSelectAreaOOC (HBaseView & view)
		: HOpSelectArea(&view)
	{}

	/*!	\return A pointer to a character string denoting the name of the operator, which is "HOpSelectAreaOOC".
	*/
	virtual char const * GetName ()
	{
		return "HOpSelectAreaOOC";
	}

	/*! Creates a new operator which is associated with the same view as the current operator.
		The user is responsible for deleting the newly created operator.
		\return A pointer to a copy of this operator.
	  */
	virtual HBaseOperator * Clone ()
	{
		return new HOpSelectAreaOOC(*GetView());
	}

	/*! Starts to create a selection box that will select points inside it.
		\param e An HEventInfo object containing information about the current event.
		\return A value indicating the status of the event.
	*/
	virtual int OnLButtonDown (HEventInfo & e)
	{
		if (!e.Control()) {
			GetSelectionSet().DeSelectAll();
		}
		return HOpSelectArea::OnLButtonDown(e);
	}

	/*! Finalizes the selection box created by OnLButtonDown and selects points that lie
		inside the box.
		\param e An HEventInfo object containing information about the current event.
		\return A value indicating the status of the event.
	*/
	virtual int OnLButtonUp (HEventInfo & e)
	{
		HSelectionSetOOC<SelectionSet> & selset = GetSelectionSet();

		return selset.SynchronizeWith([&] () -> int {
			int const result = HOpSelectArea::OnLButtonUp(e);

			ooc::Point window_min(m_ptRectangle[0].x, m_ptRectangle[0].y, 0);
			ooc::Point window_max(m_ptRectangle[1].x, m_ptRectangle[1].y, 0);

			selset.AddRectangleWindow(window_min, window_max);

			return result;
		});
	}

private:
	HSelectionSetOOC<SelectionSet> & GetSelectionSet ()
	{
		HBaseView & view = *GetView();
		HSelectionSet & selset = *view.GetSelection();
		return static_cast<HSelectionSetOOC<SelectionSet> &>(selset);
	}
};



