// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HOpNetwork_H
#define _HOpNetwork_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HBaseView.h"

//! The HOpNetwork class builds a network of nodes and connections.
/*! 
  The HOpNetwork class implements the following actions to build a network of nodes and connections:
  <ol>
  <li>Left Button Down:				Place or select a node under the mouse.
  <li>Left Button Down and Drag:	Reposition the selected node under the mouse and update its connections.
  <li>Left Button Up:				Set the operator to a default inactive state.
  <li>Right Button Down:			Select one node and anchor a connection to it.
  <li>Right Button Down and Drag:	Reposition the other end of the connection.
  <li>Right Button Up:				If there's a node under the connection, anchor the loose end of the connection to it.  Otherwise, delete the connection.
  </ol>
*/
class MVO_API HOpNetwork : public HBaseOperator
{
public:
	/*! Constructs an HOpNetwork object. 
	\param view A pointer the HBaseView that this operator is associated with.
	*/
	HOpNetwork(HBaseView * view);

	virtual ~HOpNetwork();

	/*! This method places and/or select a node.  It also sets the operator as active. 
	  \param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*! This method moves the selected node and update its connections. 
	  \param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!This method sets the operator as inactive. 
	  \param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent);


	/*! If there's a node under the mouse, this method anchors a new connection to it. 
		  \param hevent An HEventInfo object containing information about the current event.
		  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonDown(HEventInfo &hevent);

	/*! This method repositions the loose end of the connection. 
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonDownAndMove(HEventInfo &hevent);

	/*! If there's a node under the mouse, this method anchors the loose connection to it.  If not, 
	then it deletes the connection. 
	  \param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonUp(HEventInfo &hevent);


protected:
	
	HC_KEY m_NetworkKey;			//< The key to the network segment.
	HC_KEY m_NodesKey;				//< The key to the node segment where all the nodes are inserted.
	HC_KEY m_SelectedNodeKey;		//< The current selected node.
	HC_KEY m_SelectedLineKey;		//< The current selected connection.

	/*! The method inserts a line and the text measurement of the line in the middle.
	\param x1 The x coordinate for one end of the line.
	\param y1 The y coordinate for one end of the line.
	\param z1 The z coordinate for one end of the line.
	\param x2 The x coordinate for the other end of the line.
	\param y2 The y coordinate for the other end of the line.
	\param z2 The z coordinate for the other end of the line.
	\return The key associated with the inserted line.
	*/
	HC_KEY My_KInsert_Line(float x1, float y1, float z1, float x2, float y2, float z2);

	/*! This is a helper function for the custom line functions. It calculates the length of the line and
	inserts the text into the middle.
	\param key The key to insert the text measurement.
	\param x1 The x coordinate for one end of the line.
	\param y1 The y coordinate for one end of the line.
	\param z1 The z coordinate for one end of the line.
	\param x2 The x coordinate for the other end of the line.
	\param y2 The y coordinate for the other end of the line.
	\param z2 The z coordinate for the other end of the line.
	*/
	void My_Line_Common(HC_KEY key, float x1, float y1, float z1, float x2, float y2, float z2);

	/*! This method edits the line position and measurement text. 
	\param key The key to the line.
	\param x1 The x coordinate for one end of the line.
	\param y1 The y coordinate for one end of the line.
	\param z1 The z coordinate for one end of the line.
	\param x2 The x coordinate for the other end of the line.
	\param y2 The y coordinate for the other end of the line.
	\param z2 The z coordinate for the other end of the line.
	*/
	void My_Edit_Line(HC_KEY key, float x1, float y1, float z1, float x2, float y2, float z2);

	/*! This method deletes the line and the associated measurement text given a key.
	\param key The key in which the line exists.
	*/
	void My_Delete_Line(HC_KEY key);

	/*! This method inserts a node at the given position.
	\param x The x coordinate of the node position.
	\param y The y coordinate of the node position.
	\param z The z coordinate of the node position.
	\return The key representing the node in the HOOPS database.
	*/
	HC_KEY My_KInsert_Node(float x, float y, float z);

	/*! This method move the given node to a new position.
	\param key The key to the node.
	\param x The x coordinate of the new node position.
	\param y The y coordinate of the new node position.
	\param z The z coordinate of the new node position.
	*/
	void My_Edit_Node(HC_KEY key, float x, float y, float z);

	/*! This method returns the position of the node. 
	\param key The key to the node.
	\param x Returns the x coordinate of the node position.
	\param y Returns the y coordinate of the node position.
	\param z Returns the z coordinate of the node position.
	*/
	void Show_Node_Position(HC_KEY key, float &x, float &y, float &z);

	/*! \return The node (segment) key or -1 if the key doesn't belong to a node. */
	HC_KEY Get_Node_Key(HC_KEY key);

	/*! This method opens a given node.
	\param key The key to the node to open.
	*/
	void My_Open_Node(HC_KEY key);

	/*! This method closes a given node.
	\param key The key to the node to close.
	*/
	void My_Close_Node();

	/*! This method renumbers the given key locally to help in round tripping the data. 
	\param key The key to renumber.
	\return The newly renumbered key.
	*/
	HC_KEY My_Renumber_Key(HC_KEY key);

	/*! \return The next available key for this network. */
	HC_KEY GetNextKey();

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif







