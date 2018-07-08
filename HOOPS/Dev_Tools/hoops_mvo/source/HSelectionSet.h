// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HSelectionSet.h 
	Interface of the HSelectionSet class
	Manages list of selected (highlighted) items for view
	*/


#ifndef _HSELECTIONSET_H
#define _HSELECTIONSET_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HGlobals.h"


/*! \enum HSelectLevel */
/*!
  \addtogroup Selection_Level
  @{
*/
enum HSelectLevel 
{
	HSelectSegment,		//!< select on a segment
	HSelectEntity,      //!< select on an entity (piece of geometry, cutting plane or light)
	HSelectRegion,      //!< select on pre-defined regions of an entity
	HSelectSubentity	//!< select on arbitrary subentities (face, edge, or vertex)
};
/*!  @}  */



//forward declaration
class HBaseView;
class HSelectionItem;
class HShellObject;
class HVHashPShowCache;
class HSelHash;
class HSelList;
class HSelectionSegmentConditionInfo;
class HConditionStyleKeyInfoHash;




//! The HSelectionSet class manages a list of selected items.
/*!
  HSelectionSet creates and stores a list of HOOPS geometric primitives or segments that are defined by
  operators which use the HOOPS routine HC_Compute_Selection. The class supports adding, removing individual and
  multiple items from the list.  It encapsulates the logic used to check whether an item is already selected.  
  Items that are selected (those in the list) are highlighted in the scene. HSelectionSet can be easily extended 
  to support control of highlight mode and style (for example, color, lineweight, boundingbox style, or 
  2d_quickmoved geometry).
*/
class MVO_API HSelectionSet 
{
	friend class HBaseView;


protected:
	HBaseView * m_pView;				//!< The HOOPS HBaseView associated with to this HSelectionSet.
	HSelList * m_pSelItemsList;		//!< The list of selected HOOPS entities (HSelectionItem pointers).
	struct vhash_s	*m_pHighlightSegsHash;	//!< The list of highlighted segment keys.
	HC_KEY		m_SelectionSegment;		//!< The key of the reference selection segment.
	HC_KEY		m_TransparencyStyle;	//!< The key to a style used in inverse transparency highlighting.
 
	HPixelRGBA	m_selectedFaceColor;	//!< The color used for selected faces.
	HPixelRGBA	m_selectedEdgeColor;	//!< The color used for selected edges.
	HPixelRGBA	m_selectedCutEdgeColor;	//!< The color used for selected cut edges.
	HPixelRGBA	m_selectedCutFaceColor;	//!< The color used for selected cut faces.
	HPixelRGBA	m_selectedMarkerColor;	//!< The color used for selected markers.

	float		m_fselectedEdgeWeight;	//!< The line thickness used for selected edges.
	float		m_fselectedMarkerSize;	//!< The marker size used for selected markers.
	
	bool		m_bForceGrayScale;		//!< This flag indicating whether to use gray scale selection.
	bool		m_bForceInvisible;		//!< This flag indicating whether to make the selected items invisible.
	bool		m_bAllowDisplacement;	//!< This indicates whether selected segments should be drawn in front of coincident geometry.
	bool		m_bHighlightRegionEdges;//!< This indicates whether edges in a region should be highlighted.
	bool		m_bHighlightAnnotations;//!< This indicates whether MVO annotations should be highlighted.
	HRegionEdgeHighlightMode m_bHighlightRegionEdgesAutoVisibility; //!< This indicates whether edges in a region should be automatically made visible when highlighted.
	HRegionFaceHighlightMode m_bHighlightRegionFacesAutoVisibility; //!< This indicates whether faces in a region should be automatically made visible when highlighted.

	HSelectionHighlightMode m_HighlightMode; //!< Stores the current highlighting mode.
	float		m_fTransmission;		//!< the transmission value used in Inverse Transparency highlighting
	bool		m_bSortSelectionList;	//!< If this is set, the selection list will be sorted before returning it.

	bool		m_bNotify; //!< If this is set Select() will send HSignalSelected for each item.

	bool		m_bUseDefinedHighlight;


	/*! Used internally to update the transparency used in inverse transparency highlighting
		\param force_flush If true, the transparency will be removed even if there are still selected items.		
	*/
	virtual void UpdateTransparency(bool force_flush=false);

public:
	/*! Construct an HSelectionSet object.*/
	HSelectionSet(HBaseView* view, bool bReferenceSelection = false);

	virtual ~HSelectionSet();

	/*!
		Initializes the selection list and updates all selection attributes in the associated view.
	*/
	virtual void Init();


	/*! Adds the item identified by 'key' to the selection list.  If the item is geometry, that geometry is moved to a 
		highlight segment (temporary segment with new attributes). If the item is a segment, the segment's attributes are directly 
		modified.

		This method Selects a specific instance of a segment/geometric-primitive.  This instance selection
		is necessary if a driver segment/subsegment with 'view' attributes is including a segment in the 'include library', and
		you only want that particular reference of the item to be selected/deselected (so that the item will highlight/dehighlight 
		in 1 view rather than all views)  If selection on a particular instance is not necessary, then set num_include_keys to 0, and set
		include_keys to NULL

		If this method is overloaded, then the matching DeSelect and IsSelected methods should also be overloaded.  
		
		\param key The key of the item to add.
		\param num_include_keys The number of include keys in the path upto the driver key.
		\param include_keys An array of include keys.
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void Select(HC_KEY key, int num_include_keys, HC_KEY include_keys[], bool emit_message = false);	

	/*!
		Adds a custom selection item to the selection list.  If one of the standard selection items is to be used,
		there is no compelling reason to use this function.  Rather it is offered as a way of getting derived
		selection items into the selection list and having them behave in a natural way without violating the 
		encapsulation of the HSelectionSet.

		Very little work will be done with the selection item.  You can expect to have its time stamp set and its
		Highlight() function called.

		\param new_sel A pointer to a selection item.
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void Select(HSelectionItem * new_sel, bool emit_message = false);

	/*! Adds a region of a shell identified by 'key' to the selection list.  

		This method Selects a specific instance of a segment/geometric-primitive.  This instance selection
		is necessary if a driver segment/subsegment with 'view' attributes is including a segment in the 'include library', and
		you only want that particular reference of the item to be selected/deselected (so that the item will highlight/dehighlight 
		in 1 view rather than all views)  If selection on a particular instance is not necessary, then set num_include_keys to 0, and set
		include_keys to NULL

		If this method is overloaded, then the matching DeSelect method should also be overloaded.  
		
		\param oShell The HShellObject to which the region belongs.
		\param num_include_keys The number of include keys in the path upto the driver key
		\param include_keys An array of include keys
		\param region The region in the shell to select
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void SelectRegion(HShellObject const & oShell, int num_include_keys, HC_KEY include_keys[], int region, bool emit_message);

	
	/*! Adds the geometry identified by 'key' to the selection list and highlights the indicated sub-elements.

		This method selects specific sub-elements of a piece of geometry, usually a shell or mesh.  The first three
		and final arguments are identical to Select().  The remaining four are typically used passed in from 
		HC_Show_Selection_Elements().

		If this method is overloaded, then the matching DeSelect and IsSelected methods should also be overloaded.  		
		
		\param key The key of the geometry with sub-elements to be highlighted.
		\param num_include_keys The number of include keys in the path up to the driver key.
		\param include_keys An array of include keys.
		\param count The number of entries in the faces, vertex1, and vertex2 arrays.  Typically the 
					faces array is shorter than either vertex array.  This is perfectly acceptable.  In this
					case, simply pad the remainder of the array with -1.
		\param faces The face numbers to be highlighted.  Each one of these represents a face in the geometry associated with key.
		\param vertex1 Array of indices into the points array of the geometry specified by key.  Each of these indices
					represents a vertex to be highlighted.
		\param vertex2 Array of indices into the points array of the geometry specified by key.  Each one of these 
					indices represents a point that, when paired with the corresponding point in vertex1 array, 
					represents an edge to be highlighted.		
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void SelectSubentity(HC_KEY key, int num_include_keys, HC_KEY include_keys[], 
		int count, const int faces[]=0, const int vertex1[]=0, const int vertex2[]=0, bool emit_message=false);

	/*! 
		\b Deprecated.  This method has been replaced by the variant that accepts arguments for number of include keys (num_include_keys) and list of include keys (include_keys) 
		Adds the key of an item to the selection list.  
		If the item is geometry, that geometry is moved to a highlight segment (temporary segment with new attributes). 
		If a segment, the segment's attributes are directly modified.

		\param key The key of the item to add.
		\param segpath The full name of the segment. Passed by reference.
		\param include_key The key to the include segment that owns geometry in the scene.
		\param includer_key The key to the segment in which 'include_segment' has been included.
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void Select(HC_KEY key, const char* segpath, HC_KEY include_key, HC_KEY includer_key, bool emit_message = false);

	/*! 
		Removes *all* items identified by 'key' from the selection list and modifies those item's attributes so that they 
		are no longer highlighted.  Note that there can be multiple items identified by 'key' in the selection list,
		but with each item representing a different 'selection instance' of that item.  For example, there might be 4 views
		of a model, and a particular item is selected in 2 of the views.

		If it is desirable to DeSelect a single *particular* instance of an item, then call the DeSelect variant that accepts 
		arguments for number of include keys (num_include_keys) and list of include keys (include_keys) to identified the
		specific view.  
		
		\param key The HOOPS key to the item to be deselected.
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/

	virtual void DeSelect(HC_KEY key, bool emit_message = false);

	/*! 
		Removes an item from a list and modifies that item's attributes so that it is no longer highlighted.  
		
		This variant DeSelects a specific instance of a segment/geometric-primitive (typically, this would
		be desirable if a driver segment/subsegment with 'view' attributes is including a segment in the 
		'include library', and you only want that particular reference of the item to be selected/deselected.  
		In other words, use this method to deselect a given item for one view instead of the all views
		that are using it via the include library.

		This variant should be overloaded if the matching 'Select' method has been overloaded.

		\param key The HOOPS key to the item to be deselected.
		\param num_include_keys The number of include keys in the path upto the driver key
		\param include_keys An array of include keys
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void DeSelect(HC_KEY key, int num_include_keys, HC_KEY include_keys[], bool emit_message = false);

	/*!
		Removes an item from a list and modifies that item's attributes so that it is no longer highlighted.  
	
		This variant DeSelects a specific selection item in the selection list.  Use this variant if you are 
		using a custom selection item or if you have already located the exact item you would like deselected.

		\param new_sel A pointer to a selection item.
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void DeSelect(HSelectionItem * remove_sel, bool emit_message = false);

	/*! 
		Removes *all* items identified by 'key' from the selection list and modifies those item's attributes so that they 
		are no longer highlighted.  Note that there can be multiple items identified by 'key' in the selection list,
		but with each item representing a different 'selection instance' of that item.  For example, there might be 4 views
		of a model, and a particular item is selected in 2 of the views.

		If it is desirable to DeSelect a single *particular* instance of an item, then call the DeSelect variant that accepts 
		arguments for number of include keys (num_include_keys) and list of include keys (include_keys).
		
		\param oShell The HShellObject to which the region belongs.
		\param region The region in the shell to deselect
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void DeSelectRegion(HShellObject const & oShell, int region, bool emit_messag);

	/*! 
		Removes a region of a shell from the selection list and modifies that item's attributes so that 
		it is no longer highlighted.  
		
		This variant DeSelects a specific instance of a segment/geometric-primitive (typically, this would
		be desirable if a driver segment/subsegment with 'view' attributes is including a segment in the 'include library', and
		you only want that particular reference of the item to be selected/deselected).  In other words, use this 
		method to DeSelect a given region for a specific view instead of all the views that used this region via the 
		'include library.'

		This variant should be overloaded if the matching 'Select' method has been overloaded.

		\param oShell The HShellObject to which the region belongs.
		\param num_include_keys The number of include keys in the path upto the driver key
		\param include_keys An array of include keys
		\param region The region in the shell to deselect
		\param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients	
	*/
	virtual void DeSelectRegion(HShellObject const & oShell, int num_include_keys, HC_KEY include_keys[], int region, bool emit_message = false);


	/*! 
		Removes a some or all of a subentity selection of a shell or meshfrom the selection list and modifies
		that item's attributes so that it is no longer highlighted.  It does this via a logical subtraction of 
		the indicated elements from each subentity selection in the selection list.  For discussion of arguments,
		see SelectSubentity().
	*/
	virtual void DeSelectSubentity(HC_KEY key, int num_include_keys, HC_KEY include_keys[], 
		int count, const int faces[], const int vertex1[], const int vertex2[], bool emit_message = false);





	virtual bool SelectSubentityOOC (
		HC_KEY key, int num_include_keys, HC_KEY include_keys[], 
		int count, int const faces[] = 0, int const vertex1[] = 0, int const vertex2[] = 0,
		bool emit_message = false);

	virtual bool DeSelectOOC (
		HC_KEY key, int num_include_keys, HC_KEY include_keys[], 
		bool emit_message = false);




	/*! Returns true if the edges in region selection will be highlighted.  The default is true.  */
	virtual bool HighlightRegionEdges() {return m_bHighlightRegionEdges;};

	/*! Sets whether or not edges in region selection will be highlighted. */
	virtual void HighlightRegionEdges(bool yesno) {m_bHighlightRegionEdges = yesno;};

	/*! Returns true if annotations will be highlighted.  The default is true.  */
	virtual bool HighlightAnnotations() {return m_bHighlightAnnotations;};

	/*! Sets whether or MVO annotations will be highlighted. */
	virtual void HighlightAnnotations(bool yesno) {m_bHighlightAnnotations = yesno;};

	/*! Returns the state of the edges in region selection.  The default is visible.  */
	virtual HRegionEdgeHighlightMode HighlightRegionEdgesAutoVisibility() {return m_bHighlightRegionEdgesAutoVisibility;};

	/*! Sets whether or not edges in region selection will be visible. */
	virtual void HighlightRegionEdgesAutoVisibility(bool yesno) {m_bHighlightRegionEdgesAutoVisibility = yesno ? AllEdges : NoEdges;};

	/*! Sets the state of the edges in region selection. */
	virtual void HighlightRegionEdgesAutoVisibility(HRegionEdgeHighlightMode edge_state) {m_bHighlightRegionEdgesAutoVisibility = edge_state;};

	/*! Sets the state of the edges in region selection. */
	virtual void HighlightRegionFacesAutoVisibility(HRegionFaceHighlightMode face_state) {m_bHighlightRegionFacesAutoVisibility = face_state;};

	/*! Returns the state of the edges in region selection.  The default is visible.  */
	virtual HRegionFaceHighlightMode HighlightRegionFacesAutoVisibility() {return m_bHighlightRegionFacesAutoVisibility;};

	/*! 
		Removes all item from a list and modifies those items so that they are no longer highlighted.
		The base implementation of this method does NOT call the DeSelect method, due to performance concerns.
		If it is necessary for customized DeSelect logic to be executed for each item, then that should be handled in 
		the overloaded implementation of this method.  (The overloaded implementation should loop through the objects
		similar to the base method's implementation, and perform custom DeSelect logic on each item as desired.)
	*/
	virtual void DeSelectAll();

	/*! 
		Determines if an item identified by 'key' is already in the current selection list.

		If it is desirable to check if a *particular* instance of an item is selected, then 
		call the IsSelected variant that accepts arguments for number of include keys 
		(num_include_keys) and list of include keys (include_keys)

		\param key The HOOPS key to the item to be checked.
		\return True if the item is already selected, false if not.
	*/
	virtual bool IsSelected(HC_KEY key) const;

	/*! 
		Determines if a specific selection instance of an item identified by 'key' is already in the current 
		selection list.   

		This variant determines if a specific instance of a segment/geometric-primitive is selected 
		(typically, this would be desirable if a driver segment/subsegment with 'view' attributes 
		is including a segment in the 'include library', and you only want that particular reference 
		of the item to be selected/deselected (so that the item will highlight/dehighlight 
		in 1 view rather than all views).

		This variant should be overloaded if the matching Select and DeSelect methods have been overloaded.

		\param key The HOOPS key to the item to be checked.
		\param num_include_keys The number of include keys in the path upto the driver key.
		\param include_keys An array of include keys.
		\return True if the item is already selected, false if not.
	*/
	virtual bool IsSelected(HC_KEY key, int num_include_keys, const HC_KEY include_keys[]) const;

	/*! 
		Determines if a specific selection item is already in the current selection list.

		This variant determines if the exact HSelectionItem (or derived class) is already in the 
		selection list.  It does not examine sub-components to determine if an equivalent item is selected.  

		This variant should be overloaded if the matching Select and DeSelect methods have been overloaded.

		\param item The HSelectionItem (or derived class) to be searched for.
		\return True if the item is already selected, false if not.
	*/	
	virtual bool IsSelected(const HSelectionItem * item) const;

	/*! 
		Determines if a specific selection instance of a region identified by 'key' and 'region' is 
		already in the current selection list.   

		This variant determines if a specific instance of a segment/geometric-primitive is selected 
		(typically, this would be desirable if a driver segment/subsegment with 'view' 
		attributes is including a segment in the 'include library', and you only want that 
		particular reference of the item to be selected/deselected 
		(so that the item will highlight/dehighlight in 1 view rather than all views).

		This variant should be overloaded if the matching Select and DeSelect methods have been overloaded.

		\param key The HOOPS *shell* key to the item to be checked.
		\param num_include_keys The number of include keys in the path upto the driver key
		\param include_keys An array of include keys
		\param region the region number we're checking for
		\return True if the item is already selected, false if not.
	*/
	virtual bool IsRegionSelected(HC_KEY key, int num_include_keys, const HC_KEY include_keys[], int region) const;

	/*! 
		Determines if the segment key passed is the temporary highlight segment. 
		\param segment_key The HOOPS segment key to be checked.
		\return true if the segment key is a temporary highlight segment
	*/
	virtual bool IsHighlightSegment(HC_KEY segment_key) const;

	/*! 
		Sets the color to use for selected faces
		\param rgba The color used to highlight selected faces.
	*/
	virtual void SetSelectionFaceColor(HPixelRGBA &rgba){m_selectedFaceColor = rgba; UpdateHighlightStyle();}

	/*! 
		Use this method to cause the scene to be drawn in grayscale.
		\param grayScale Pass if true if you wish the scene to be drawn in grayscale and false if otherwise.
	*/
	virtual void SetGrayScale(bool grayScale);


	/*! 
		Use this method to cause the highlighted item to be invisible.
		\param Invisible Pass if true if you wish the items to beinvisible and false if otherwise.
	*/
	virtual void SetInvisible(bool Invisible);

	/*!Use this method to find out if highlighted items will be invisible.
	\return True if HOOPS will hide selected items.
	*/
	virtual bool GetInvisible() { return m_bForceInvisible; }
	
 
	/*!
	Use this method to allow or disallow HOOPS to draw selected segments in front of coincident geometry.
	*/
	virtual void SetAllowDisplacement(bool allowDisplacement) { m_bAllowDisplacement = allowDisplacement; }

	/*!Use this method to find out if HOOPS will push back coincident geometry and draw selected items in front.
	\return True if HOOPS will draw selected segments in front of coincident geometry.
	*/
	virtual bool GetAllowDisplacement() { return m_bAllowDisplacement; }
	
	/*! Returns the current selection list pointer.  This function is deprecated and should not be used.
		To retrieve members from the selection list, use GetAt() instead.
		\return A HSelList pointer to current selection list.
	*/
	virtual HSelList * GetSelectionList();

	/*! 
		Returns the current color for selected faces
		\param rgba This method populates this parameter with the color used to highlight selected faces. 
	*/
	virtual void GetSelectionFaceColor(HPixelRGBA &rgba){ rgba = m_selectedFaceColor; };


	/*! 
		Sets the color to use for selected edges
		\param rgb The color used to highlight selected edges.
	*/
	virtual void SetSelectionEdgeColor(HPixelRGBA &rgb){m_selectedEdgeColor = rgb; UpdateHighlightStyle();}

	/*! 
		Returns the current color for selected Edges.
		\param rgba This method populates this parameter with the color used to highlight selected edges. 
	*/
	virtual void GetSelectionEdgeColor(HPixelRGBA &rgba){ rgba = m_selectedEdgeColor; };


	/*! 
		Sets the color to use for selected cutting edges.
		\param rgb The color used to highlight selected cutting edges.
	*/
	virtual void SetSelectionCutEdgeColor(HPixelRGBA &rgb){m_selectedCutEdgeColor = rgb; UpdateHighlightStyle();}

	/*! 
		Returns the current color for selected cutting edges.
		\param rgba This method populates this parameter with the color used to highlight selected cutting edges.
	*/
	virtual void GetSelectionCutEdgeColor(HPixelRGBA &rgba){ rgba = m_selectedCutEdgeColor; };

	/*! 
		Sets the color to use for selected cutting faces.
		\param rgb The highlight color for selected cutting faces.
	*/
	virtual void SetSelectionCutFaceColor(HPixelRGBA &rgb){m_selectedCutFaceColor = rgb; UpdateHighlightStyle();}

	/*! 
		Returns the current color for selected cutting faces.
		\param rgba This method populates this parameter with the color used to highlight selected cutting faces. 
	*/
	virtual void GetSelectionCutFaceColor(HPixelRGBA &rgba){ rgba = m_selectedCutFaceColor; };

	/*! 
		Sets the color to use for selected markers.
		\param rgb The highlight color you want to use for selected markers. 
	*/
	virtual void SetSelectionMarkerColor(HPixelRGBA &rgb){m_selectedMarkerColor = rgb; UpdateHighlightStyle();}


	/*! 
		Returns the current color for selected Markers.
		\param rgba The method populates this parameter with the highlight color for selected markers.
	*/
	virtual void GetSelectionMarkerColor(HPixelRGBA &rgba){ rgba = m_selectedMarkerColor; };

	/*! 
		Sets the weight to use for highlighting selected edges.
		\param weight The edge weight used for highlighting.
	*/
	virtual void SetSelectionEdgeWeight(float weight){m_fselectedEdgeWeight= weight; UpdateHighlightStyle();}


	/*! 
		Returns the current edge weight used to highlight selected edges.
		\return Selection edge weight. 
	*/
	virtual float GetSelectionEdgeWeight(){return m_fselectedEdgeWeight;}


	/*! 
		Sets the size to use for selected markers.
		\param size Size of the current marker used for highlighting
	*/
	virtual void SetSelectionMarkerSize(float size){m_fselectedMarkerSize = size; UpdateHighlightStyle();}

	/*! 
		Returns the current marker size used to highlight selected markers.
		\return Selection marker size
	*/
	virtual float GetSelectionMarkerSize(){return m_fselectedMarkerSize;}

	/*! Returns the value of a given list item.  Note: if it is important that 
		the list order be the same as the selection order, call SetSortSelectionList first.
		\param index The index into the list.
		\return The item at a given offset within the list. Returns INVALID_KEY if no item is found.
	*/
	virtual HC_KEY GetAt(unsigned int index);

	/*! Fills the keys of the selection list in to an array.  Note: if it is important that
	the list order be the same as the selection order, call SetSortSelectionList first.
	\param out_array A key array that is pre-sized to receive all the selection keys.
	\return This returns true if it succeeds or false if it fails.
	*/
	virtual bool GetAll(HC_KEY out_array[]);


	/*! Returns the selection item object pointer at the given offset.
	\param index The index into the list.
	\return The item at a given offset within the list. Returns 0, if no item is found.
	*/
	const HSelectionItem * GetSelectionItemAt(unsigned int index);


	/*! Returns the selection item object pointer with the given key.
	\param key The key of the selection item you are interested in.
	\return The item with the given key within the list. Returns 0, if no item is found.
	*/
	const HSelectionItem * GetSelectionItem(HC_KEY key);


	/*! \return The length of the current selection list. */
	unsigned int  GetSize() const;

	/*!
		Resets the selection list length to zero.
	*/
	virtual void Reset();


	/*! Deletes all the items currently selected.
	  \param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
								would typically use the HOOPS/Net 'Send' function to dispatch the message to clients.
		\return True if the currently selected items were successfully deleted.
	*/
	virtual	bool DeleteSelection(bool emit_message=false);

	/*! Returns the key to style segment which is being used to highlight the selection items.
		\return HC_KEY to highlight style segment.
	*/
	HC_KEY GetHighlightStyle() const{ return m_keyHighlightStyle; };


	void SetHighlightStyle(HC_KEY key) { m_keyHighlightStyle = key; };

	const char * GetHighlightStyleName() const{ return m_keyHighlightStyleName; };
	
	/*! If the selected item is a shared segment or entity, this method will make a copy of it so that 
		it is not shared anymore. Use this method when you want to physically modify the selection 
		(delete, transform, color) and, at the same time, <i>only</i> affect the selected entity (even if 
		it is shared). Once the method is finished, the sel_item will be the selection item itself 
		which with the newly changed data. Using GetKey function, you can access the new key to 
		de-instanced segment/entity. The function will copy every segment that is include from 
		top (except for the topmost which is assumed to be model segment included by view). <br>
		NOTE: <br>
		1. This will change the segment hierarchy of the model and hence will update any other 
		selection made. <br>
		2. The selection item has to be a part of current selection list to be able to de-instance.
		\param sel_item The HSelectionItem item you wish to deinstance.
		\return True if successful and false if otherwise.
	*/
	bool DeInstance(HSelectionItem * sel_item);

	
	/*! Deinstances the item located at index in the selection list.
		\param index the index in the selection list of the item you wish to deinstance.
		\return True if successful and false if otherwise.
	*/
	bool DeInstance(unsigned int index);

	/*! \return A boolean indicating whether the HSelectionSet allows Selection 
	through Subwindows. Note that this behavior is undefined in conjunction with 
	reference selection.  Default setting is false.
	*/
	virtual bool GetSubwindowPenetration() { return m_bSubwindowPenetration; }

	/*!
	Sets the entity selection mode for the HSelectionSet. 
	\param allow If true, then Selection through Subwindows is enabled.
	Otherwise, subwindows block the selection from walking the tree below them. 
	Note that this behavior is undefined in conjunction with reference selection. 
	Default setting is false.
	*/
	virtual void SetSubwindowPenetration(bool allow);

	/*! \return A boolean indicating whether the HSelectionSet allows Subentity Selection.	*/
	virtual bool GetAllowSubentitySelection() { return m_selection_level == HSelectSubentity; }

	/*!
		Sets the subentity selection mode for the HSelectionSet. 
		\param allow If true, then Subentity Selection is enabled.
	*/
	virtual void SetAllowSubentitySelection(bool allow);

	/*! \return An enumeration indicating whether the HSelectionSet allows segment, entity, region, or subentity selection.	*/
	virtual HSelectLevel GetSelectionLevel() const { return m_selection_level; }

	/*! \param level An enumeration indicating whether the HSelectionSet should allow
	segment, entity, region, or subentity selection.	*/
	virtual void SetSelectionLevel(HSelectLevel level);

	/*! \return A boolean indicating whether the HSelectionSet allows Entity Selection.	*/
	virtual bool GetAllowEntitySelection() { return m_selection_level == HSelectEntity; }


	/*!
		Sets the entity selection mode for the HSelectionSet. 
		\param allow If true, then Entity Selection is enabled.
	*/
	virtual void SetAllowEntitySelection(bool allow);

	/*! \return A boolean indicating whether the HSelectionSet allows Region Selection.	*/
	virtual bool GetAllowRegionSelection() const { return m_selection_level == HSelectRegion; }

	/*!
		Call this method to enable or disable region selection mode for the HSelectionSet. 
		\param allow If true, then Region Selection is allowed.
	*/
	virtual void SetAllowRegionSelection(bool allow);

	/*! 
		Used by HSelectionItem to add a segment to the list when it has been highlighted.
		\param segment_key The HOOPS segment key to be registered.
	*/
	void RegisterHighlightSeg(HC_KEY seg_key);
	
	/*! 
		Used by HSelectionItem removed segment from the list once it has been unhighlighted.
		\param segment_key The HOOPS segment key to be unregistered.
	*/
	void UnRegisterHighlightSeg(HC_KEY seg_key);

	/*! 
		\return The selection highlight mode.
	*/
	virtual const HSelectionHighlightMode GetHighlightMode() { return m_HighlightMode; }

	/*!
		This function determines the behavior when an object is selected.  
		
		Setting the mode to HighlightDefault is the classical HOOPS behavior whereby highlighting 
		is achieved by using a conditional include to modify the color of the actual geometry.  This
		is very robust and time-tested, but has the disadvantage of modifying the actual segment structure.  
		As a result, a full update is typically required and, if the static model is being used, it will 
		need to be regenerated.
		
		HighlightQuickmoves, on the other hand, uses quick moves to draw another copy of the selected 
		geometry over the top of the existing one.  This has two benefits.  It does not require a full
		update, improving behavior on large models.  Further, because it does not modify the segment
		structure, static models do not need to be regenerated.

		InverseTransparency uses the same mechanics as HighlightDefault -- and inherits all its 
		benefits and drawbacks -- to achieve a very different effect.  When an object is selected with this 
		highlight mode, the selected object does not change.  Instead the rest of the model becomes 
		transparent (use SetHighlightTransparency() to set how transparent).  Use of this mode has the largest
		performance implications.  Not only does it modify the segment structure, but it also creates a 
		potentially large amount of transparent geometry which can be computationally intensive to render.

		ColoredInverseTransparency is identical to InverseTransparency highlighting detailed above except 
		the selected object(s) are highlighted in the highlight color.

		\param m the new highlight mode (see the SelectionHighlightMode enumeration for possible values)
	*/
	virtual void SetHighlightMode(HSelectionHighlightMode m);

	/*!
		Sets the transparency level of unselected geometry in inverse transparency selection mode.
	*/
	virtual void SetHighlightTransparency(float trans);

	/*!
		Returns the key to the selection segment
		\return key to selection segment.
	*/
	virtual HC_KEY GetSelectionSegment() {return m_SelectionSegment;}

	/*!
	   Unhighlights all selection items, toggles the highlight style between quickmoves and conditional
	   highlighting, and re-highlights them all in the new style.  This method is deprecated.  It has no 
	   effect if the highlight mode is not either quickmoves or conditional.
   */
   void ToggleHighlightStyle();

	/*!
		Returns true if the key is the selection segment or a child of it
		\param key key of the object to be queried.
	*/
	virtual bool GetIsSelection(HC_KEY key);

	/*!
		Returns true if either inverse transparency or quickmoves highlighting 
		are being used.reference selection is currently being used.
		\return the current reference selection state.
	*/
	virtual bool GetReferenceSelection() const {return m_HighlightMode!=HighlightDefault 
												|| m_selection_level == HSelectSubentity;}

	/*!
		This function is deprecated.  The correct way to do this is to call
		SetHighlightMode(HighlightQuickmoves) or SetHighlightMode(HighlightDefault).
		\param ref_sel the new reference selection state.
	*/
	virtual void SetReferenceSelection(bool ref_sel) {
		m_HighlightMode = ref_sel ? HighlightQuickmoves : HighlightDefault;
	}

	/*!
		If this is set then HighlightQuickmoves will use 3dgs built in defined highlighting rather than MVO highlighting code.
		\param use_defined_highlight the new use defined highlight state.
	*/
	void SetUseDefinedHighlight(bool use_defined_highlight);

	/*!
		Returns true if 3dgs built in defined highlighting is going to be used for HighlightQuickmoves.
		\return the current use defined highlight state.
	*/
	bool GetUseDefinedHighlight() {
		return m_bUseDefinedHighlight;
	}
	

	/*!
		Overwrites the default quick moves type (spriting) with type
		\param type the new reference selection type.
	*/
	virtual void SetReferenceSelectionType(HRefSelType type);
	virtual HRefSelType GetReferenceSelectionType() {return m_quickmoves_type;}

	/*!
		Updates all the attributes of the highlight style segment from the members
	*/
	virtual void UpdateHighlightStyle();

	/*!
		Gets a pointer to the PShow cache hash table 
		used for improving performance of area selection.
		\return PShow cache hash pointer
	*/	
	HVHashPShowCache * GetPShowCache(){ return m_pshow_cache; }

	/*!
		Replaces the current PShow cache (if any) with a new one.
		If one already exists, FlushPShowCache() should be called before replacing it.
		\param new_cache the new cache hash table to use
	*/	
	void SetPShowCache(HVHashPShowCache * new_cache){ m_pshow_cache=new_cache; }

	/*!
		Discards the current PShow cache hash and frees all the elements.
	*/	
	void FlushPShowCache();

	/*!
		Updates all reference highlighting.  This can be slow with large selection sets.
	*/
	void UpdateHighlighting();

	HBaseView * GetView() { return m_pView; }

	/*!
	If set, the selection list will be sorted before it is returned via GetSelectionList()
	*/
	void SetSortSelectionList(bool sort)  { m_list_last_updated=0; m_bSortSelectionList=sort;}

	/*!
	If set, the selection list will be sorted before it is returned via GetSelectionList()
	*/
	bool GetSortSelectionList()  {return m_bSortSelectionList;}

	/*!
	Will faces be highlighted if subentity selection is enabled?
	*/
	virtual bool GetSubentityFaceSelection() { return m_subentity_face; }

	/*!
	Set whether faces be highlighted if subentity selection is enabled.
	*/
	virtual void SetSubentityFaceSelection(bool select) { m_subentity_face=select; }

	/*!
	Will edges be highlighted if subentity selection is enabled?
	*/
	virtual bool GetSubentityEdgeSelection() { return m_subentity_edge; }
	
	/*!
	Set whether edges be highlighted if subentity selection is enabled.
	*/
	virtual void SetSubentityEdgeSelection(bool select) { m_subentity_edge=select; }

	/*!
	Will vertices be highlighted if subentity selection is enabled?
	*/
	virtual bool GetSubentityVertexSelection() { return m_subentity_vertex; }
	
	/*!
	Set whether vertices be highlighted if subentity selection is enabled.
	*/
	virtual void SetSubentityVertexSelection(bool select) { m_subentity_vertex=select; }

	/*!
		Alias for SetSubentityTracking().
	*/
	virtual void SetAllowSubentityDeselection(bool allow) { SetSubentityTracking(allow); }

	/*!
		Alias for GetSubentityTracking().
	*/
	virtual bool GetAllowSubentityDeselection() { return GetSubentityTracking(); }

	/*!
		This method controls whether subentity selections can be deleted or partially deselected.  By default this
		is set to true which means selection information for the source geometry will be retained.  If this is set to false, 
		no deletion can occur and subentity selections can only be de-selected as a whole.  Doing so does, however, 
		reduce memory requirements which can be potentially large, depending on the size of the source geometry and 
		the number of subentity selections.
	*/
	virtual void SetSubentityTracking(bool allow) { m_subentity_maintain_maps = allow; }

	/*!
		Returns whether subentity selections can be deleted or partially deselected.  See SetSubentityTracking() for details.
	*/
	virtual bool GetSubentityTracking() { return m_subentity_maintain_maps; }

	/*!
	Retrieves a bounding box for the currently selected geometry in world space.
	\param minimum the minimum values for the x, y, and z components of the bounding box.
	\param maximum the maximum values for the x, y, and z components of the bounding box.
	*/
	void GetSelectionBounding(HPoint * minimum, HPoint * maximum) const;

	
	/*!
		\param allow If true, then Select() will notify is enabled.
	*/
	virtual void SetSelectWillNotify(bool notify) {m_bNotify = notify;}

	/*! \return A boolean indicating whether Select() will notify. */
	virtual bool GetSelectWillNotify() const { return m_bNotify; }


	// internal use only
	int GetAttrLookupID(const char * attribute_string);
	HSelectionSegmentConditionInfo *	GetSelectionSegmentConditionInfo();
	HConditionStyleKeyInfoHash *		GetConditionStyleKeyInfo();

	
private:
	void HoopsRemove( HC_KEY key );
	void DeselectSegment( HC_KEY key, char const *keytype );
	void DeselectGeometry( HC_KEY key );

	/*!
		common implementation function for overloaded IsSelected(HC_KEY) variants
	*/
	inline bool is_selected(HC_KEY key) const;

	void ensure_is_segment(HC_KEY &key) const;


	int m_AttrID;
	struct vhash_s	*m_pAttrLookupHash;	

	HSelectionSegmentConditionInfo * m_pSelectionSegmentConditionInfo;
	HConditionStyleKeyInfoHash * m_pConditionStyleKeyInfoHash;


	HVHashPShowCache	* m_pshow_cache;
	HSelHash			* m_selected_hash;
	float				m_hash_last_updated;
	float				m_list_last_updated;
	HRefSelType			m_quickmoves_type;

	HC_KEY	m_keyHighlightStyle;
	char	m_keyHighlightStyleName[32];

	HSelectLevel	m_selection_level;

	bool			m_subentity_face;
	bool			m_subentity_edge;
	bool			m_subentity_vertex;
	bool			m_subentity_maintain_maps;

	bool	m_bSubwindowPenetration;

	bool			m_currently_in_DeleteSelection;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



