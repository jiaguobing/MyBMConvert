// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HGraph.h
	Defines datatypes associated with the HBaseGraph, HPlot2D, and HPieChart class.
*/
#ifndef _HGraph_H
#define _HGraph_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HUtility.h"
#include "HTools.h"
#include "varray.h"

#define _PI 3.14159265358979323

class HGraphAxis;
class HGraphLabelNode;
class HGraphDataSetNode;
class HGraphPieSlice;

#ifdef WINDOWS_SYSTEM
template class MVO_API VArray< HGraphLabelNode *>;
template class MVO_API VArray< HGraphDataSetNode *>;
template class MVO_API VArray< HGraphPieSlice *>;
#endif

/*! \enum HGraphAxisSelection */
enum HGraphAxisSelection
{
	X_Axis=1,
	Y_Axis,
	Z_Axis
};

/*! \enum HGraphPointFormat */
enum HGraphPointFormat
{
	PointFormatCartesian=1,
	PointFormatPolarDegrees,
	PointFormatPolarRadians
};

/*! \enum HGraphAxisScale */
enum HGraphAxisScale
{
	AxisScaleLinear=1,
	AxisScaleLogarithmic
};

/*! \enum HGraphGridType */
enum HGraphGridType
{
	GridTypeNone=1,
	GridTypeRectangular,
	GridTypePolar
};

/*! \enum HGraphPlotType */
enum HGraphPlotType
{
	PlotTypeScatter=1,
	PlotTypeLine,
	PlotTypeBar,
};

/*! \enum HGraphLegendEntryType */
enum HGraphLegendEntryType
{
	LegendEntryTypeLine=1,
	LegendEntryTypeBox
};

/*!< internal use only */
class MVO_API HGraphAxis
{
public:
	HGraphAxis()
		: axis_scale_factor(1.0)
		, tick_frequency(1.0)
		, tick_size(0.1)
		, grid_frequency(1.0)
		, grid_min(-4.0)
		, grid_max(4.0)
		, segment(INVALID_KEY)
		, precision(-1)
		, axis_scale(AxisScaleLinear)
		, show_labels(true)
		, visible(true)
	{
		min = -4.0;
		max = 4.0;
	}

	double min;					//minimal point of axis
	double max;					//maximal point of axis
	
	double axis_scale_factor;   //multiplier, or logarithmic base of the axis (based on axis_scale)
	double tick_frequency;		//how frequently ticks occur on the axis (respects the axis_scale setting)
	double tick_size;			//the size of the actual ticks

	double grid_frequency;		//how frequently grid lines occur on this axis
	double grid_min;			//minimal point on the axis of the grid
	double grid_max;			//maximal point on the axis of the grid

	HC_KEY segment;				//the location of the axis

	int precision;				//the number of places after the decimal point to display

	HGraphAxisScale axis_scale;	//is the axis linear or logarithmic?
	
	bool show_labels;			//whether or not to label each tick
	bool visible;				//is the axis visible?
};

/*!< internal use only */
class MVO_API HGraphLabelNode
{
public:
	HC_KEY segment;				//the segment housing the label
	HC_KEY text_key;			//the key of the label itself
};

/*!< internal use only */
class MVO_API HGraphDataSetNode
{
public:
	double bar_width;			//the width of the bars to be drawn
	HC_KEY segment;				//location in the segment tree of the data
	int data_size;				//how many points are in the data set
	HGraphPointFormat format;	//format of point data
	bool line_vis;				//are the points connected by a polyline?
	bool bar_vis;				//visible in bar charts
	bool polygon_vis;			//the visibility of a colored region
};

/*!< internal use only */
class MVO_API HGraphPieSlice
{
public:
	double amount;				//how much of the pie is in this slice.
	HC_KEY segment;				//the segment housing the slice
	HC_KEY text_key;			//the key of any label (if any);
};

/*!< internal use only */
class MVO_API HGraphLegend
{
public:
HGraphLegend():segment(INVALID_KEY), title(INVALID_KEY) {entries.ReplaceAt(0,0);}

HC_KEY segment;
HC_KEY title;
VArray< HGraphLabelNode *> entries;
};

/*! Provides basic graphing capabilities required by all types of graphs */
class MVO_API HBaseGraph
{
public:

	/*!
	HBaseGraph Constructor.  

	\param plot_seg The segment that all graph subsegments will be created beneath.
	*/
	HBaseGraph(HC_KEY plot_seg);

	/*!
	~HBaseGraph Destructor.
	*/
	virtual ~HBaseGraph();


	/*! 
	This method retrieves the title of the plot.

	\param title The title of the plot. Returned to user. Passed by reference always.
	*/
	void GetPlotTitle(char * title);

	/*! 
	This method retrieves the title of the plot.

	\param title The title of the plot. Returned to user. Passed by reference always.
	*/
	void GetPlotUnicodeTitle(unsigned short * title);


	/*! 
	This method adds a plot title.  If a title already exists, it replaces the existing one.

	\param title The title of the plot.
	*/
	virtual void SetPlotTitle(const char * title)=0;

	/*! 
	This methods adds a unicode plot title.  If a title already exists, it replaces the existing one.

	\param title The title of the plot.
	*/
	virtual void SetPlotUnicodeTitle(unsigned short * title)=0;

	/*! 
	This method retrieves the color of the title of the plot.

	\param color The color of the title of the plot. Passed by reference always.
	*/
	virtual void GetPlotTitleColor(char * color);

	/*! 
	This method sets a plot title color.

	\param color The color of the title of the plot.
	*/
	virtual void SetPlotTitleColor(const char * color);


	/*! 
	Retrieves the current font used to render the plot title.

	\param font The font of the plot title. Returned to user. Passed by reference always.
	*/
	void GetPlotTitleTextFont(char * font);

	/*! 
	Retrieves the current font used to render the plot title.

	\param font The font of the plot title. Passed by reference always.
	*/
	void SetPlotTitleTextFont(const char * font);


	/*! 
	Retrieves the location of the current plot title.

	\return An HPoint value representing the location of the title of the plot.
	*/
	HPoint GetPlotTitleLocation();

	/*! 
	Sets the location of the plot title.

	\param loc An HPoint value representing the new location of the title of the plot.
	*/
	void SetPlotTitleLocation(HPoint loc);


	/*! 
	This method retrieves the location of the current plot origin.

	\return An HPoint value representing the location of the plot's origin point.
	*/
	HPoint GetPlotOrigin();

	/*! 
	This method sets the location of the plot origin.

	\param origin An HPoint value representing the new location of the title of the plot.
	*/
	void SetPlotOrigin(HPoint origin);


	/*! 
	This method retrieves the visibility of the plot frame.

	\return A boolean value reflecting the current visibility of the plot's frame
	*/
	bool GetFrameVisibility();

	/*! 
	This method sets the visibility of the plot frame.

	\param visible A boolean value setting the current visibility of the plot's frame
	*/
	void SetFrameVisibility(bool visible);


	/*! 
	This method retrieves the current frame line pattern.

	\param pattern The line pattern. Returned to user. Passed by reference always.
	*/
	void GetFramePattern(char * pattern);

	/*! 
	Sets the frame line pattern.

	\param pattern The line pattern. Passed by reference always.
	*/
	void SetFramePattern(const char * pattern);

	
	/*! 
	This method retrieves the current frame color.

	\param color The frame color. Returned to user. Passed by reference always.
	*/
	void GetFrameColor(char * color);

	/*! 
	This method sets the frame color.

	\param color The frame color. Passed by reference always.
	*/
	void SetFrameColor(const char * color);

	
	/*! 
	This method retrieves the line weight of the current plot frame.

	\return The frame weight.
	*/
	float GetFrameWeight();

	/*! 
	Sets the line weight of the plot frame.

	\param weight The new frame weight to be set.
	*/
	void SetFrameWeight(float weight);

	
	/*! 
	Adds a label on the graph, returns the ID of the label.

	\param str The contents of the label.
	\param loc The location on the graph to put the label.
	\param format The format of the point loc.  Default value: PointFormatCartesian.
	\param xvector The x component of the vector that the label text will be aligned with.  Default value: (1.0, 0.0, 0.0).
	\param yvector The y component of the vector that the label text will be aligned with.  Default value: (1.0, 0.0, 0.0).
	\param zvector The z component of the vector that the label text will be aligned with.  Default value: (1.0, 0.0, 0.0).
	\return The ID of the label just added.  This ID can be used to later modify the label.
	*/
	int AddLabel(const char * str, HPoint loc, HGraphPointFormat format=PointFormatCartesian, float xvector=1.0,
		float yvector=0.0, float zvector=0.0);
	
	/*! 
	Adds a unicode label on the graph and returns the ID of the label.

	\param str The contents of the label.
	\param loc The location on the graph to put the label.
	\param format The format of the point loc.  Default value: PointFormatCartesian.
	\param xvector The x component of the vector that the label text will be aligned with.  Default value: (1.0, 0.0, 0.0).
	\param yvector The y component of the vector that the label text will be aligned with.  Default value: (1.0, 0.0, 0.0).
	\param zvector The z component of the vector that the label text will be aligned with.  Default value: (1.0, 0.0, 0.0).
	\return The ID of the label just added.  Used to later modify the label.
	*/
	int AddUnicodeLabel(const unsigned short * str, HPoint loc, HGraphPointFormat format=PointFormatCartesian, 
		float xvector=1.0, float yvector=0.0, float zvector=0.0);
	
	/*! 
	Removes a previously created label.

	\param label_id The ID of the label to be removed.
	*/
	void RemoveLabel(int label_id);


	/*! 
	Shows the contents of a label.

	\param label_id The ID of the label.
	\param str The label string. Returned to user. Passed by reference always.
	*/
	void GetLabelContents(int label_id, char * str);

	/*! 
	Shows the contents of a unicode label.

	\param label_id The ID of the label.
	\param str The label string. Returned to user. Passed by reference always.
	*/
	void GetLabelUnicodeContents(int label_id, unsigned short * str);


/*! 
	Replaces the contents of a label.

	\param label_id The ID of the label.
	\param str The label string. Passed by reference always.
	*/
	void SetLabelContents(int label_id, const char * str);

	/*! 
	Replaces the contents of a unicode label.

	\param label_id The ID of the label.
	\param str The label string. Passed by reference always.
	*/
	void SetLabelUnicodeContents(int label_id, const unsigned short * str);


	/*! 
	Sets the text font of a label.

	\param label_id The ID of the label.
	\param font The label font. Passed by reference always.
	*/
	void SetLabelTextFont(int label_id, const char * font);

	/*! 
	Retrieves the text font of a label.

	\param label_id The ID of the label.
	\param font The label font. Returned to user. Passed by reference always.
	*/
	void GetLabelTextFont(int label_id, char * font);

	/*! 
	Sets the text color of a label.

	\param label_id The ID of the label.
	\param color The label color. Passed by reference always.
	*/
	void SetLabelTextColor(int label_id, const char * color);

	/*! 
	Retrieves the text color of a label.

	\param label_id The ID of the label.
	\param color The label color. Returned to user. Passed by reference always.
	*/
	void GetLabelTextColor(int label_id, char * color);
	
