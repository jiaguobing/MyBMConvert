// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef H_MARKUP_MANAGER_H
#define H_MARKUP_MANAGER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HUtility.h"

 
class HBaseView;

/*! The HMarkupManager class allows users to manage the different Markup operators.  Via this class
	users can save and restore markup information as well as set color and weights on the markup data. 
 */

class  MVO_API HMarkupManager {

public:
	/*!	Constructs an HMarkupManager object.*/
	HMarkupManager(HBaseView *view); 


	/*! If the markup segment is deleted and recreated (for example by an opcode handler in HSF), reinit reinitializes the 
	    internals of HMarkupManager to reflect the changes. */
	void ReInit(HBaseView *view);

	virtual ~HMarkupManager(); 

	/*!	Opens a markup layer.  If the layer does not exist then a new layer will be created. 

		\param layername if layername refers to an existing segment it will be used to restore that layer, otherwise it will be ignored.
		\param setcamera bool indicating whether the camera used when this layer was initially created should also be restored
		\param emit_message bool This parameter has been deprecated.  Indicating whether or not a message should be transmitted to
		possible members of a HNet session
	*/
 	virtual HC_KEY	OpenLayer(const char *layername, bool setcamera = true, bool emit_message = true);

	/*! Renames an existing layer */
	virtual HC_KEY  RenameLayer(HC_KEY laykey, char const * new_name);

	/*! Copy an existing layer */
	virtual HC_KEY  CopyLayer(HC_KEY laykey, char const * new_name);

	/*! Returns the camera for a layer key. If a camera is set, true is returned. */
	virtual bool GetCamera(HC_KEY layerkey, HCamera & cam) const;

	/*!	Opens a Markup layer.  If the layer does not exist then a new layer will be created 

		\param layerkey if layerkey refers to an existing segment it will be used to restore that layer, otherwise it will be ignored.
		\param setcamera bool indicating whether the camera used when this layer was initially created should also be restored
	*/
	virtual HC_KEY	OpenLayer(HC_KEY layerkey, bool setcamera = true);


	/*!	Check if a named Markup layer exists.  If the layer exist return its key, if not, return -1 

		\param layername Named layer to check exist
	*/
	virtual HC_KEY	FindLayer(const char *layername) const;

	/*!	Returns a simicolon separated list of view names such as "Default View;View1;View2;Awesome View;" */
	virtual char const * GetLayerNames() const;


	/*!	\return The key to the currently active layer.
	*/
	virtual HC_KEY  GetCurrentLayerKey() const;

	/*!	\return The name of the currently active layer.
	*/
	virtual char const *  GetCurrentLayerName() const;


	/*!	Turns off the visibility of the currently active layer.

		\param emit_message bool This parameter has been deprecated.  Indicating whether or not a message should be transmitted to
		possible members of a HNet session.
	*/
	virtual void	DisableCurrentLayer(bool emit_message = true);

 
	/*!	Deletes a markup layer.

		\param layername  name of the layer to be deleted.  Name should be the name that was
		initially provided to OpenLayer.
	*/
	virtual void	DeleteLayer(const char *layername);

	/*!	Deletes a markup layer.

		\param layerkey The key to the layer which needs to be deleted.
	*/
	virtual void	DeleteLayer(HC_KEY layerkey);

 
	/*!	Set default layer. If no layer is open, all new markup geometry will be added to the default layer
		\param layerkey The key to the new default layer.
 	*/
	void			SetDefaultLayerKey(HC_KEY layerkey) {m_DefaultLayerKey = layerkey; }

	/*!	Get default layer. If no layer is open all new markup, geometry will be added to the default layer
		\return Key to the default layer.
 	*/
	HC_KEY			GetDefaultLayerKey() {return m_DefaultLayerKey; }

	/*!	Gets the top level markup key.
		\return Key to the parent segment for all markup layers.
 	*/
 	HC_KEY			GetMarkupKey() {return m_MarkupKey; }

	/*!	Gets the key to the segment containing the individual markup layers.
		\return Key to segment containing the individual markup layers.
 	*/
	HC_KEY			GetMarkupLayerKey() {return m_MarkupLayerKey; }

	/*!	\return The existing line/edge weight of markup data.
	*/
	virtual float	GetMarkupWeight(){return m_fMarkupWeight;};

