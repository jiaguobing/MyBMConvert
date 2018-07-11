// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HSelectionItem.h 
	Interface of the HSelectionItem class
	Wrapper over selected items (mostly keys) held by HSelectionSet class
	*/


#ifndef _HSELECTIONITEM_H
#define _HSELECTIONITEM_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HUtility.h"

class HPixelRGBA;
class HSelectionSet;
class HBaseView;


//! The HSelectionItem class is an interface definition for a selection item
/*!
  This is the abstract base class for selection items held in a list the HSelectionSet class.  It defines
  a set of interfaces for the HSelectionSet and operators like HOpSelectAperture to interact with selection
  items.  Use this class as a base class when you want to implement new types of selection item classes.  Note
  that you cannot create an instance of this class.  It contains pure virtual functions which have no 
  implementation.
*/
class MVO_API HSelectionItem 
{

public:
	/*! Constructs. */
	HSelectionItem(HC_KEY key, HSelectionSet * selection_set);

	virtual ~HSelectionItem();

	/*! \return A character pointer denoting the name which is "HSelectionItem."
	*/
	virtual const char * GetName() const { return "HSelectionItem"; };


	/*! \return The HOOPS key to the selection item.
	 */
	virtual HC_KEY GetKey() const { return m_key; };


	/*! \return If the selection item is a segment, then the item key is returend. If the selection item is entity, then 
		key to the owner segment is returned.
	 */
	virtual HC_KEY GetSegmentKey() const;


	/*! \return True when the two selection elements are equal.
		\param i2 An HSelectionItem object to be compared with this object.
	*/
	virtual bool Equals(const HSelectionItem * i2) const;


	/*! This function highlights the selection item.  
		\return True if the selection item was succesffuly highlighted.
	*/
	virtual bool Highlight() = 0;

	/*! This method unhighlights the selection item.
		\return A true if the unhighlight is succesful
	*/
	virtual bool UnHighlight() = 0;

	/*! 
		Checks to see if an item is currently highlighted.
		\return True if the item is currently highlighted, false if the item is not highlighted.
	*/
	virtual bool IsHighlighted() const = 0;


	/*! Call this function to delete the selection item.
		NOTE: If the selection is a non-instanced segment or entity, it will be deleted. 
			  If the selection is an instanced segment or entity, it's include link will be removed.
		\return True if the item was successfully deleted.
		*/
	virtual bool Delete() = 0;


	/*! 
		Checks to see if the selection item has any segments/geometry which might be 
		potentially shared. This is determined by checking the include keys in the 
		selection path. If there is more than one (the model key), then it is instanced. 
		\return True if the item is instanced.
	*/
	virtual bool IsInstanced() const = 0;


	/*! \return The number of include keys in the include path.
	 */
	virtual int GetIncludeCount() const = 0;

	/*! \return An array to include keys. The return pointer is for read-only purposes.
	 */
	virtual const HC_KEY * const GetIncludeKeys() const = 0;

	/*! \param includes An array of include keys (returned to the user).  Note that the caller 
	must ensure that the array is of the appropriate size, e.g, via the GetIncludeCount function.
	*/
	virtual void GetIncludeKeys(HC_KEY includes[]) const = 0;

	/*! 
		Determines if the selection item has any dependency on the include key being provide.
		If it does have a dependency, this method returns true and returns which skey is dependent as the second argument.
		\param include_key HC_KEY to include key on which the dependency is wanted
		\param ret_dependent_key HC_KEY to segment/entity which depends on the include key
		\return True if the item has dependency on the given include_key.
	*/
	virtual bool QueryIncludeDependency(HC_KEY include_key, HC_KEY & ret_dependent_key) = 0;


	/*! 
		This method updates the given key's dependency on a given include key with a new
		include key.
		\param include_key HC_KEY of the include key which is deleted/going to be deleted. 
		\param dependent_key HC_KEY of the segment/entity which depends on the include key.
		\param replacement_key The new HC_KEY that will replace include_key for the given dependent_key.
		\return True if the dependency has been successfully updated.
	*/
	virtual bool UpdateIncludeDependency(HC_KEY include_key, HC_KEY dependent_key, HC_KEY replacement_key) = 0;

	/*! \return A count of number of include keys in the include path plus 2 to account for the 
				the selection item and view key.
	 */
	virtual int GetFullPathCount(HBaseView * view);

	/*! \return An array of keys, suitable for passing to HOOPS' HC_PShow* or 
				HC_Compute*By_Path functions
		\param view the HBaseView in which this item is selected
	 */
	virtual const HC_KEY *GetFullPathKeys (HBaseView * view);

	/*!	\param view The HBaseView in which the item is selected
		\param path An array of keys (returned to the user), suitable for passing to the PShow or By_Path
			   functions.  Note that the caller must ensure that the array is of the appropriate size, e.g.,
			   via the GetFullPathCount function.
	 */
	virtual void GetFullPathKeys (HBaseView * view, HC_KEY path[]);

	/*!
		Sets a selection item's time stamp
		\param now the time stamp
	*/
	void SetTimeStamp(float now) {m_TimeStamp=now;}

	/*!
		Retrieves a selection item's time stamp
		\return the time stamp
	*/
	float GetTimeStamp() {return m_TimeStamp;}			

protected:

	HC_KEY m_key;						//!< The HOOPS key to the selection element.
	HSelectionSet * m_pSelectionSet;	//!< The selection set which owns this selection item.

	HC_KEY * m_FullPath;				//!< The include path plus the selection item and view key.  If the object is not a HSmartSelItem, the path may be incomplete.
	int m_FullPathCount;				//!< The length of the m_FullPath array of keys including the selection item and view key.

	float m_TimeStamp;					//!< Time the item was created. Used for sorting the selection list.
};



//! The HSmartSelItem - selection item which can handle instancing.
/*!
  This implementation of HSelectionItem class handles instanced segments and entities. 
  It also uses the 'conditional styling' mechanism to highlight the selection.
*/

class MVO_API HSmartSelItem : public HSelectionItem
{
	friend class HSelectionSet;

public:

	/*! 
	Constructs a HSmartSelItem object.
	\param key HOOPS key to selection item.
	\param selection_set pointer to the HOOPS selection set which owns this selection item.
	\param incl_count Number of include keys in the selection item segment path.
	\param incl_keys An array of HOOPS keys of all the include keys in the selection item path.
	 */
	HSmartSelItem(HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY incl_keys[]);

	/*! 
	Constructs a HSmartSelItem object.
	\param key HOOPS key to selection item.
	\param selection_set pointer to the HOOPS selection set which owns this selection item.
	\param incl_count Number of include keys in the selection item segment path.
	\param incl_keys An array of HOOPS keys of all the include keys in the selection item path.
	\param is_segment Is the item known to be a segment.
	 */
	HSmartSelItem(HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY incl_keys[], bool is_segment);

	virtual ~HSmartSelItem();

	/*! \return A character pointer denoting the name which is "HSmartSelItem."
	*/
	virtual const char * GetName() const { return "HSmartSelItem"; };


	/*! \return The number of include keys in the include path.
	 */
	virtual int GetIncludeCount() const { return m_IncludePath.count; };

	/*! \return An array to include keys. The return pointer is for read-only purpose.
	 */
	virtual const HC_KEY * const GetIncludeKeys() const { return m_IncludePath.keys; };

	/*! \param includes An array of include keys (returned to the user).  Note that the caller 
			   must ensure that the array is of the appropriate size, e.g, via the GetIncludeCount function.
	 */
	virtual void GetIncludeKeys(HC_KEY includes[]) const;

	/*! \return An array to include keys, just like GetIncludeKeys, except that the keys array includes
				the selection item and the view key (as passed in).
		\param view The HBaseView in which this item is selected.
	 */
	virtual const HC_KEY *GetFullPathKeys (HBaseView *view);

	/*!	\param view The HBaseView in which the item is selected
		\param path An array of keys (returned to the user), just like GetIncludeKeys, except that the 
			   keys array includes the selection item and the view key (as passed in).  Note that the 
			   caller must ensure that the array is of the appropriate size, e.g.,
			   via the GetFullPathCount function.
	 */
	virtual void GetFullPathKeys (HBaseView *view, HC_KEY path[]);

	/*! \return True if the two selection elements are equal.
		\param i2 An HSelectionItem object to be compared with this object
	*/
	virtual bool Equals(const HSelectionItem* i2) const;


	/*! Call this function to highlight the selection item.
		\return True if the highlight is successful.
	*/
	virtual bool Highlight();

	/*! Call this function to unhighlight the selection item.
		\return True if the unhighlight is successful.
	*/
	virtual bool UnHighlight();

	/*! 
		Checks to see if an item is currently highlighted.
		\return True if the item is currently highlighted, false if otherwise.
	*/
	virtual bool IsHighlighted() const;


	/*! 
		Checks to see if an item is currently highlighted. If you do not have 
		the HSmartSelItem object, but just have the keys and include list returned 
		from HOOPS, you could use this method to find out if the key was selected.
		\param key HOOPS key to the item to be tested for selection.
		\param selection_set A pointer to the HOOPS selection set in which this is to be tested.
		\param incl_count Number of include keys in the item segment path.
		\param incl_keys An array of HOOPS keys of all the include keys in the item path.
		\return True if the item is currently highlighted, false if otherwise.
	*/
	static bool IsHighlighted(const HC_KEY key, const HSelectionSet * selection_set, 
							  const int incl_count, const HC_KEY incl_keys[]);
	/*! 
		Checks to see if a region is currently highlighted. If you do not have 
		the HSmartSelItem object, but just have the keys and include list returned 
		from HOOPS, you could use this method to find out if the key was selected
		\param key HOOPS *shell* key to the item to be tested for selection.
		\param selection_set A pointer to the HOOPS selection set in which this is to be tested.
		\param incl_count Number of include keys in the item segment path.
		\param incl_keys An array of HOOPS keys of all the include keys in the item path.
		\param region The region number in the shell.
		\return True if the item is currently highlighted, false if otherwise.
	*/
	static bool IsRegionHighlighted(const HC_KEY key, const HSelectionSet * selection_set, 
							  const int incl_count, const HC_KEY incl_keys[], int region);


	/*! Call this function to delete the selection item.
		NOTE: If the selection is a non-instanced segment or entity, it will be deleted. 
			  If the selection is an instanced segment or entity, it's include link will be removed.
		\return True if the item was successfully deleted.
	*/
	virtual bool Delete();


	/*! 
		Checks to see if the selection item has any segments/geometry which might be 
		potentially shared. This is determined by checking the include keys in the 
		selection path. If there is more than one (the model key), then it is instanced. 
		\return True if the item is instanced.
	*/
	virtual bool IsInstanced() const;


	/*! 
		Determines if the selection item has any dependency on the include key being provide.
		If it has, the api returns true and returns which key is dependent as the second argument.
		\param include_key HC_KEY to include key on which the dependency is wanted.
		\param ret_dependent_key HC_KEY to segment/entity which depends on the include key.
		\return True if the item has dependency on the include_key.
	*/
	virtual bool QueryIncludeDependency(HC_KEY include_key, HC_KEY& ret_dependent_key);


	/*! 
		This method updates the given key's dependency on a given include key with a new
		include key.
		\param include_key HC_KEY to include key which is deleted/going to be deleted 
		\param dependent_key HC_KEY to segment/entity which depends on the include key
		\param replacement_key new HC_KEY as a replacement to the dependent key
		\return True if the dependency has been successfully updated.
	*/
	virtual bool UpdateIncludeDependency(HC_KEY include_key, HC_KEY dependent_key, HC_KEY replacement_key);

	/*!
		Replace the original include path and count passed into the constructor.
		\param incl_count The new number of keys in the include path
		\param incl_keys  An array of include keys
	*/
	void UpdateIncludePath(int incl_count, HC_KEY incl_keys[]);

	/*! 
		\return The selection highlight mode.
	*/
	virtual const int GetHighlightMode() { return m_HighlightMode; }

	/*!
		Sets the selection highlight mode.
		\param m What the new highlight mode should be (see the SelectionHighlightMode enumeration for possible values)
	*/
	virtual void SetHighlightMode(HSelectionHighlightMode m) { m_HighlightMode = m;}

protected:
	HSmartSelItem(bool is_ooc, HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY incl_keys[], bool is_segment);

	void CommonUnhighlightRef();
private:

	HC_KEY		m_ConditionStyle;		//!< The HOOPS conditional style key applied to selected item. Required to unhighlight.

	HSelectionHighlightMode m_HighlightMode; //!< Stores the current highlighting mode

public:

	/*!
		Retrieves the size of the list of highlighted objects
		\return size of highlighted list
	*/
	int GetHighlightedRefListCount();

	/*!
		Copies the list of Geometry Reference keys from this selection item.
		\param array of Reference Geometry keys.  Returned to user.  Passed by reference always.
	*/
	void GetHighlightedRefList(HC_KEY * keys);

	bool IsSegment() {return m_is_segment;}

protected:	
	/*!
		Ensure the include path is correct for the type of highlighting performed
	*/
	void FixIncludePath();

	/*!
		Dequeues a Reference Geometry key from the list of highlighted objects
		\return the reference geometry key of a highlighted object
	*/
	HC_KEY GetHighlightedRefKey();
	
	/*!
		Adds a Reference Geometry key to the list of highlighted objects
		\param the reference geometry key of a highlighted object
	*/
	void AddHighlightedRefKey(HC_KEY key);

	struct vlist_s	*m_pSelKeyList;

private:
	void GetBaseTag(char * retTag);

private:

	// The include path is a list of all the include keys from the root segment right down to 
	// entity/segment. This can be constructed from the selection path returned by HOOPS by filtering 
	// out any keys other than include keys. 
	struct {
		int count;
		HC_KEY * keys;
	} m_IncludePath;

	bool m_is_segment;

	// Selection of a non-segment item with conditional highlighting will throw the item into a proxy
	// segment so the item can be styled. However, if different selection set selects the same item,
	// it should *not* move the item to a further proxy subsegment because this can cause cleanup
	// problems. Now we cannot clean up after ourselves if another selection set has selected that
	// item, even if we want to clean up after our own selection set. So an in-use tag needs to
	// be funneled in as well to allow us to see if others are using the proxy segment as well.
	// The could probably be done with a simple counter attached to the segment as a user option or
	// the like, but I suspect that a multithreaded environment could make this quickly go awry.
	HC_KEY m_entity_proxy_seg_tag;
};

//! The HRegionSelItem - region selection item which can handle instancing
/*!
  This implementation of HSmartSelItem class handles instanced regions. 
  It also uses the 'conditional styling' mechanism to highlight the selection
*/

class MVO_API HRegionSelItem : public HSmartSelItem
{
public:
		
	/*! 
	Constructs a HRegionSelItem object.
	\param key HOOPS key to selection item.
	\param selection_set A pointer to the HOOPS selection set which owns this selection item.
	\param incl_count Number of include keys in the selection item segment path.
	\param incl_keys An array of HOOPS keys of all the include keys in the selection item path.
		*/
	HRegionSelItem(HShellObject const & oShellObj, HSelectionSet * selection_set, int incl_count, HC_KEY incl_keys[], int region);

	~HRegionSelItem(){

	}

	/*! \return A character pointer denoting the name which is "HRegionSelItem." 
	*/
	virtual const char * GetName() const { return "HRegionSelItem"; };

	/*! \return The shell region which is selected.
	*/
	virtual int const GetRegion() const { return m_Region; }

	/*! \return True if the two region selection elements are equal.
		\param i2 An HRegionSelItem object to be compared with this object.
	*/
	virtual bool Equals(const HSelectionItem* i2) const;

	/*! Call this function to highlight the selection region.
		\return A true if the item was successfully highlighted.
	*/
	virtual bool Highlight();
	
	/*! Call this function to unhighlight the selection item.
		\return A true if the item was successfully unhighlighted.
	*/
	virtual bool UnHighlight();
	
private:
	/*!  The shell region which is selected.
	*/
	int		m_Region;

	/*! Call this to increase the edge highlight count. This is a helper function for the HighlightEdges method.
		\return Rhe number of times the item has been highlighted.
	*/
	virtual int IncrEdgeHighlightCount(HC_KEY key, HShellEdge const & Edge);
	
	/*! Call this to decrease the edge highlight count. This is a helper function for the UnHighlightEdges method.
		\return The number of times the item is highlighted.
	*/
	virtual int DecrEdgeHighlightCount(HC_KEY key, HShellEdge const & Edge);

	/*! Call this function to highlight the selection region's edges.  This is a helper function for Highlight.
	*/
	virtual void HighlightEdges();

	/*! Call this function to highlight the selection region's edges.  This is a helper function for UnHighlight.
	*/
	virtual void UnHighlightEdges();

	/*! Call this function to highlight the selection region's faces.  This is a helper function for Highlight.
	*/
	virtual void HighlightFaces();