	/*! 
	Sets the text alignment of a label.

	\param label_id The ID of the label.
	\param alignment The label alignment. Passed by reference always.
	*/
	void SetLabelTextAlignment(int label_id, const char * alignment);

	/*! 
	Retrieves the text alignment of a label.

	\param label_id The ID of the label.
	\param alignment The label alignment. Returned to user. Passed by reference always.
	*/
	void GetLabelTextAlignment(int label_id, char * alignment);

	/*! 
	Sets the location of a label.

	\param label_id The ID of the label.
	\param loc The label location. 
	\param format The format of the point loc.  Default value: PointFormatCartesian.
	*/
	void SetLabelLocation(int label_id, HPoint loc, HGraphPointFormat format=PointFormatCartesian);

	/*! 
	Retrieves the location of a label.

	\param label_id The ID of the label.
	\return The label location in cartesian space. 
	*/
	HPoint GetLabelLocation(int label_id);

	/*! 
	Sets the text path used for a label.

	\param label_id The ID of the label.
	\param xvector The x component of the text path vector.
	\param yvector The y component of the text path vector.
	\param zvector The z component of the text path vector.
	*/	
	void SetLabelTextPath(int label_id, float xvector, float yvector, float zvector);


	/*! 
	Gets the text path used for a label

	\param label_id The ID of the label.
	\param xvector The x component of the text path vector. Returned to user.
	\param yvector The y component of the text path vector. Returned to user.
	\param zvector The z component of the text path vector. Returned to user.
	*/	
	void GetLabelTextPath(int label_id, float *xvector, float *yvector, float *zvector);


	/*! 
	Adds a legend to the current plot.  Each plot may only have one legend.

	\param loc The location of the top left corner of the legend. 
	\param format The format of the point loc.  Default value: PointFormatCartesian.
	*/
	void AddLegend(HPoint loc, HGraphPointFormat format=PointFormatCartesian);

	/*! 
	Removes the current legend (if any) from the plot and deletes all legend entries.
	*/
	void RemoveLegend();


	/*! 
	Sets the title of the legend.

	\param title The title of the plot. Passed by reference always.
	*/
	void SetLegendTitle(const char * title);

	/*! 
	Retrieves the title of the legend.

	\param title The title of the plot. Returned to user. Passed by reference always.
	*/
	void GetLegendTitle(char * title);

	/*! 
	Removes the title of the legend.  This method has no effect if the title is uninitialized.
	*/
	void RemoveLegendTitle();

	/*! 
	Sets a unicode title for the legend.

	\param title The title of the plot. Passed by reference always.
	*/
	void SetLegendUnicodeTitle(const unsigned short * title);

	/*! 
	Gets the unicode legend title.

	\param title The title of the plot. Returned to user. Passed by reference always.
	*/
	void GetLegendUnicodeTitle(unsigned short * title);
	
	
	/*! 
	Sets the text font of the legend title.  Note that oru font sizes should be used.

	\param font The font of the legend title. Passed by reference always.
	*/
	void SetLegendTitleTextFont(const char * font);

	/*! 
	Retrieves the text font of the legend title.

	\param font The font of the legend title. Returned to user. Passed by reference always.
	*/
	void GetLegendTitleTextFont(char * font);


	/*! 
	Sets the text font of the legend entries.  Note that oru font sizes should be used.

	\param font The font of the legend entries. Passed by reference always.
	*/
	void SetLegendTextFont(const char * font);

	/*! 
	Sets the text font of the legend entries.  Note that oru font sizes should be used.

	\param font The font of the legend entries. Returned to user. Passed by reference always.
	*/
	void GetLegendTextFont(char * font);

	/*! 
	Adds an entry to the legend associated with a color.

	\param str The string to be entered in to the legend.
	\param color The color associated with the string.
	\param ltype What sort of geometry should be drawn in the legend entry.
	\return The ID of the legend entry.  This ID can be used to later modify or remove the legend entry.
	*/
	int AddLegendEntry(const char * str, const char * color, HGraphLegendEntryType ltype=LegendEntryTypeLine);

	/*! 
	Adds a unicode legend entry associated with a color.

	\param str The string to be entered in to the legend.
	\param color The color associated with the string.
	\param ltype What sort of geometry should be drawn in the legend entry.
	\return The ID of the legend entry.  This ID can be used to later modify or remove the legend entry.
	*/
	int AddLegendUnicodeEntry(const unsigned short * str, const char * color, HGraphLegendEntryType ltype=LegendEntryTypeLine);
	
	/*!
	Removes the legend entry associated with entry_id.

	\param entry_id The legend entry to be removed.
	*/
	void RemoveLegendEntry(int entry_id);

	/*!
	Retrieves the legend entry associated with entry_id.

	\param entry_id The legend entry.
	\param str The text string associated with the legend entry.
	\param color The color of the geometry associated with the legend entry.
	\param entry_type Optionally, the type of geometry drawn in the legend entry can be retrieved. Returned to user. Passed by reference always.
	*/
	void GetLegendEntry(int entry_id, char * str, char * color, HGraphLegendEntryType * entry_type=0);

	/*!
	Gets the unicode legend entry specified by the entry_id.

	\param entry_id The legend entry.
	\param str The unicode string associated with the legend entry.
	\param color The color of the geometry associated with the legend entry.
	\param entry_type Optionally, the type of geometry drawn in the legend entry can be retrieved. Returned to user. Passed by reference always.
	*/
	void GetLegendUnicodeEntry(int entry_id, unsigned short * str, char * color, HGraphLegendEntryType * entry_type=0);

	
	/*! 
	Sets the location of the legend.

	\param loc The label location. 
	\param format The format of the point loc.  Default value: PointFormatCartesian.
	*/
	void SetLegendLocation(HPoint loc, HGraphPointFormat format=PointFormatCartesian);