	/*!	Sets the line and edge weight of Markup data.  It is applied to the current active 
	layer and all subsequent layers.

		\param weight the pixel width of the line/edge data
	*/
	virtual void	SetMarkupWeight(float weight);

	/*!	Returns the existing color of markup data.
		\param markup_color	This method populates the parameter with the color of the markup data.
	*/
	virtual void	GetMarkupColor(HPoint &markup_color){markup_color.Set(m_MarkupColor);};

	/*!	Sets the color of Markup Data.  It is applied to the current active 
	layer and all subsequent layers.

		\param markup_color	RGB color of Markup data
	*/
	virtual void	SetMarkupColor(HPoint markup_color);

// HNet Work

	/*!	This method has been deprecated.  Sends data from a Markup Operator to other members of a HNet session.
		\param key key to the markup segment you want to transmit.  Note, this is not a layer but rather the
		segment that a markup operator is inserting geometry.
	*/
	virtual void	EmitMarkupSegment(HC_KEY key);

	/*!	This method has been deprecated.  Function which receives markup data from HNet server.

		\param in_data HNet data
		\param data_length length of in_data
	
	*/
	virtual void	InsertMarkupSegmentFromMessage(const char *in_data, unsigned int data_length);

	/*!	This method has been deprecated.  Sends a message to other members of a HNet session to disable the current markup layer.
	*/
	virtual void	EmitDisableCurrentMarkupLayer();

	/*!	This method has been deprecated.  Function which receives disable current layer message from HNet Server.

		\param data HNet data
		\param data_length length of in_data
	*/
	virtual void	DisableCurrentMarkupLayerFromMessage(const char *data, unsigned int data_length);	

	/*!	This method has been deprecated.  Sends a message to other members of a HNet session to create/restore a specific markup layer.
	*/
	virtual void	EmitSetMarkupLayer(const char *layer);

	/*!	This method has been deprecated.  Function which receives the create/restore MarkupLayer message from the HNet Server.

		\param data HNet data
		\param data_length length of in_data
	*/
	virtual void	SetMarkupLayerFromMessage(const char *data, unsigned int data_length);

	/*! Returns a preview image to the markup */
	virtual bool	GetPreviewImage(char const *, int*, int*, int*, unsigned char const**);


	virtual void	UpdatePreviewImage(bool forceUpdate = false);
	bool			IsCameraLayer(HC_KEY layerkey);

	/*
 	get and apply condition on model segments
 	*/
	void			SetLayerCondition(HC_KEY layerkey);
 	/*
 	reset conditions applied on model segments
 	*/
	void			UnsetCurrentLayerCondition();

	void			UnsetLayerCondition(HC_KEY layerkey);

	void			SetGeneratePreviewImage(bool onoff) {m_bGeneratePreviewImage = onoff; }
	bool			GetGeneratePreviewImage()      { return m_bGeneratePreviewImage; }
	void			SetNoResetOnCameraChange();
	bool			GetResetOnCameraChange(HC_KEY layerkey);


protected:
	/*!	Setup default attributes/camera settings for new markup layer.
 	*/
	virtual void	SetupNewLayer();
	/*!	Switches to new layer and restores camera settings if appropriate
		\param layerkey layer to open
		\param setcamera set camera to stored layer settings
 	*/

	virtual void	OpenLayerInternal(HC_KEY layerkey, bool setcamera);
	/*!	 Creates new unique layer name.
		\param newlayername pointer to new layer name
		\return The new layer name.
 	*/
	virtual char	*FindValidLayerName(char *newlayername);
	HBaseView		*m_pHView;				//!< A pointer to the view object.
	HC_KEY			m_ActiveLayerKey;		//!< Currently open markup layer
	mutable char *	m_ActiveLayerName;		//!< Currently open lay name
	HC_KEY			m_DefaultLayerKey;		//!< Default layer (gets used if no other layer is open)
	HC_KEY			m_MarkupKey;			//!< Key to the top level markup layer
	HC_KEY			m_MarkupLayerKey;		//!< Key to parent segment of all layers
 	HPoint			m_MarkupColor;			//!< Color of markup geometry
	float			m_fMarkupWeight;		//!< Weight of markup geometry
	mutable char *	m_allNames;
	bool			m_bGeneratePreviewImage;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif














