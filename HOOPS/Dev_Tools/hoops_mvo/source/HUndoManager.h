// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HUNDOMANAGER_H
#define _HUNDOMANAGER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "HTools.h"
#include "HEventListener.h"

class HEventListener;
class HBaseView;


/*! 
	HUndoItem is the base class for all undo items which hold information necessary to 
	perform a rollback of a particular scene.  Use this class as a base class when you 
	want to create new undo items to be managed by the HUndoManager.
*/ 
class MVO_API HUndoItem
{
public:
	/*!
	Constructs an HUndoItem object.
 	*/
	HUndoItem() { ; }
	virtual ~HUndoItem() { ; }

	/*!
		This method restores the conditions captured by the item when it was first created.
		\param setupOnly Pass true if you want to perform the undo without updating the view.  
		The default value is false.
 	*/
	virtual void Undo(bool setupOnly = false) { UNREFERENCED(setupOnly); }
	/*!
		This method restores the conditions just before the undo method was called. 
 		\param setupOnly Pass true if you want to perform the redo without updating the view.  
		The default value is false.
	*/
	virtual void Redo(bool setupOnly = false) { UNREFERENCED(setupOnly); }

	/*!
		\return The item type name which is 'HUndoItem'.
	*/
	virtual const char * GetName() { return "HUndoItem"; }

};

 
/*! 
	HUndoItemCamera stores a camera state for undo and redo.
*/ 
class MVO_API HUndoItemCamera : public HUndoItem
{
public:
	/*!
	Constructs an HUndoItemCamera object.
	\param view The HBaseView associated with this undo item.
 	*/
	HUndoItemCamera(HBaseView *view);
	virtual ~HUndoItemCamera() { ; }

	/*!
		This method restores the scene to be viewed with the initial
		camera state when this item was created.
		\param setupOnly Pass true if you want to perform the undo without updating the view.  
		The default value is false.
 	*/
	virtual void Undo(bool setupOnly = false);
	/*!
		This method returns the scene to being viewed by the camera that was 
		used prior to the undo method being called.
 		\param setupOnly Pass true if you want to perform the redo without updating the view.  
		The default value is false.
 	*/
	virtual void Redo(bool setupOnly = false);

	/*!
	\returns A pointer to this item's camera.
 	*/
	virtual HCamera * GetCamera() { return &m_Camera; }

	/*!
		\return The item type name which is 'HUndoItemCamera'.
	*/
	virtual const char * GetName() { return "HUndoItemCamera"; }
protected:
	HCamera m_Camera;					/*!< Camera Information */
	HBaseView *m_pView;					/*!< Pointer to view associated to this item */
};


/*! 
	HUndoItemMatrix
	MatrixUndo Item that stores a segment key and its associated matrix.
*/ 
class MVO_API HUndoItemMatrix : public HUndoItem
{
public:
	/*!
	Constructs an HUndoItemMatrix object.
 	*/
	HUndoItemMatrix(HBaseView *view, int num_items, HC_KEY keys[], float matrices[]);
	virtual ~HUndoItemMatrix();

	/*!
		This method restores the transformation matrices applied to the given segments when
		this undo item was created.  Note that it also stores the current modelling matrices 
		before making changes so the item can perform a redo.
 		\param setupOnly Pass true if you want to perform the undo without updating the view.  
		The default value is false.
	*/
	virtual void Undo(bool setupOnly = false);
	/*!
		This method restores the transformation matrices applied to the given segments prior to
		undo being called.  
  		\param setupOnly Pass true if you want to perform the redo without updating the view.  
		The default value is false.
	*/
	virtual void Redo(bool setupOnly = false);

	/*!
		\return The number of segments referenced by this item.
 	*/
	virtual int GetCount() { return m_Count; }

	/*!
		\return The list of item keys that this item records and perform transformations on.
 	*/
	virtual HC_KEY * GetKey() { return m_SegmentKey; }
	virtual void GetKey(HC_KEY keys[]);

	/*!
		\return A pointer to the list of item transformation matrices that will be applied to the
		segment list.
 	*/
	virtual float * GetMatrix() { return m_Matrix; }
	virtual void GetMatrix(float m[]);

	/*!
		\return The item type name which is 'HUndoItemMatrix'.
	*/
	virtual const char * GetName() { return "HUndoItemMatrix"; }
protected:
	int m_Count;						/*!< Number of segments referenced by this undo item. */
	float *m_Matrix;					/*!< The modelling matrices associated with the segment list. */
	HC_KEY *m_SegmentKey;				/*!< The keys of the segments associated with matrices. */
	HBaseView *m_pView;					/*!< A pointer to view associated to this item */ 
};



/*! 
	HUndoManager manages the undo stack.  It performs redo and undos.  You can also add undo items to the 
	list, get the next item on the list and peek at the current item. 
*/ 
class MVO_API HUndoManager  
{
public:
	/*!
	Constructs an HUndoManager object.
 	*/
	HUndoManager();
	virtual ~HUndoManager();

	/*!
		This method restores the scene to its previous state.
  		\param steps Number of steps to roll back
		\param testOnly Pass True if you only want to validate that undos are available.
		\param setupOnly Pass True to perform undos without updating the display.
		\return The number of undo steps performed.
 	*/
	int Undo(int steps = 1, bool testOnly = false, bool setupOnly = false);
	/*!
		This method restores the scene to the state prior to the rollback.
  		\param steps Number of steps to redo
		\param testOnly Pass True if you only want to validate that redos are available.
		\param setupOnly Pass True to perform redos without updating the display.
		\return The number of redo steps performed.
 	*/
	int Redo(int steps = 1, bool testOnly = false, bool setupOnly = false);

	/*!
		The method adds <i>item</i> to the undo list at the current cursor position.  It
		also remove all items after the current cursor position from
		the undo list.
  		\param item Undo Item
  	*/
	void AddUndoItem(HUndoItem *item);

	/*!
		This method removes all undos and redos from the undo list.
   	*/
	void Flush();

	/*!
		\return A pointer to the current undo item.
	*/
	HUndoItem * Peek();
 
	/*!
		This method smoothly transitions the scene back to its original state.
	*/
	void Reset(HBaseView *view);

	/*!
		This method begins iteration of the undo list by setting the correct cursor positions.
	*/
	void BeginUndoItemIteration();

	/*!
		This methods returns the next undo item in the list and moves the cursor to the
		next item in the list.
		\return The next undo item.
	*/
	HUndoItem *GetNextUndoItem();

	/*!
		This method ends iteration of the undo list by resetting the cursor of the undo list.
	*/
	void EndUndoItemIteration();

private:

	struct vdlist_s *			m_pUndoItemList;			/*!< Undo List */
	struct vdlist_cursor_s *	m_CurrentItem;				/*!< List Cursor */
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