	/*! 
	Gets the location of the legend.

	\return The location of the legend in cartesian space.
	*/
	HPoint GetLegendLocation();

	/*! 
	Converts in_points from one system to another.  It does not actually modify in_points, but returns the transformed points as out_points.  For in place conversion, out_points can be the same as in_points.

	\param in_count The size of the in_points array.
	\param in_points An array of points to be transformed. Passed by reference always.
	\param in_system The point system that in_points is in.
	\param out_points An array of points the same size as in_points to hold the transformed values. Returned to user. Passed by reference always.
	\param out_system The point system that in_points should be transformed into.
	*/
	void ConvertPoints(unsigned int in_count, const HPoint in_points[], HGraphPointFormat in_system, 
		HPoint out_points[], HGraphPointFormat out_system);

	/*! 
	If preserve is true, the plot_seg passed into the constructor will be preserved when the plot object is destroyed.  Otherwise the segment and all child segments will be deleted.
	
	\param preserve Set this parameter to true if you wish to presevere the plog segment.
	*/
	void PreserveData(bool preserve=true);

	/*!
	If automatic updates are enabled, each call that modifies the graph will immediately modify the underlying
	segment tree.  If not, updates must be triggered manually by the user via the Update() call.  
	By default, automatic updates are on.

	\param automatic If true, automatic updates will be enabled.
	*/
	void SetAutomaticUpdates(bool automatic=true);

	/*!
	Gets the status of automatic updates.

	\return true if automatic updates are enabled, false otherwise.	
	*/
	bool GetAutomaticUpdates();

	/*!
	Force the segment tree to reflect any queued changes in the graph.
	*/
	virtual void Update();
 
protected:
	HC_KEY m_plot_segment;
	HPoint m_origin;
	bool m_show_frame, m_preserve_data, m_automatic_updates, m_force_update;	
	HGraphLegend m_legend;

	
	VArray< HGraphLabelNode *> m_labels;

	/*! 
	Draws a frame around the perimeter of the plot.
	*/
	virtual void DrawFrame()=0;

	/*! 
	Constructs the legend in the Hoops segment tree.
	*/
	virtual void DrawLegend();
};

//! Provides plotting capabilities for most 2d graphs including bar charts, line graphs, scatter plots, or any combination of these.
class MVO_API HPlot2D : public HBaseGraph
{
public:

	/*!
	HPlot2D Constructor.  

	\param plot_seg The segment that all graph subsegments will be created beneath.
	\param plot_type The type of 2d plot preferred.  This just affects the default visibilities for datasets created on this plot.  
	*/
	HPlot2D(HC_KEY plot_seg, HGraphPlotType plot_type=PlotTypeScatter);

	/*!
	~HPlot2D Destructor
	*/
	virtual ~HPlot2D();

	/*! 
	Adds a plot title or replaces the existing one.

	\param title The title of the plot.
	*/
	void SetPlotTitle(const char * title);

	/*! 
	Adds a unicode plot title or replaces the existing one.

	\param title The title of the plot.
	*/
	void SetPlotUnicodeTitle(unsigned short * title);


	/*! 
	Retrieves the type of plot specified when creating the graph.

	\return The type of plot specified when creating the graph.
	*/
	HGraphPlotType GetPlotType();

	/*! 
	Retrieves the location of the plot origin.

	\param origin An HPoint value representing the new location of the title of the plot.
	*/
	void SetPlotOrigin(HPoint origin);

	
	/*! 
	Retrieves the type of grid that will be drawn over the plot.

	\return The type of grid.
	*/
	HGraphGridType GetGridType();
	
	/*! 
	Sets the type of grid that will be drawn over the plot.

	\param gtype The type of grid.
	*/
	void SetGridType(HGraphGridType gtype);


	/*! 
	Retrieves the visibility state of the grid.

	\return True if the grid is visible and false if otherwise.
	*/
	bool GetGridVisibility();

	/*! 
	Sets the visibility for the grid.

	\param value The grid visibility flag: true sets visibility on, false to off.
	*/
	void SetGridVisibility(bool value);

	/*! 
	Retrieves the line pattern used for the grid.

	\param pattern The line pattern used for the grid. Returned to user. Passed by reference always.
	*/
	void GetGridPattern(char * pattern);

	/*! 
	Sets the line pattern used for the grid.

	\param pattern The line pattern used for the grid. Passed by reference always.
	*/
	void SetGridPattern(const char * pattern);


	/*! 
	Retrieves the color used for the grid.

	\param color The color used for the grid. Returned to user. Passed by reference always.
	*/
	void GetGridColor(char * color);

	/*! 
	Sets the color used for the grid.

	\param color The color used for the grid. Passed by reference always.
	*/
	void SetGridColor(const char * color);

	/*! 
	Retrieves the scale used for labeling an axis.

	\param axis The axis.
	\param factor Optionally the scaling factor may be retrieved. Returned to user. Passed by reference always
	\return The type of axis scaling performed.
	*/
	HGraphAxisScale GetAxisScale(HGraphAxisSelection axis, double * factor=0);

	/*! 
	Sets the scale used for labeling an axis.

	\param axis The axis.
	\param scale The type of axis scaling to be performed.
	\param factor Optionally the scaling factor may also be set. Default value: 1.0 for linear, 10.0 for logarithmic.
	*/
	void SetAxisScale(HGraphAxisSelection axis, HGraphAxisScale scale, double factor=0.0);