	/*! Call this function to highlight the selection region's faces.  This is a helper function for UnHighlight.
	*/
	virtual void UnHighlightFaces();

	HShellObject m_oShellObj;
	HShellRegion m_oRegionObj;
};


#define HSUBENTITY_FACES	0x01
#define HSUBENTITY_VERTICES	0x02
#define HSUBENTITY_EDGES	0x04

//! The HSubentitySelItem - subentity selection item which can handle instancing
/*!
  This implementation of HSmartSelItem class handles selection of arbitrary 
  subentities (faces, edges, vertices).  It uses an overdraw approach to achieve highlighting.  
*/

#include "vhash.h"

class SubentityMap: public VHash<int, int>
{

};

class MVO_API HSubentitySelItem : public HSmartSelItem
{
	friend class HSelectionSet;

public:
		
	/*! 
	Constructs a HSubentitySelItem object.
	\param key HOOPS key to selection item.
	\param selection_set A pointer to the HOOPS selection set which owns this selection item.
	\param incl_count Number of include keys in the selection item segment path.
	\param incl_keys An array of HOOPS keys of all the include keys in the selection item path.
	\param face_count The number of faces to be retained from the source shell, usually returned from HC_Show_Selection_Elements().
	\param faces The faces to be retained from the source shell, usually returned from HC_Show_Selection_Elements().
	\param vertex_count The number of vertices to be retained from the source shell, usually returned from HC_Show_Selection_Elements().  Vertices required by faces, above, will be retained in all cases.
	\param vertex1 The vertices to be retained from the source shell, usually returned from HC_Show_Selection_Elements().
	\param vertex2 If supplied, vertices in this array will also be retained and an edge will be defined between the vertex in this array and the corresponding vertex in vertex1.
	\param highlight_faces If true and if faces are supplied, faces will be highlighted 
	\param highlight_vertices If true and if vertices are supplied, vertices will be highlighted 
	\param highlight_edges If true and if both vertex arrays are supplied, edges will be highlighted 
	\param maintain_maps If true, a mapping from the points and faces of the original shell will be retained after highlighting.  This can be useful for performing operations on the original geometry, but will use more memory.
	*/
	HSubentitySelItem(
		HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY incl_keys[], 
		int face_count, const int faces[], int vertex_count, int const vertex1[], int const vertex2[], 
		bool highlight_faces = true, bool highlight_vertices = true, bool highlight_edges = true, bool maintain_maps = false);

	~HSubentitySelItem();

	/*! \return A character pointer denoting the name which is "HRegionSelItem." 
	*/
	virtual const char * GetName() const { return "HSubentitySelItem"; };

	/*! \return True if the two subentity selection elements are equal.
		\param i2 An HSelectionItem object to be compared with this object.
	*/
	virtual bool Equals(const HSelectionItem* i2) const;

	/*! \return True if the two subentity selection elements refer to the same object.
		They may or may not have the same subentity details.
		\param i2 An HSelectionItem object to be compared with this object.
	*/
	virtual bool Similar(const HSelectionItem * i2) const;

	/*! Call this function to highlight the selection region.
		\return A true if the item was successfully highlighted.
	*/
	virtual bool Highlight();
	
	/*! Call this function to unhighlight the selection item.
		\return A true if the item was successfully unhighlighted.
	*/
	virtual bool UnHighlight();

	/*! Call this function to unhighlight select elements from this selection item.
		\return true if the elements were successfully unhighlighted.
	*/
	virtual bool UnHighlight(HSubentitySelItem const * remove_sel);

	/*!
		\return true if the selection item has no faces, vertices, or edges remaining in its highlight list.
	*/
	bool IsEmpty() {return !m_vertex_count && !m_face_count; }

	/*! Call this function to delete the selection item and the selected portion of the geometry.
		\return True if the item was successfully deleted.
	*/
	virtual bool Delete();

	int GetFaceCount() const { return m_face_count; } 

	void GetFaces(int faces[]);

	int GetVertexCount() const { return m_vertex_count; }

	void GetVertices(int vertex1[], int vertex2[]);

	bool MaintainMaps() { return m_maintain_maps; }
	
	void MaintainMaps(bool maintain_maps) { m_maintain_maps=maintain_maps; }

	void GetMapCounts(int * face_map_count, int * vertex_map_count);
	
