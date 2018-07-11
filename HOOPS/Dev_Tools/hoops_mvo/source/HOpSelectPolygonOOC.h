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


#include "HOpSelectPolygon.h"
#include "HSelectionSetOOC.h"
#include <vector>


//////////////////////////////////////////////////////////////////////////


/*! The HSelectPolygonOOC class computes a selection list for OOC points inside a polygonal area.
	If this class is used by an HBaseView, the view must have an HSelectionSetOOC<SelectionSet>
	instance for its selection set.
	\param SelectionSet The selection set type used by HSelectionSetOOC<SelectionSet>
*/
template <typename SelectionSet>
class HOpSelectPolygonOOC : public HOpSelectPolygon {
public:
	/*! Constructs an HOpSelectPolygonOOC.
		\param view A reference to an HBaseView object.
	*/
	HOpSelectPolygonOOC (HBaseView & view)
		: HOpSelectPolygon(&view)
	{}

	/*!	\return A pointer to a character string denoting the name of the operator, which is "HOpSelectPolygonOOC".
	*/
	virtual char const * GetName ()
	{
		return "HOpSelectPolygonOOC";
	}

	/*! Creates a new operator which is associated with the same view as the current operator.
		The user is responsible for deleting the newly created operator.
		\return A pointer to a copy of this operator.
	  */
	virtual HBaseOperator * Clone ()
	{
		return new HOpSelectPolygonOOC(*GetView());
	}

	/*! Starts to create a selection lasso that will select points inside it.
		Each time the left mouse button is pressed, a new vertex is added to the polygon lasso.
		\param e An HEventInfo object containing information about the current event.
		\return A value indicating the status of the event.
	*/
	virtual int OnLButtonDown (HEventInfo & e)
	{
		GetSelectionSet().DeSelectAll();
		return HOpSelectPolygon::OnLButtonDown(e);
	}

	/*! Finalizes the selection lasso created by OnLButtonDown and selects points that lie
		inside the polygon lasso.
		\param e An HEventInfo object containing information about the current event.
		\return A value indicating the status of the event.
	*/	virtual int OnLButtonDblClk (HEventInfo & e)
	{
		HSelectionSetOOC<SelectionSet> & selset = GetSelectionSet();

		return selset.SynchronizeWith([&] () -> int {
			int const result = HOpSelectPolygon::OnLButtonDblClk(e);

			std::vector<ooc::Point> const triangles(ComputePolygonTriangles());

			for (size_t i = 0; i < triangles.size(); i += 3) {
				selset.AddTriangleWindow(triangles[i], triangles[i + 1], triangles[i + 2]);
			}

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

	std::vector<ooc::Point> ComputePolygonTriangles ()
	{
		std::vector<ooc::Point> triangles;

		HC_Open_Segment("/");{
			HC_KEY segment_key = HC_Open_Segment("");{
				HC_KEY polygon_key = HC_Insert_Polygon(m_PolylineCount, m_pPolyline);
				HC_KEY shell_key = HC_Generate_Shell_From_Geometry(polygon_key, "");

				int point_count;
				int tristrip_count;
				HC_Show_Shell_By_Tristrips_Size(shell_key, &point_count, &tristrip_count, 0);
				std::vector<ooc::Point> points(point_count);
				std::vector<int> tristrips(tristrip_count);
				HC_Show_Shell_By_Tristrips(shell_key, 0, points.data(), 0, tristrips.data(), 0, 0);

				triangles = ComputeTrianglesFromTristrips(points, tristrips);
			}HC_Close_Segment();

			HC_Delete_By_Key(segment_key);
		}HC_Close_Segment();

		return triangles;
	}

	static bool IsDegenerate (int const (&triangle_indices)[3])
	{
		if (triangle_indices[0] == triangle_indices[1]) {
			return true;
		}
		if (triangle_indices[0] == triangle_indices[2]) {
			return true;
		}
		if (triangle_indices[1] == triangle_indices[2]) {
			return true;
		}
		return false;
	}

	static std::vector<ooc::Point> ComputeTrianglesFromTristrips (
		std::vector<ooc::Point> const & points,
		std::vector<int> const & tristrips)
	{
		std::vector<ooc::Point> triangles;

		for (size_t i = 0; i < tristrips.size(); ) {
			size_t const strip_len = tristrips[i++];
			ASSERT(i + strip_len <= tristrips.size());
			for (size_t j = 2; j < strip_len; ++j) {
				ASSERT(i + j < tristrips.size());
				int triangle_indices[] = {
					tristrips[i + j - 2],
					tristrips[i + j - 1],
					tristrips[i + j - 0],
				};
				if (!IsDegenerate(triangle_indices)) {
					for (size_t k = 0; k < 3; ++k) {
						triangles.push_back(points[triangle_indices[k]]);
					}
				}
			}
			i += strip_len;
		}

		return triangles;
	}
};