	/*! 
	Retrieves the range of an axis.  

	\param axis The axis.
	\param min The minimal value of the axis. Returned to user. Passed by reference always
	\param max The maximal value of the axis. Returned to user. Passed by reference always
	*/
	void GetAxisRange(HGraphAxisSelection axis, double* min, double* max);

	/*! 
	Sets the range of an axis.  The minimal value must be less than or equal the maximal value.

	\param axis The axis.
	\param min The minimal value of the axis.
	\param max The maximal value of the axis.
	*/
	void SetAxisRange(HGraphAxisSelection axis, double min, double max);


	/*! 
	\param axis The axis.
	\return True if the given axis is visible and false if otherwise.
	*/
	bool GetAxisVisibility(HGraphAxisSelection axis);

	/*! 
	Set whether the specified axis is visible.

	\param axis The axis.
	\param vis Pass true to ensure that specified axis will be drawn and false if otherwise.
	*/
	void SetAxisVisibility(HGraphAxisSelection axis, bool vis);


	/*! 
	Retrieves the color used to draw an axis.

	\param axis The axis.
	\param color A string value for the color. Returned to user. Passed by reference always
	*/
	void GetAxisColor(HGraphAxisSelection axis, char * color);

	/*! 
	Sets the color used to draw an axis.

	\param axis The axis.
	\param color A string value for the color. Passed by reference always
	*/
	void SetAxisColor(HGraphAxisSelection axis, const char * color);


	/*! 
	Retrieves the line weight of an axis.

	\param axis The axis.
	\return The line weight.
	*/
	float GetAxisWeight(HGraphAxisSelection axis);

	/*! 
	Sets the line weight of an axis.

	\param axis The axis.
	\param weight The line weight.
	*/
	void SetAxisWeight(HGraphAxisSelection axis, float weight);


	/*! 
	Retrieves a value indicating how frequently regularly spaced ticks occur on the axis.

	\param axis The axis.
	\return The tick frequency.
	*/
	double GetAxisTickFrequency(HGraphAxisSelection axis);

	/*! 
	Sets how frequently regularly spaced ticks occur on the axis.

	\param axis The axis.
	\param freq The tick frequency.
	*/
	void SetAxisTickFrequency(HGraphAxisSelection axis, double freq);

	
	/*! 
	Gets the number of places after the decimal point that will be displayed.

	\param axis The axis.
	\return the number of places.
	*/
	int GetAxisPrecision(HGraphAxisSelection axis);
	
	/*! 
	Sets the number of places after the decimal point to display.

	\param axis The axis.
	\param precision the number of places to display.
	*/
	void SetAxisPrecision(HGraphAxisSelection axis, int precision);

	/*! 
	Retrieves the size of tick marks along the axis.

	\param axis The axis.
	\return The tick size.
	*/
	double GetAxisTickSize(HGraphAxisSelection axis);

	/*! 
	Sets the size of tick marks along the axis.

	\param axis The axis.
	\param size The tick size.
	*/
	void SetAxisTickSize(HGraphAxisSelection axis, double size);

	
	/*! 
	Retrieves the frequency of lines along an axis when the grid is drawn.

	\param axis The axis.
	\return The grid line frequency.
	*/	
	double GetAxisGridFrequency(HGraphAxisSelection axis);

	/*! 
	Sets the frequency of lines along an axis when the grid is drawn.

	\param axis The axis.
	\param freq The grid line frequency.
	*/	
	void SetAxisGridFrequency(HGraphAxisSelection axis, double freq);

	/*! 
	Retrieves the range of an axis.  

	\param axis The axis.
	\param min The minimal value of the grid on the axis. Returned to user. Passed by reference always
	\param max The maximal value of the grid on the axis. Returned to user. Passed by reference always
	*/
	void GetAxisGridRange(HGraphAxisSelection axis, double* min, double* max);

	/*! 
	Sets the range of an axis.  The minimal value must be less than or equal to the maximal value.

	\param axis The axis.
	\param min The minimal value of the grid on the axis.
	\param max The maximal value of the grid on the axis.
	*/
	void SetAxisGridRange(HGraphAxisSelection axis, double min, double max);

	/*! 
	\return True if the axis labels are visible and false if otherwise.
	\param axis The axis.
	*/		
	bool GetAxisLabelVisibility(HGraphAxisSelection axis);

	/*! 
	This method sets the visibilty for the labels at each tick on the given axis.

	\param axis The axis.
	\param vis The visibility of labels on the axis.
	*/		
	void SetAxisLabelVisibility(HGraphAxisSelection axis, bool vis);


	/*! 
	Retrieves the text font used for labels on an axis.

	\param axis The axis.
	\param font A string value for the font. Returned to user. Passed by reference always.
	*/
	void GetAxisLabelTextFont(HGraphAxisSelection axis, char * font);

	/*! 
	Sets the text font used for labels on an axis.

	\param axis The axis.
	\param font A string value for the font. Passed by reference always.
	*/
	void SetAxisLabelTextFont(HGraphAxisSelection axis, const char * font);


	/*! 
	Retrieves the text font used for labels on an axis.

	\param axis The axis.
	\param color A string value for the color. Returned to user. Passed by reference always.
	*/
	void GetAxisLabelTextColor(HGraphAxisSelection axis, char * color);

	/*! 
	Sets the text font used for labels on an axis.

	\param axis The axis.
	\param font A string value for the color. Passed by reference always.
	*/
	void SetAxisLabelTextColor(HGraphAxisSelection axis, const char * color);