	void GetMaps(int face_map[], int vertex_map[]);

	void MergeMaps(HSubentitySelItem * that);

protected:
	HSubentitySelItem(
		bool is_ooc, HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY incl_keys[], 
		int face_count, const int faces[], int vertex_count, int const vertex1[], int const vertex2[], 
		bool highlight_faces = true, bool highlight_vertices = true, bool highlight_edges = true, bool maintain_maps = false);

private:

	int m_face_count;
	int * m_faces;
	int m_vertex_count;
	int * m_vertex1;
	int * m_vertex2;
	bool m_maintain_maps;
	int m_vertex_map_count;
	SubentityMap * m_vertex_map;
	int m_face_map_count;
	SubentityMap * m_face_map;
	int m_subentity_highlight;
};





//! The HOldSelItem class implements selection using the old style highlighting which does not handle includes.
/*!
  This class is purely provided for backward compatibility reasons and should be considered obsolete. HOldSelItem 
  implement HSelectionItem interfaces using the old style of highlighting which couldn't handle the multiply included 
  segments/entities.  
*/
class MVO_API HOldSelItem : public HSelectionItem
{

public:
	/*! Constructor */
	HOldSelItem(HC_KEY key, HSelectionSet * selection_set, const char * selection_path, 
				HC_KEY include_key, HC_KEY includer_key);


	virtual ~HOldSelItem();

	/*! \return A character pointer denoting the name of operator. 
	*/
	virtual const char * GetName() const { return "HOldSelItem"; };


	/*! \return A true when the two selection elements are equal.
		\param i2 An HOldSelItem object to be compared with this object
	*/
	virtual bool Equals(const HSelectionItem* i2) const;


	/*! Call this function to highlight the selection item
		\return A true when the highlight is succesful
	*/
	virtual bool Highlight();

	/*! Call this function to unhighlight the selection item
		\return A true if the unhighlight is succesful
	*/
	virtual bool UnHighlight();

	/*! 
		Checks to see if an item is currently highlighted
		\return true if the item is currently highlighted, false if not
	*/
	virtual bool IsHighlighted() const;


	/*! Call this function to delete the selection item
		NOTE: If the selection is a non-instanced segment or entity it will be deleted. 
			  If the selection is an instanced segment or entity, it's include link will be removed.
		\return A true if the delete is succesful
	*/
	virtual bool Delete();

	/*! 
		Checks to see if the selection item has any segments/geometry which might be 
		potentially shared. In the old selection behaviour we never handled instancing. 
		To preserve the behaviour, this returns false.
	*/
	virtual bool IsInstanced() const { return false; };

	/*! \return A count of number of include keys in the include path. Old selection doesn't deal with it.
	 */
	virtual int GetIncludeCount() const { return 0; };

	/*! \return An array to include keys. The return pointer is meant for read-only purpose
		 Old selection doesn't deal with it.
	 */
	virtual const HC_KEY * const GetIncludeKeys() const { return 0; };

	virtual void GetIncludeKeys(HC_KEY[]) const { /*do nothing*/ }

	/*! 
		Finds out if the selection item has any dependency on the include key being provide.
		If it has, the api returns true and returns which key is dependent as the second argument.
		\param include_key HC_KEY to include key on which the dependency is wanted
		\param ret_dependent_key HC_KEY to segment/entity which depends on the include key
		\return true if the item has dependency on the include_key
	*/
	virtual bool QueryIncludeDependency(HC_KEY include_key, HC_KEY& ret_dependent_key);


	/*! 
		Call to selection item to update its selection data. Specifically the key which 
		depended on the include key and have it replaced.
		\param include_key HC_KEY to include key which is deleted/going to be deleted 
		\param dependent_key HC_KEY to segment/entity which depends on the include key
		\param replacement_key new HC_KEY as a replacement to the dependent key
		\return true if the dependency has been successfully updated
	*/
	virtual bool UpdateIncludeDependency(HC_KEY include_key, HC_KEY dependent_key, HC_KEY replacement_key);

private:

	char * m_pSelectionPath;
	HC_KEY m_IncludeKey;
	HC_KEY m_IncluderKey;

	void HoopsRemove( HC_KEY key );
	void DeselectSegment( HC_KEY key, char const *keytype );
	void DeselectGeometry( HC_KEY key );
};




#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif





