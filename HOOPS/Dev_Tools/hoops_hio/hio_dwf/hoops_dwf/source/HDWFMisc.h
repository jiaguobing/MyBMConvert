#ifndef HDWFMISC_H
#define HDWFMISC_H

#include "HW2DReaderCommon.h"

struct Point;

static inline void WT2HT(const WT_Integer32 in_x, const WT_Integer32 in_y, 
	float & out_x, float & out_y, float & out_z,
	const HW2DReaderCommon * pW2DReaderCommon);

static inline void WT2HT(const WT_Logical_Point & in, Point & out, const HW2DReaderCommon * pW2DReaderCommon);

static inline void WT2HT (const float in_x, const float in_y,
	float & out_x, float & out_y, float & out_z,
	const HW2DReaderCommon * pW2DReaderCommon);

static inline void WT2HT(const WT_Point_Set & in, Point * out, HW2DReaderCommon * pW2DReaderCommon);

static inline void WT2HT(const WT_Logical_Point * points, int count, Point * out, HW2DReaderCommon * pW2DReaderCommon);

struct Point 
{
	Point ()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Point(const WT_Integer32 xx, const WT_Integer32 yy, const HW2DReaderCommon * pHW2DReaderCommon)
	{ WT2HT(xx, yy, x, y, z, pHW2DReaderCommon); }

	Point(const WT_Logical_Point p, const HW2DReaderCommon * pHW2DReaderCommon)
	{ WT2HT(p.m_x, p.m_y, x, y, z, pHW2DReaderCommon); }

	Point (const float xx, const float yy, const HW2DReaderCommon * pHW2DReaderCommon)
	{ WT2HT (xx, yy, x, y, z, pHW2DReaderCommon); }

	float x, y, z;
};

class PointSet
{
public:
	PointSet(const WT_Logical_Point * points, WT_Integer32 count, HW2DReaderCommon * pW2DReaderCommon);
	PointSet(const WT_Point_Set & ps, HW2DReaderCommon * pW2DReaderCommon);
	~PointSet();

	Point * points() const { return m_pts; }
	int count() const { return m_count; }
	Point * GetPoint(int idx) { return &m_pts[idx]; }

private:
	void set(const WT_Logical_Point * points);
	void set(const WT_Point_Set & in);

	Point	m_pts_static[32];
	Point * m_pts;
	int		m_count;
	bool	m_isDynamic;

	HW2DReaderCommon * m_pW2DReaderCommon;
};

//update bounds functions
static inline void update_bounds(WT_Logical_Box & lhlb, const WT_Logical_Box & rhlb) 
{
	if( rhlb.m_min.m_x < lhlb.m_min.m_x )
		lhlb.m_min.m_x = rhlb.m_min.m_x;

	if( rhlb.m_min.m_y < lhlb.m_min.m_y )
		lhlb.m_min.m_y = rhlb.m_min.m_y;

	if( rhlb.m_max.m_x > lhlb.m_max.m_x )
		lhlb.m_max.m_x = rhlb.m_max.m_x;

	if( rhlb.m_max.m_y > lhlb.m_max.m_y )
		lhlb.m_max.m_y = rhlb.m_max.m_y;
}

static inline void update_bounds(WT_Logical_Box & lhlb, const WT_Logical_Point & wt_point) 
{
	if( wt_point.m_x < lhlb.m_min.m_x )
		lhlb.m_min.m_x = wt_point.m_x;
	else if( wt_point.m_x > lhlb.m_max.m_x )
		lhlb.m_max.m_x = wt_point.m_x;

	if( wt_point.m_y < lhlb.m_min.m_y )
		lhlb.m_min.m_y = wt_point.m_y;
	else if( wt_point.m_y > lhlb.m_max.m_y )
		lhlb.m_max.m_y = wt_point.m_y;
}

static inline void update_bounds(WT_Logical_Box & lhlb, const int nPoints, const WT_Logical_Point * pPoints) 
{
	for( int i = 0; i < nPoints; i++ )
		update_bounds(lhlb, pPoints[i]);
}

static void WT2HT(const WT_Integer32 in_x, const WT_Integer32 in_y, 
	float & out_x, float & out_y, float & out_z,
	const HW2DReaderCommon * pW2DReaderCommon)
{
	if(pW2DReaderCommon->m_CoordMode == HW2DReaderCommon::recentered_coords)
	{
		out_x = in_x - pW2DReaderCommon->m_x_extents;
		out_y = in_y - pW2DReaderCommon->m_y_extents;
		out_z = pW2DReaderCommon->m_z_plane;
	}
	else if(pW2DReaderCommon->m_CoordMode == HW2DReaderCommon::application_coords)
	{
		WT_Point3D pt_in((double)in_x, (double)in_y);
		WT_Point3D pt_out = pW2DReaderCommon->m_pWhipUnits->transform_from_DWF_to_application(pt_in);
		out_x = pt_out.m_x;
		out_y = pt_out.m_y;
		out_z = pW2DReaderCommon->m_z_plane;
	}
	else
	{
		out_x = in_x;
		out_y = in_y;
		out_z = pW2DReaderCommon->m_z_plane;
	}
}

static void WT2HT(const WT_Logical_Point & in, Point & out, const HW2DReaderCommon * pW2DReaderCommon)
{ WT2HT(in.m_x, in.m_y, out.x, out.y, out.z, pW2DReaderCommon); }

static void WT2HT (const float in_x, const float in_y,
	float & out_x, float & out_y, float & out_z,
	const HW2DReaderCommon * pW2DReaderCommon)
{
	if (pW2DReaderCommon->m_CoordMode == HW2DReaderCommon::recentered_coords)
	{
		out_x = in_x - pW2DReaderCommon->m_x_extents;
		out_y = in_y - pW2DReaderCommon->m_y_extents;
		out_z = pW2DReaderCommon->m_z_plane;
	}
	else if(pW2DReaderCommon->m_CoordMode == HW2DReaderCommon::application_coords)
	{
		WT_Point3D pt_in((double)in_x, (double)in_y);
		WT_Point3D pt_out = pW2DReaderCommon->m_pWhipUnits->transform_from_DWF_to_application(pt_in);
		out_x = pt_out.m_x;
		out_y = pt_out.m_y;
		out_z = pW2DReaderCommon->m_z_plane;
	}
	else
	{
		out_x = in_x;
		out_y = in_y;
		out_z = pW2DReaderCommon->m_z_plane;
	}
}

static void WT2HT(const WT_Point_Set & in, Point * out, HW2DReaderCommon * pW2DReaderCommon)
{
	WT_Logical_Point * wlp = in.points();

	for (int i = 0; i < in.count(); i++)
		WT2HT(wlp[i], out[i], pW2DReaderCommon);
}

static void WT2HT(const WT_Logical_Point * points, int count, Point * out, HW2DReaderCommon * pW2DReaderCommon)
{    
	for (int i = 0; i < count; i++)
		WT2HT(points[i], out[i], pW2DReaderCommon);
}

void eliminate_special_chars(char * zString);
void set_2d_view_mode(HBaseView * hbv);
void set_3d_view_mode(HBaseView * hbv);

#endif //HDWFMISC_H