	/*! 
	Sets the text path used for labels on an axis.

	\param axis The axis.
	\param xvector The x component of the text path vector.
	\param yvector The y component of the text path vector.
	\param zvector The z component of the text path vector.
	*/	
	void SetAxisLabelTextPath(HGraphAxisSelection axis, float xvector,	
		float yvector, float zvector);


	/*! 
	Sets the text path used for labels on an axis.

	\param axis The axis.
	\param xvector The x component of the text path vector.  Returned to user.
	\param yvector The y component of the text path vector.  Returned to user.
	\param zvector The z component of the text path vector.  Returned to user.
	*/	
	void GetAxisLabelTextPath(HGraphAxisSelection axis, float *xvector,	
		float *yvector, float *zvector);


	/*! 
	Adds a data set to the plot.

	\param points_count The number of points being passed.
	\param points An array of points in the data set. Passed by reference always.
	\param format The format of the points.  Default value: PointFormatCartesian.
	\param colors An array of (RGB) color values, one per point, that override the colors of the 
					data set.  If used, this should be the same size as points.  
					Passed by reference always.
	\return The ID of the data set.  The ID can be used later for modifying or removing the data set.
	*/
	int AddDataSet(int points_count, const HPoint points[], HGraphPointFormat format=PointFormatCartesian, 
					const HPoint colors[]=0);


	/*! 
	Removes a data set from the plot.

	\param data_set The ID of the data set to be removed.
	*/
	void RemoveDataSet(int data_set);


	/*! 
	Retrieves the number of points in a data set

	\param data_set The ID of the data set.
	\return The number of points.
	*/
	int GetDataSetSize(int data_set);


	/*! 
	Retrieves the point format for a data set.

	\param data_set The ID of the data set.
	\return The point format.
	*/
	HGraphPointFormat GetDataSetFormat(int data_set);

	
	/*! 
	Retrieves the array of points in a data set.  The points array should already be allocated and large enough to accommodate all the points in the set.  Use GetDataSetSize to get the size of a data set.
	
	\param data_set The ID of the data set.
	\param points An array of the points in the data set. Returned to user.  Passed by reference always.
	*/
	void GetDataSet(int data_set, HPoint points[]);
	
	/*! 
	Replaces the points in a data set.  
	
	\param data_set The ID of the data set.
	\param points_count The size of the points array.
	\param points An array of the points to be put into the data set. Passed by reference always.
	\param format The point format of points. Default value: PointFormatCartesian.
	\param colors An array of (RGB) color values, one per point, that override the colors of the 
					data set.  If used, this should be the same size as points.  
					Passed by reference always.
	*/
	void ReplaceDataSet(int data_set, int points_count, const HPoint points[],
						HGraphPointFormat format=PointFormatCartesian, const HPoint colors[]=0);

	
	/*! 
	Sets the color of points in a data set.
	
	\param data_set The ID of the data set.
	\param color A color string. Passed by reference always.
	*/
	void SetPointColor(int data_set, const char * color);

	/*! 
	Retrieves the color of points in a data set.
	
	\param data_set The ID of the data set.
	\param color A color string. Returned to user. Passed by reference always.
	*/
	void GetPointColor(int data_set, char * color);


	/*! 
	Sets the marker symbol for points in a data set.
	
	\param data_set The ID of the data set.
	\param symbol A string of special constants, see Set_Marker_Symbol(). Passed by reference always.
	*/
	void SetPointSymbol(int data_set, const char * symbol);

	/*! 
	Retrieves the marker symbol for points in a data set.
	
	\param data_set The ID of the data set.
	\param symbol A string of special constants, see Set_Marker_Symbol(). Returned to user. Passed by reference always.
	*/
	void GetPointSymbol(int data_set, char * symbol);


	/*! 
	Sets the size of points in a data set.
	
	\param data_set The ID of the data set.
	\param size The size of points in the data set
	*/
	void SetPointSize(int data_set, double size);

	/*! 
	Retrieves the size of points in a data set.
	
	\param data_set The ID of the data set.
	\return The size of points in the data set.
	*/
	float GetPointSize(int data_set);


	/*! 
	\param data_set The ID of the data set.
	\return True if points are drawn and false if otherwise.
	*/
	bool GetPointVisibility(int data_set);

	/*! 
	Use this method to determine if points should points be drawn.
	
	\param data_set The ID of the data set.
	\param vis True if the points in the given data set should be drawn and false if otherwise.
	*/
	void SetPointVisibility(int data_set, bool vis);

	/*! 	
	\param data_set The ID of the data set.
	\return True if points in a data set are connected by a polyline.
	*/
	bool GetLineVisibility(int data_set);

	/*! 
	Determines if points in a data set should be connected by a polyline.
	
	\param data_set The ID of the data set.
	\param vis True if points in the given data set should be connected by a polyline.
	*/
	void SetLineVisibility(int data_set, bool vis);


	/*! 
	Sets the line pattern used for lines in the data set.

	\param data_set The ID of the data set.
	\param pattern The line pattern used for the data set. Passed by reference always.
	*/
	void SetLinePattern(int data_set, const char * pattern);

	/*! 
	Retrieves the line pattern used for lines in the data set.

	\param data_set The ID of the data set.
	\param pattern The line pattern used for the data set. Returned to user. Passed by reference always.
	*/
	void GetLinePattern(int data_set, char * pattern);

	/*! 
	Sets the line color used for lines in the data set.

	\param data_set The ID of the data set.
	\param color The line color used for the data set. Passed by reference always.
	*/
	void SetLineColor(int data_set, const char * color);

	/*! 
	Retrieves the line color used for lines in the data set.

	\param data_set The ID of the data set.
	\param color The line color used for the data set. Returned to user. Passed by reference always.
	*/
	void GetLineColor(int data_set, char * color);

	/*! 
	Retrieves the line weight of a data set.

	\param axis The data set.
	\return The line weight.
	*/
	float GetLineWeight(int data_set);

	/*! 
	Sets the line weight of a data set.

	\param axis The data set.
	\param weight The line weight.
	*/
	void SetLineWeight(int data_set, float weight);

	/*! 	
	\param data_set The ID of the data set.
	\return True if the data set is interpreted as data points in a bar char. 
	*/
	bool GetBarVisibility(int data_set);

	/*! 
	Determines if points in a data set are interpreted as data points for a bar chart.
	
	\param data_set The ID of the data set.
	\param vis True if the data point are show in a bar chart
	*/
	void SetBarVisibility(int data_set, bool vis);

	
	/*! 
	Sets a color map for the bars in a data set.  Bars are colored in the order they appear in the data set point list.  When the color map is exhausted, colors wrap around to the beginning.
	
	\param data_set The ID of the data set.
	\param map A text string of all the colors in the color map. Passed by reference always.
	*/
	void SetBarColorMap(int data_set, const char * map);

	/*! 
	Retrieves the color map for the bars in a data set.  Bars are colored in the order they appear in the data set point list.  When the color map is exhausted, colors wrap around to the beginning.
	
	\param data_set The ID of the data set.
	\param map A text string of all the colors in the color map. Returned to user. Passed by reference always.
	*/
	void GetBarColorMap(int data_set, char * map);


	/*! 
	Sets a color map for the bars in a data set.  Bars are colored in the order they appear in the data set point list.  When the color map is exhausted, colors wrap around to the beginning.
	
	\param data_set The ID of the data set.
	\param count The number of colors in the values array.
	\param values An array of HPoints containing all the colors in the color map. Passed by reference always.
	\param color_space The color space that the values are in.  Default value: "RGB"
	*/
	void SetBarColorMapByValue(int data_set, int count, const HPoint values[], const char * color_space=0);
	
	/*! 
	Retrieves a color map for the bars in a data set.  Bars are colored in the order they appear in the data set point list.  When the color map is exhausted, colors wrap around to the beginning.
	
	\param data_set The ID of the data set.
	\param count The number of colors in the values array. Returned to user.  Passed by reference always.
	\param values An array of HPoints containing all the colors in the color map. Returned to user. Passed by reference always.
	\param color_space The color space that the values are in. Returned to user.  Passed by reference always.
	*/
	void GetBarColorMapByValue(int data_set, int * count, HPoint values[],  char * color_space);
	
	/*! 
	Retrieves the width of bars, centered on data points.
	
	\param data_set The ID of the data set.
	\return	The width of bars.
	*/
	double GetBarWidth(int data_set);

	/*! 
	Sets the width of bars.  The bars are centered on data points.
	
	\param data_set The ID of the data set.
	\param width The width of bars.
	*/
	void SetBarWidth(int data_set, double width);
	
	/*! 	
	\param data_set The ID of the data set.
	\return	True if bar edges are drawn.
	*/
	bool GetBarEdgeVisibility(int data_set);

	/*! 
	Sets a flag indicating where the bar edges will be visible.
	
	\param data_set The ID of the data set.
	\param vis Pass true for bar edge to be visibile and false if otherwise.
	*/
	void SetBarEdgeVisibility(int data_set, bool vis);

	
	/*! 
	\param data_set The ID of the data set.
	\return True if the data set is considered the boundary of a colored region.
	*/
	bool GetPolygonVisibility(int data_set);

	
	/*! 
	Sets whether a data set should be considered 
	                     the boundary of a colored region.
	
	\param data_set The ID of the data set.
	\param vis The polygon visibility.
	*/
	void SetPolygonVisibility(int data_set, bool vis);

	/*! 
	Returns the color that a polygonal region will 
					be drawn in if polygon visibility is on.
	
	\param data_set The ID of the data set.
	\param color The polygon color. Passed by reference always. Returned to user.
	*/
	void GetPolygonColor(int data_set, char * color);

	/*! 
	If polygon visibility is on, this sets the color of that polygon.
	
	\param data_set The ID of the data set.
	\param color The polygon color. Passed by reference always.
	*/
	void SetPolygonColor(int data_set, const char * color);

	/*!
	Force the segment tree to reflect any queued changes in the graph.
	*/
	virtual void Update();

	/*!
	Get the forced aspect ratio.
	*/
	double GetAspectRatio();

	/*! 
	Constrain graph to this aspect ratio given as horizontal/vertical.  To get a square, pass in 1.0.
	Aspect ratio will be maintained in this manner while underlying graph data is changed.

	\param ratio Forced aspect ratio (double precision).
	*/
	void SetAspectRatio(double ratio);

	/*!
	Remove the forced aspect ratio.
	*/
	void UnSetAspectRatio();

protected:
	HGraphPlotType m_plot_type;
	HGraphGridType m_grid_type;
	HGraphAxis m_x_axis, m_y_axis;
	double m_aspect_ratio;

	VArray< HGraphDataSetNode *> m_points;

	/*! 
	Draw the graph axes.
	*/
	virtual void DrawAxes();

	/*! 
	Draw the grid overlay for the graph.
	*/
	virtual void DrawGrid();

	/*! 
	Draw a frame around the graph.
	*/
	virtual void DrawFrame();

	/*! 
	Draw a data set.
	
	\param data_set the data set to be drawn or updated.
	*/
	virtual void DrawData(int data_set);

};

//! Provides simple pie chart creation tools
class MVO_API HPieChart: public HBaseGraph
{
public:

	/*!
	HPieChart Constructor.  

	\param plot_seg The segment that all graph subsegments will be created beneath.
	*/
	HPieChart(HC_KEY plot_seg);

	/*!
	~HPieChart Destructor.
	*/
	virtual ~HPieChart();

	/*! 
	Sets the title of the plot.

	\param title The title of the plot. Passed by reference always.
	*/
	void SetPlotTitle(const char * title);

	/*! 
	This method adds a unicode plot title or replaces the existing one.

	\param title The title of the plot.
	*/
	void SetPlotUnicodeTitle(unsigned short * title);

	
	/*! 
	Adds a slice to the pie chart with the given size.  Pie slice sizes are automatically tracked and updated.

	\param size The size of the pie slice.
	\return The ID of the pie slice.  Can be used to later modify or remove the slice.
	*/
	int AddPieSlice(double size);


	/*! 
	Removes (eats) a slice of the pie.

	\param slice_id The slice to be removed.
	*/
	void RemovePieSlice(int slice_id);

	/*! 
	Sets the color map to be used for the pie chart.  Pie slices are colored in the order they are added.

	\param map A text string containing all the colors in the pie chart color map. Passed by reference always.
	*/
	void SetPieColorMap(const char * map);

	/*! 
	Retrieves the color map to be used for the pie chart.  Pie slices are colored in the order they are added.

	\param map A text string containing all the colors in the pie chart color map. Returned to user. Passed by reference always.
	*/
	void GetPieColorMap(char * map);

	/*! 
	Sets the color map by value to be used for the pie chart.  Pie slices are colored in the order they are added.
	
	\param count The number of colors in the values array.
	\param values An array of HPoints containing all the colors in the color map. Passed by reference always.
	\param color_space The color space that the values are in.  Default value: "RGB"
	*/
	void SetPieColorMapByValue(int count, const HPoint values[], const char * color_space=0);

	/*! 
	Sets the color map by value to be used for the pie chart.  Pie slices are colored in the order they are added.
	
	\param count The number of colors in the values array. Returned to user. Passed by reference always.
	\param values An array of HPoints containing all the colors in the color map. Returned to user. Passed by reference always.
	\param color_space The color space that the values are in. Returned to user. Passed by reference always.
	*/
	void GetPieColorMapByValue(int * count, HPoint values[],  char * color_space);

	/*! 
	Labels a pie slice.
	
	\param slice_id The pie slice.
	\param str A text string containing the label of the pie slice. Passed by reference always.
	*/
	void AddPieSliceLabel(int slice_id, const char * str);

	/*! 
	Add a unicode label for the pie slice given by the slice ID.
	
	\param slice_id The pie slice.
	\param str A unicode string containing the label of the pie slice. Passed by reference always.
	*/
	void AddPieSliceUnicodeLabel(int slice_id, const unsigned short * str);

	/*! 
	Removes the label from a pie slice.
	
	\param slice_id The pie slice.
	*/
	void RemovePieSliceLabel(int slice_id);

	/*! 
	Retrieves the contents of a pie slice label.
	
	\param slice_id The pie slice.
	\param str A text string containing the pie slice label.
	*/
	void GetPieSliceLabelContents(int slice_id, char * str);

	/*! 
	Gets the pie slice label contents in unicode.  

	\param slice_id The pie slice.
	\param str A unicode string containing the pie slice label.
	*/
	void GetPieSliceUnicodeLabelContents(int slice_id, unsigned short * str);

	/*! 
	Sets the text font used to render a pie slice label.
	
	\param slice_id The pie slice.
	\param font A text string containing the text font. Passed by reference always.
	*/
	void SetPieSliceLabelTextFont(int slice_id, const char * font);

	/*! 
	Retrieves the text font used to render a pie slice label.
	
	\param slice_id The pie slice.
	\param font A text string containing the text font. Returned to user. Passed by reference always.
	*/
	void GetPieSliceLabelTextFont(int slice_id, char * font);
	
	
	/*! 
	Retrieves the flag that determines if pie edges are drawn.
	
	\return True if the edge visibility and false if otherwise.
	*/
	bool GetPieEdgeVisibility();

	/*! 
	Sets the option to draw borders on pie slices.
	
	\param vis If true, the edge are drawn.
	*/
	void SetPieEdgeVisibility(bool vis);
	
	
	/*! 
	Retrieves the color used for pie slice edges.
	
	\param color A text string containing the color used for pie slice edges. Returned to user. Passed by reference always.
	*/
	void GetPieEdgeColor(char * color);

	/*! 
	Sets the color used for pie slice edges.
	
	\param color A text string containing the color used for pie slice edges. Passed by reference always.
	*/
	void SetPieEdgeColor(const char * color);

	/*! 
	Gets the radius of the pie chart.  Default value: 1.0.

	\return The radius.
	*/
	double GetPieRadius();

	/*! 
	Sets a new radius for the pie chart.  

	\param radius The new radius.
	*/
	void SetPieRadius(double radius);

	/*! 
	Sets the location of the plot origin.

	\param origin An HPoint value representing the new location of the title of the plot.
	*/
	void SetPlotOrigin(HPoint origin);

	/*!
	Force the segment tree to reflect any queued changes in the graph.
	*/
	virtual void Update();

protected:
	VArray< HGraphPieSlice *> m_pie;
	double m_pie_total;
	double m_scale;

	/*! 
	Draws a frame around a graph.
	*/
	virtual void DrawFrame();

	/*! 
	Updates the pie chart geometry and labels.
	*/
	virtual void DrawPie();
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




































