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


#include "HMutexOOC.h"
#include "HSelectionSet.h"
#include "HTManager.h"
#include "HWindowFilterOOC.h"
#include "PointCloudAPI.h"
#include <map>
#include <vector>


//////////////////////////////////////////////////////////////////////////


#ifdef _WIN32
#	pragma warning(disable: 4127)
#endif


#ifndef ASSERT
#	ifdef NDEBUG
#		define ASSERT(x)		do {} while (false)
#	else
#if _WIN32
#		define ASSERT(x)		((!(x)) ? __debugbreak() : 1)
#else
#		include <assert.h>
#		define ASSERT			assert
#endif
#	endif
#endif


//////////////////////////////////////////////////////////////////////////


class HSelectionSetHelperBaseOOC {
protected:
	static HSelectionSet *& Instance ()
	{
		static HSelectionSet * instance = nullptr;
		return instance;
	}
};


/*! The HOpSelectionSetOOC class is the class that should be used when using the
	HOpSelectAreaOOC and HOpSelectPolygonOOC selection operators for selecting OOC points.
	Only one instance of HOpSelectionSetOOC can be alive at any given time.
	\param SelectionSet The underlying selection set type to use.
*/
template <typename SelectionSet>
class HSelectionSetOOC : public SelectionSet, private HSelectionSetHelperBaseOOC, private HTClient {
	typedef HMutexOOC::LockGuard LockGuard;
	typedef void (HC_CDECL * CallbackFunc)(...);

private:
	enum CallbackType {
		CallbackType_Invalid,
		CallbackType_Populate,
		CallbackType_Deleted,
	};

	struct CallbackKeyInfo {
	public:
		CallbackKeyInfo ()
			: type(CallbackType_Invalid)
		{}

		CallbackKeyInfo (HC_KEY key, CallbackType type)
			: key(key)
			, type(type)
		{
			ASSERT(type != CallbackType_Invalid);
		}

	public:
		HC_KEY key;
		CallbackType type;
	};

private:
	HSelectionSetOOC (HBaseView & view, bool deep_select)
		: SelectionSet(&view)
		, HTClient(0.01f, HTCS_PeriodicSkip)
		, filter(deep_select)
		, callback_key_infos(new VArray<CallbackKeyInfo>())
	{
		static bool callback_names_defined = false;

		static_cast<HSelectionSet *>(static_cast<SelectionSet *>(this)); // Ensure SelectionSet is a valid base class (i.e. it derives from HSelectionSet).

		{
			LockGuard lock(selection_mutex);

			if (!callback_names_defined) {
				HC_Define_Callback_Name("ooc_selection_on_populate_key", reinterpret_cast<CallbackFunc>(StaticOnPopulateKey));
				HC_Define_Callback_Name("ooc_selection_on_deleted_key", reinterpret_cast<CallbackFunc>(StaticOnDeletedKey));
				callback_names_defined = true;
			}
			HC_Define_System_Options(H_FORMAT_TEXT("populate key callback = ooc_selection_on_populate_key"));
			HC_Define_System_Options(H_FORMAT_TEXT("deleted key callback = ooc_selection_on_deleted_key"));
		}

		HDB::GetHTManager()->RegisterClient(this);
	}

	~HSelectionSetOOC ()
	{
		LockGuard lock(selection_mutex);

		HDB::GetHTManager()->UnRegisterClient(this);

		HC_Define_System_Options("no populate key callback");
		HC_Define_System_Options("no deleted key callback");

		Reset();
		delete callback_key_infos;

		Instance() = nullptr;
	}

public:
	/*! Creates a new HSelectionSetOOC instance if one does not already exist.
		Returns a null pointer otherwise.
		\param view A reference to an HBaseView object.
		\param deep_select Controls whether or not OOC points that are not loaded in memory get selected with this selection set. (See SetDeepSelection.)
		\return A pointer to the created HSelectionSetOOC.
	*/
	static HSelectionSetOOC * CreateInstance (HBaseView & view, bool deep_select)
	{
		HSelectionSet *& instance = Instance();

		if (instance == nullptr) {
			instance = new HSelectionSetOOC(view, deep_select);
			return static_cast<HSelectionSetOOC *>(instance);
		}
		else {
			ASSERT(false);
			return nullptr;
		}
	}

	/*! \return A boolean that denotes whether or not an HSelectionSetOOC object can be created by a call to CreateInstance.
	*/
	static bool CanCreateInstance ()
	{
		return Instance() == nullptr;
	}

	/*! Resets the HSelectionSetOOC object. This will deselect any points selected by this object.
	*/
	virtual void Reset ()
	{
		LockGuard lock(selection_mutex);

		delete callback_key_infos;
		callback_key_infos = new VArray<CallbackKeyInfo>();

		filter.ClearWindows();
		SelectionSet::Reset();
	}

	/*! Deletes all points selected by this object.
		This function works by doing the following:
			- Obtaining an iterator to the selected OOC points.
			- Iterating these points to associate each with a collection of points per ooc::NodeHandle.
			- Calling ooc::delta::DeleteSpecificPoints for each ooc::NodeHandle points pairing.
		Notably, this function only deletes the points in memory. This change will not be saved to disk
		unless one calls ooc::io::CommitDeltasToFile.
		\param emit_message Controls whether or not the HBaseView associated with this object is notified about the deletion.
	*/
	virtual bool DeleteSelection (bool emit_message = false)
	{
		using namespace ooc;
		using namespace ooc::delta;
		using namespace ooc::query;

		HBaseView & view = *GetView();

		bool has_root;
		HC_Open_Segment_By_Key(view.GetModelKey());{
			has_root = HC_QShow_Existence("root", "self") > 0;
		}HC_Close_Segment();

		if (!has_root) {
			return SelectionSet::DeleteSelection();
		}

		std::map<ooc::NodeHandle, std::vector<int>> handle_to_indices;
		{
			QueryIterator it = SelectedPoints();
			while (true) {
				QueryIterator::Status status = it.GetStatus();
				if (status != QueryIterator::Status_Alive) {
					if (status != QueryIterator::Status_Dead) {
						ASSERT(false); // replace with appropriate handling code
						return false;
					}
					break;
				}
				QueryResult const & res = *it;
				NodeHandle const & handle = res.GetNodeHandle();
				int point_index = static_cast<int>(res.GetNodePointIndex());
				handle_to_indices[handle].push_back(point_index);
				it.Advance();
			}
		}

		HC_KEY const ooc_root = HC_Create_Segment_Key_By_Key(view.GetModelKey(), "root");
		Env env;
		if (!GetEnv(ooc_root, env)) {
			ASSERT(false); // replace with appropriate handling code
			return false;
		}
		SyncResult sync_res = ooc::delta::SynchronizeWith(env, [&](SyncToken const & sync_token) {
			using namespace ooc;
			using namespace ooc::delta;

			auto const end = handle_to_indices.end();
			auto it = handle_to_indices.begin();
			for (; it != end; ++it) {
				auto & pair = *it;
				NodeHandle const & handle = pair.first;
				std::vector<int> & indices = pair.second;
				ModifyResult result = DeleteSpecificPoints(sync_token, handle, indices.data(), indices.size());
				if (result != ModifyResult_Success) {
					ASSERT(false); // replace with appropriate handling code
					return;
				}
			}
		});
		if (sync_res != SyncResult_Success) {
			ASSERT(false); // replace with appropriate handling code
			return false;
		}

		Reset();

		if (emit_message) {
			m_pView->EmitDeleteSelectionListMessage();
		}

		return true;
	}

	/*! Sets whether or not the selection set will select points that are not currently loaded into memory.
		\param deep_select Enables deep selection if true. Disables deep selection if false.
	*/
	void SetDeepSelection (bool deep_select)
	{
		LockGuard lock(selection_mutex);
		Reset();
		filter = HWindowFilterOOC(deep_select);
	}

	/*! \return Returns a QueryIterator to the points selected by this selection set.
	*/
	ooc::query::QueryIterator SelectedPoints ()
	{
		HBaseView & view = *GetView();
		HC_KEY const ooc_root = HC_Create_Segment_Key_By_Key(view.GetModelKey(), "root");
		ooc::Env env;
		if (!ooc::GetEnv(ooc_root, env)) {
			ASSERT(false); // replace with appropriate handling code
			return ooc::query::QueryIterator();
		}
		return ooc::query::QueryPoints(env, filter);
	}

	/*! Updates the highlighting of the selection points.
		This may be needed if points are loaded into memory or points are unloaded from memory.
		This will not force an HBaseView::Update. That is, this function will not update the
		visual presentation of the model. A seperate update is needed for that.
		This function will be automatically called in this object's Tick function, which executes periodically.
		The Tick function will call HBaseView::Update if this function returns true.
		This allows newly loaded points to be automatically highlighted without programmer interaction.
		\return Returns true if any new points have been highlighted or if any points have been unhighlighted.
	*/
	bool UpdateSelection ()
	{
		if (callback_key_infos->Count() == 0) {
			return false;
		}

		LockGuard lock(selection_mutex);

		bool update_needed = false;

		HBaseView & view = *GetView();

		HC_KEY key_path[] = {
			view.GetViewKey(),
			view.GetIncludeLinkKey(),
		};

		// Move data because the following code can modify original collection.
		VArray<CallbackKeyInfo> * infos = callback_key_infos;
		callback_key_infos = new VArray<CallbackKeyInfo>();

		size_t const count = infos->Count();
		for (size_t i = 0; i < count; ++i) {
			CallbackKeyInfo const & info = infos->GetData(i);
			switch (info.type) {
				case CallbackType_Populate: {
					HC_KEY const segment_key = info.key;
					ooc::Point min_bounding;
					ooc::Point max_bounding;
					HC_Open_Segment_By_Key(segment_key);{
						HC_Show_Bounding_Cuboid(&min_bounding, &max_bounding);
						if (!filter.RejectBounding(min_bounding, max_bounding)) {
							HC_Begin_Contents_Search(".", "shell");{
								HC_KEY shell_key;
								while (HC_Find_Contents(nullptr, &shell_key)) {
									int point_count;
									HC_Show_Shell_Size(shell_key, &point_count, nullptr);
									ooc::Point const * points;
									HC_Show_Geometry_Pointer(shell_key, "points", &points);
									std::vector<int> points_to_highlight;
									for (int i = 0; i < point_count; ++i) {
										ooc::Point const & point = points[i];
										if (filter.AcceptPoint(point, static_cast<size_t>(i))) {
											points_to_highlight.push_back(i);
										}
									}
									if (!points_to_highlight.empty()) {
										update_needed |= SelectSubentityOOC(
											shell_key, sizeof(key_path) / sizeof(HC_KEY), key_path,
											static_cast<int>(points_to_highlight.size()),
											nullptr, points_to_highlight.data(), nullptr, true);
									}
								}
							}HC_End_Contents_Search();
						}
					}HC_Close_Segment();
				} break;

				case CallbackType_Deleted: {
					HC_KEY const shell_key = info.key;
					update_needed |= DeSelectOOC(shell_key, sizeof(key_path) / sizeof(HC_KEY), key_path);
				} break;

				default: {
					ASSERT(false);
				}
			}
		}

		delete infos;

		return update_needed;
	}

	/*! Adds a rectangular window to select points from in window space coordinates.
		This should only be called within an HSelectionSetOOC::SynchronizeWith callback.
		\param min The minumum point of the rectangle.
		\param max The maximum point of the rectangle.
	*/
	void AddRectangleWindow (ooc::Point const & min, ooc::Point const & max)
	{
		LockGuard lock(selection_mutex);
		filter.AddRectangleWindow(*GetView(), min, max);
	}

	/*! Adds a triangular window to select points from in window space coordinates.
		This should only be called within an HSelectionSetOOC::SynchronizeWith callback.
		\param p1 The first point of the triangle.
		\param p2 The second point of the triangle.
		\param p3 The third point of the triangle.
	*/
	void AddTriangleWindow (ooc::Point const & p1, ooc::Point const & p2, ooc::Point const & p3)
	{
		LockGuard lock(selection_mutex);
		filter.AddTriangleWindow(*GetView(), p1, p2, p3);
	}

	/*! Creates a synchronization point to add rectangular or triangular windows to the selection set.
		(See AddRectangleWindow and AddTriangleWindow functions.)
		\param task The callback to be called when the HSelectionSetOOC object is in a synchronized state.
	*/
	template <typename Func>
	auto SynchronizeWith (Func const & task) -> decltype(task())
	{
		LockGuard lock(selection_mutex);
		return task();
	}

private:
	virtual bool Tick (float request_time, float actual_time)
	{
		UNREFERENCED(request_time);
		UNREFERENCED(actual_time);

		while (UpdateSelection()) {
			GetView()->Update();
		}

		return true;
	}

	void OnPopulateKey (HC_KEY segment_key)
	{
		if (filter.HasWindows()) {
			LockGuard lock(selection_mutex);
			if (filter.HasWindows()) {
				CallbackKeyInfo info(segment_key, CallbackType_Populate);
				callback_key_infos->Append(info);
			}
		}
	}

	static void HC_CDECL StaticOnPopulateKey (HC_KEY segment_key)
	{
		static_cast<HSelectionSetOOC *>(Instance())->OnPopulateKey(segment_key);
	}

	void OnDeletedKey (HC_KEY key, int type)
	{
		if (type == HIC_Geometry_Type_SHELL) {
			if (filter.HasWindows()) {
				LockGuard lock(selection_mutex);
				if (filter.HasWindows()) {
					CallbackKeyInfo info(key, CallbackType_Deleted);
					callback_key_infos->Append(info);
				}
			}
		}
	}

	static void HC_CDECL StaticOnDeletedKey (HC_KEY key, int type)
	{
		static_cast<HSelectionSetOOC *>(Instance())->OnDeletedKey(key, type);
	}

private:
	HWindowFilterOOC filter;
	HMutexOOC selection_mutex;
	VArray<CallbackKeyInfo> * callback_key_infos; // Using a pointer for fast "move" semantics
};





