// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HC_LEGACY_DEFINED
#define HC_LEGACY_DEFINED

/*** C++ STYLE COMMENTS PROHIBITED IN THIS FILE ***/

#include "hc.h"


/* OBSOLETE functions */

#define HC_Zero_Memory_Profile(which) (void)which

/* these three can server no useful purpose */
#define HC_PShow_Net_Streaming_Mode (key_count, path_keys, flag)
#define HC_QShow_Net_Streaming_Mode (segment_name, spec)
#define HC_Show_Net_Streaming_Mode (flag) 


#define HC_Flush_Segment(segment_name) HC_Flush_Geometry(segment_name)
#define HC_Clear_Display() HC_Flush_Geometry("?picture/...")

HC_EXTERNAL void HC_CDECL HC_Mark_As_Updated (char const *segment);
HC_EXTERNAL void HC_CDECL HC_Mark_As_Refresh_Needed (char const * segment, double left, double right, double bottom, double top);

HC_EXTERNAL void HC_CDECL HC_Show_Location_Status (int * status);
#define HC_Request_Location(x, y) HC_Get_Location (x, y)

#define HC_UnDefine_Color(name) HC_UnDefine_Color_Name(name)
#define HC_Define_Color(base, first, second, definition) HC_Define_Color_Name (base, first, second, definition);

#define HC_QSet_User_Option_By_Index(segment_name,index,value) HC_QSet_User_Index(segment_name,index,value)
#define HC_Set_User_Option_By_Index(index,value) HC_Set_User_Index(index,value)
#define HC_UnSet_User_Option_By_Index(index) HC_UnSet_One_User_Index(index)
#define HC_QUnSet_User_Option_By_Index(segment_name,index) HC_QUnSet_One_User_Index(segment_name,index)

#define HC_UnSet_Driver()	HC_Flush_Contents(".", "drivers")

#define HC_Show_NURBS_Curve_Count(key, degree, cp_count, knot_count) HC_Show_NURBS_Curve_Size(key, degree, cp_count, 0, knot_count)

#define HC_UnSet_Text_Size() HC_UnSet_One_Text_Font ("size")
HC_EXTERNAL void HC_CDECL HC_QUnSet_Text_Size (char const * segment_name);
HC_EXTERNAL void HC_CDECL HC_Show_Net_Text_Size (float *value);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Size (float	*value);
HC_EXTERNAL void HC_CDECL HC_QShow_Text_Size (char const * segment_name, float * value);
HC_EXTERNAL void HC_CDECL HC_QSet_Text_Size (char const * segment_name, double ivalue);
HC_EXTERNAL void HC_CDECL HC_Set_Text_Size (double ivalue);

/* Q functions */

HC_EXTERNAL void HC_CDECL HC_QSet_Bounding_Cuboid (char const * segment_name,HC_POINT const	* min,HC_POINT const	* max);
HC_EXTERNAL void HC_CDECL HC_QSet_Bounding_Sphere (char const * segment_name,HC_POINT const	* center,double radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_QShow_Bounding_Cuboid (char const * segname,HC_POINT * min,HC_POINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_QShow_Bounding_Sphere (char const * segname,HC_POINT * center,float * radius);
HC_EXTERNAL void HC_CDECL HC_QShow_Bounding_Info (char const * segment_name,char * list);
HC_EXTERNAL void HC_CDECL HC_QUnSet_Bounding_Volume (char const * segment_name);

HC_EXTERNAL void HC_CDECL HC_QSet_Callback (char const * segment_name, char const * list);
HC_EXTERNAL void HC_CDECL HC_QSet_Callback_With_Data (char const * segment_name, char const * list, void const * data);
HC_EXTERNAL void HC_CDECL HC_QShow_Callback (char const * segment_name, char * spec);
HC_EXTERNAL void HC_CDECL HC_QShow_Net_Callback (char const * segment_name, char * spec);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Callback (char const * segment_name, char const * callback_name, char * spec);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Callback_With_Data (char const * segment_name, char const * callback_name, char * spec, void *	data);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Callback (char const * segment_name, char const * callback_name, char *	spec);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Callback_WD (char const * segment_name, char const * callback_name, char * spec, void * data);
#define  HC_QShow_One_Net_Callback_With_Data(segment_name, callback_name, spec, data) \
	HC_QShow_One_Net_Callback_WD(segment_name, callback_name, spec, data)

HC_EXTERNAL void HC_CDECL HC_QSet_Text_Font (char const * segment_name, char const * list);
HC_EXTERNAL void HC_CDECL HC_QShow_Text_Font (char const * segment_name, char * result);
HC_EXTERNAL void HC_CDECL HC_QShow_Net_Text_Font (char const * segment_name, char * result);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Text_Font (char const * segment_name, char const * which, char * result);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Text_Font (char const * segment_name, char const * which, char * result);

HC_EXTERNAL void HC_CDECL HC_QSet_Visibility (const char * segment_name, const char * list);
HC_EXTERNAL void HC_CDECL HC_QShow_Visibility (const char * segment_name, char * list);
HC_EXTERNAL void HC_CDECL HC_QShow_Net_Visibility (const char * segment_name, char * list);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Visibility (const char * segment_name, const char * which, char * list);
HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Visibility (const char * segment_name, const char * which, char * list);


HC_EXTERNAL void HC_CDECL HC_QSet_Camera (
	char const *		segment_name,
	HC_POINT const	*	position,
	HC_POINT const	*	target,
	HC_POINT const	*	up_vector,
	double				width,
	double				height,
	char const *		proj);

HC_EXTERNAL void HC_CDECL HC_QSet_Camera_Near_Limit (
	char const *	segment_name,
	double			limit);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Camera_Near_Limit (
	char const *	segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Camera (
	char const *	segment_name);

HC_EXTERNAL void HC_CDECL HC_QSet_Camera_By_Volume (
	char const *	segment_name,
	char const *	proj,
	double			xmin,
	double			xmax,
	double			ymin,
	double			ymax);

HC_EXTERNAL void HC_CDECL HC_QSet_Camera_Field (
	char const *	segment_name,
	double			width,
	double			height);

HC_EXTERNAL void HC_CDECL HC_QSet_Camera_Position (
	char const *	segment_name,
	double			x,
	double			y,
	double			z);

HC_EXTERNAL void HC_CDECL HC_QSet_Camera_Projection (
	char const *	segment_name,
	char const *	proj);

HC_EXTERNAL void HC_CDECL HC_QSet_Camera_Target (
	char const *	segment_name,
	double			x,
	double			y,
	double			z);

HC_EXTERNAL void HC_CDECL HC_QSet_Camera_Up_Vector (
	char const *	segment_name,
	double			x,
	double			y,
	double			z);

HC_EXTERNAL void HC_CDECL HC_QShow_Camera (
	char const *	segment_name,
	HC_POINT *		position,
	HC_POINT *		target,
	HC_POINT *		up_vector,
	float *			width,
	float *			height,
	char *			proj);

HC_EXTERNAL void HC_CDECL HC_QShow_Camera_Field (
	char const *	segment_name,
	float *			width,
	float *			height);

HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_QShow_Camera_Near_Limit (
	char const *	segment_name,
	float *			limit);

HC_EXTERNAL void HC_CDECL HC_QShow_Camera_Position (
	char const *	segment_name,
	float *			x,
	float *			y,
	float *			z);

HC_EXTERNAL void HC_CDECL HC_QShow_Camera_Projection (
	char const *	segment_name,
	char *			proj);

HC_EXTERNAL void HC_CDECL HC_QShow_Camera_Target (
	char const *	segment_name,
	float *			x,
	float *			y,
	float *			z);

HC_EXTERNAL void HC_CDECL HC_QShow_Camera_Up_Vector (
	char const *	segment_name,
	float *			x,
	float *			y,
	float *			z);

HC_EXTERNAL void HC_CDECL HC_QShow_Camera_By_Volume (
	char const *	segment_name,
	char *			proj,
	float *			xmin,
	float *			xmax,
	float *			ymin,
	float *			ymax);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Camera (
	char const *	segment_name,
	HC_POINT *			position,
	HC_POINT *			target,
	HC_POINT *			up_vector,
	float *			width,
	float *			height,
	char *			proj);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Camera_Field (
	char const *	segment_name,
	float *			width,
	float *			height);

HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_QShow_Net_Camera_Near_Limit (
	char const *	segment_name,
	float *			limit);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Camera_Position (
	char const *	segment_name,
	float *			x,
	float *			y,
	float *			z);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Camera_Projection (
	char const *	segment_name,
	char *			proj);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Camera_Target (
	char const *	segment_name,
	float *			x,
	float *			y,
	float *			z);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Camera_Up_Vector (
	char const *	segment_name,
	float *			x,
	float *			y,
	float *			z);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Camera_By_Volume (
	char const *	segment_name,
	char *			proj,
	float *			xmin,
	float *			xmax,
	float *			ymin,
	float *			ymax);

HC_EXTERNAL void HC_CDECL HC_QZoom_Camera (
	char const *	segment_name,
	double			zoom);

HC_EXTERNAL void HC_CDECL HC_QRoll_Camera (
	char const *	segment_name,
	double			angle);

HC_EXTERNAL void HC_CDECL HC_QPan_Camera (
	char const *	segment_name,
	double			horiz,
	double			vert);

HC_EXTERNAL void HC_CDECL HC_QOrbit_Camera (
	char const *	segment_name,
	double			horiz,
	double			vert);

HC_EXTERNAL void HC_CDECL HC_QDolly_Camera (
	char const *	segment_name,
	double			x,
	double			y,
	double			z);

HC_EXTERNAL void HC_CDECL HC_QSet_Polygonal_Clip_Region (
	char const *		segment_name,
	int					count,
	HC_POINT const *	points,
	char const *		options);

HC_EXTERNAL void HC_CDECL HC_QShow_Clip_Region_Size (
	char const *		segment_name,
	int  *				loops,
	int  *				total,
	char *				spec); 

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Clip_Region (
	char const *		segment_name,
	int  *				loops,
	int  *				lengths,
	HC_POINT  *			points,
	char *				spec); 

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Clip_Region_Size (
	char const *		segment_name,
	int  *				loops,
	int  *				total,
	char *				spec);


HC_EXTERNAL void HC_CDECL HC_QSet_Color (
	char const		*segment_name,
	char const		*cname);

HC_EXTERNAL void HC_CDECL HC_QSet_Color_By_Index (
	char const		*segment_name,
	char const		*who,
	int				index);

HC_EXTERNAL void HC_CDECL HC_QSet_Color_By_Value (
	char const		*segment_name,
	char const		*who,
	char const		*type,
	double			a,
	double			b,
	double			c);

HC_EXTERNAL void HC_CDECL HC_QSet_Color_By_FIndex (
	char const		*segment_name,
	char const		*who,
	double			findex);

HC_EXTERNAL void HC_CDECL HC_QShow_Color (
	char const		*segment_name,
	char			*cname);

HC_EXTERNAL void HC_CDECL HC_QShow_Color_By_Index (
	char const		*segment_name,
	char			*type,
	int				*index);

HC_EXTERNAL void HC_CDECL HC_QShow_Color_By_Value (
	char const		*segment_name,
	char			*type,
	char			*style,
	float			*a,
	float			*b,
	float			*c);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Color (
	char const		*segment_name,
	char			*cname);


HC_EXTERNAL void HC_CDECL HC_QShow_One_Color_Map_By_Value (
	char const *		segment_name,
	int					offset,
	char *				style,
	float *				x,
	float *				y,
	float *				z);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Color_Map (
	char const *		segment_name,
	int					offset,
	char *				cnames);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Color_Map_By_Value (
	char const *		segment_name,
	char *				style,
	int *				cmcount,
	HC_POINT *			values);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Color_Map (
	char const *		segment_name,
	char *				cnames);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Color_Map_By_V (
	char const *		segment_name,
	int					offset,
	char *				style,
	float *				x,
	float *				y,
	float *				z);

#define  HC_QShow_One_Net_Color_Map_By_Value(string_in1, int_in2, string_out3, float_out4, float_out5, float_out6) \
	HC_QShow_One_Net_Color_Map_By_V(string_in1, int_in2, string_out3, float_out4, float_out5, float_out6)

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Color_Map (
	char const *		segment_name,
	int					offset,
	char *				cnames);

HC_EXTERNAL void HC_CDECL HC_QShow_Color_Map_Count (
	char const *		segment_name,
	int *				value);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Color_Map_Count (
	char const *		segment_name,
	int *				value);

HC_EXTERNAL void HC_CDECL HC_QShow_Color_Map_Length (
	char const *		segment_name,
	int *				value);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Color_Map_Length (
	char const *		segment_name,
	int *				value);

HC_EXTERNAL void HC_CDECL HC_QModify_Color_Map_By_Value (
	char const *			segment_name,
	int						offset,
	char const *			style,
	int						count,
	HC_POINT const *		values);

HC_EXTERNAL void HC_CDECL HC_QModify_Color_Map (
	char const *			segment_name,
	int						offset,
	char const *			cnames);

HC_EXTERNAL void HC_CDECL HC_QSet_Color_Map_By_Value (
	char const *			segment_name,
	char const *			style,
	int						count,
	HC_POINT const *		values); 

HC_EXTERNAL void HC_CDECL HC_QSet_Color_Map (
	char const *			segment_name,
	char const *			cnames); 

HC_EXTERNAL void HC_CDECL HC_QShow_Color_Map_By_Value (
	char const *			segment_name,
	char *					style,
	int *					cmcount,
	HC_POINT *				values);

HC_EXTERNAL void HC_CDECL HC_QShow_Color_Map (
	char const *			segment_name,
	char *					cnames); 


HC_EXTERNAL void HC_CDECL HC_QSet_Conditions (
	char const *		segment_name,
	char const *		options);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Conditions (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QShow_Conditions (
	char const				*segment_name,
	char					*options);

HC_EXTERNAL void HC_CDECL HC_QShow_Conditions_Length (
	char const				*segment_name,
	int						*length);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Conditions (
	char const				*segment_name,
	char					*options);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Conditions_Length (
	char const				*segment_name,
	int						*length);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Condition (
	char const				*segment_name,
	char const				*cname,
	char					*options);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Condition (
	char const				*segment_name,
	char const				*cname,
	char					*options);


HC_EXTERNAL void HC_CDECL HC_QSet_Driver_Options (
	char const *		segment_name,
	char const *		list);

HC_EXTERNAL void HC_CDECL HC_QShow_Driver_Options (
	char const *		segment_name,
	char *				options);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Driver_Options (
	char const *		segment_name,
	char *				options);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Driver_Option (
	char const *		segment_name,
	char const *		doname,
	char *				options);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Driver_Option (
	char const *		segment_name,
	char const *		doname,
	char *				options);

HC_EXTERNAL void HC_CDECL HC_QSet_Handedness (
	char const *		segment_name,
	char const *		spec);

HC_EXTERNAL void HC_CDECL HC_QShow_Handedness (
	char const *		segment_name,
	char *				spec);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Handedness (
	char const *		segment_name,
	char *				spec);


HC_EXTERNAL void HC_CDECL HC_QSet_Heuristics (
	char const *		segment_name,
	char const *		list);

HC_EXTERNAL void HC_CDECL HC_QShow_Heuristics (
	char const *		segment_name,
	char *				list);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Heuristics (
	char const *		segment_name,
	char *				list);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Heuristic (
	char const *		segment_name,
	char const *		hname,
	char *				list);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Heuristic (
	char const *		segment_name,
	char const *		hname,
	char *				list);



HC_EXTERNAL void HC_CDECL HC_QSet_Modelling_Matrix (
	char const *			segment_name,
	float const *			fbfp);

HC_EXTERNAL void HC_CDECL HC_QShow_Modelling_Matrix (
	char const *			segment_name,
	float *					fbfp);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Modelling_Matrix (
	char const *			segment_name,
	float *					fbfp);

HC_EXTERNAL void HC_CDECL HC_QTranslate_Object (
	char const *			segment_name,
	double					x,
	double					y,
	double					z);

HC_EXTERNAL void HC_CDECL HC_QScale_Object (
	char const *			segment_name,
	double					x,
	double					y,
	double					z);

HC_EXTERNAL void HC_CDECL HC_QRotate_Object_Offaxis (
	char const *			segment_name,
	double					x,
	double					y,
	double					z,
	double					angle);

HC_EXTERNAL void HC_CDECL HC_QRotate_Object (
	char const *			segment_name,
	double					x,
	double					y,
	double					z);

HC_EXTERNAL void HC_CDECL HC_QAppend_Modelling_Matrix (
	char const *			segment_name,
	float const *			fbfp);




HC_EXTERNAL void HC_CDECL HC_QSet_Face_Pattern (
	char const *			segment_name,
	char const *			pattern);

HC_EXTERNAL void HC_CDECL HC_QSet_Window_Pattern (
	char const *			segment_name,
	char const *			pattern);

HC_EXTERNAL void HC_CDECL HC_QSet_Text_Alignment (
	char const *			segment_name,
	char const *			pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Face_Pattern (
	char const *		segment_name,
	char *				pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Window_Pattern (
	char const *		segment_name,
	char *				pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Text_Alignment (
	char const *		segment_name,
	char *				pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Face_Pattern (
	char const *		segment_name,
	char *				pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Window_Pattern (
	char const *		segment_name,
	char *				pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Text_Alignment (
	char const *		segment_name,
	char *				pattern);



HC_EXTERNAL void HC_CDECL HC_QSet_Line_Weight (
	char const				*segment_name,
	double					value);

HC_EXTERNAL void HC_CDECL HC_QSet_Variable_Line_Weight (
	char const				*segment_name,
	char const *			options);

HC_EXTERNAL void HC_CDECL HC_QSet_Edge_Weight (
	char const				*segment_name,
	double					value);

HC_EXTERNAL void HC_CDECL HC_QSet_Variable_Edge_Weight (
	char const				*segment_name,
	char const *			options);

HC_EXTERNAL void HC_CDECL HC_QSet_Marker_Size (
	char const				*segment_name,
	double					value);

HC_EXTERNAL void HC_CDECL HC_QSet_Variable_Marker_Size (
	char const				*segment_name,
	char const *			options);

HC_EXTERNAL void HC_CDECL HC_QSet_Text_Spacing (
	char const				*segment_name,
	double					value);

HC_EXTERNAL void HC_CDECL HC_QShow_Edge_Weight (
	char const				*segment_name,
	float					*value);

HC_EXTERNAL void HC_CDECL HC_QShow_Variable_Edge_Weight (
	char const				*segment_name,
	char					*list);

HC_EXTERNAL void HC_CDECL HC_QShow_Line_Weight (
	char const				*segment_name,
	float					*value);

HC_EXTERNAL void HC_CDECL HC_QShow_Variable_Line_Weight (
	char const				*segment_name,
	char					*list);

HC_EXTERNAL void HC_CDECL HC_QShow_Marker_Size (
	char const				*segment_name,
	float					*value);

HC_EXTERNAL void HC_CDECL HC_QShow_Variable_Marker_Size (
	char const				*segment_name,
	char					*list);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Edge_Weight (
	char const				*segment_name,
	float					*value);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Var_Edge_Weight (
	char const				*segment_name,
	char					*list);
#define  HC_QShow_Net_Variable_Edge_Weight(string_in1, string_out2) \
	HC_QShow_Net_Var_Edge_Weight(string_in1, string_out2)

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Line_Weight (
	char const				*segment_name,
	float					*value);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Var_Line_Weight (
	char const				*segment_name,
	char					*list);
#define  HC_QShow_Net_Variable_Line_Weight(string_in1, string_out2) \
	HC_QShow_Net_Var_Line_Weight(string_in1, string_out2)

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Marker_Size (
	char const				*segment_name,
	float					*value);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Var_Marker_Size (
	char const				*segment_name,
	char					*list);
#define  HC_QShow_Net_Variable_Marker_Size(string_in1, string_out2) \
	HC_QShow_Net_Var_Marker_Size(string_in1, string_out2)

HC_EXTERNAL void HC_CDECL HC_QShow_Text_Spacing (
	char const				*segment_name,
	float					*value);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Text_Spacing (
	char const				*segment_name,
	float					*value);


HC_EXTERNAL void HC_CDECL HC_QSet_Priority (
	char const *		segment_name,
	long				priority);

HC_EXTERNAL void HC_CDECL HC_QShow_Priority_Range (
	char const *		segment_name,
	long *				low,
	long *				high);


HC_EXTERNAL void HC_CDECL HC_QSet_Rendering_Options (
	char const		*segment_name,
	char const		*list);

HC_EXTERNAL void HC_CDECL HC_QShow_Rendering_Options (
	char const *	segment_name,
	char *			list);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Rendering_Options (
	char const		*segment_name,
	char			*list);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Rendering_Option (
	char const				*segment_name,
	char const				*roname,
	char					*list);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Rendering_Opti  (
	char const				*segment_name,
	char const				*roname,
	char					*list);
#define  HC_QShow_One_Net_Rendering_Option(string_in1, string_in2, string_out3) \
	HC_QShow_One_Net_Rendering_Opti(string_in1, string_in2, string_out3)



HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Selectability (
	char const		*segment_name,
	char const		*selname,
	char			*list);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Selectability (
	char const		*segment_name,
	char const		*selname,
	char			*list);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Selectability (
	char const		*segment_name,
	char			*list);


HC_EXTERNAL void HC_CDECL HC_QShow_Selectability (
	char const		*segment_name,
	char			*list);

HC_EXTERNAL void HC_CDECL HC_QSet_Selectability (
	char const				*segment_name,
	char const				*list);



HC_EXTERNAL void HC_CDECL HC_QShow_Net_Line_Pattern (
	char const *				segment_name,
	char *						pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Edge_Pattern (
	char const *			segment_name,
	char *					pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Line_Pattern (
	char const *			segment_name,
	char *					pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Edge_Pattern (
	char const *			segment_name,
	char *					pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Marker_Symbol (
	char const *			segment_name,
	char *					pattern);

HC_EXTERNAL void HC_CDECL HC_QShow_Marker_Symbol (
	char const *			segment_name,
	char*					pattern);

HC_EXTERNAL void HC_CDECL HC_QSet_Edge_Pattern (
	char const *			segment_name,
	char const *			pattern);

HC_EXTERNAL void HC_CDECL HC_QSet_Line_Pattern (
	char const *		segment_name,
	char const *		pattern);

HC_EXTERNAL void HC_CDECL HC_QSet_Marker_Symbol (
	char const *	segment_name,
	char const *	pattern);



HC_EXTERNAL void HC_CDECL HC_QShow_Net_Explicit_Color (
	char const		*segment_name,
	char const		*type,
	char const		*channel,
	float 		*rgb,
	char 		*texture,
	char 		*options);

HC_EXTERNAL void HC_CDECL HC_QShow_Explicit_Color (
	char const		*segment_name,
	char const		*type,
	char const		*channel,
	float 		*rgb,
	char 		*texture,
	char 		*options);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Color_By_Value (
	char const		*segment_name,
	char const		*type,
	char			*style,
	float			*a,
	float			*b,
	float			*c);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Color_By_Index (
	char const		*segment_name,
	char const		*type,
	int				*index);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Color (
	char const		*segment_name,
	char const		*type,
	char			*cname);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Color_By_Value (
	char const		*segment_name,
	char const		*type,
	char			*style,
	float			*a,
	float			*b,
	float			*c);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Color_By_Index (
	char const		*segment_name,
	char const		*type,
	int				*index);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Color (
	char const		*segment_name,
	char const		*type,
	char			*cname);


HC_EXTERNAL void HC_CDECL HC_QSet_Streaming_Mode (
	char const *		segment_name,
	char const *		spec);

HC_EXTERNAL void HC_CDECL HC_QShow_Streaming_Mode (
	char const *		segment_name,
	char *				spec);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Text_Path (
	char const				*segment_name,
	float					*x,
	float					*y,
	float					*z);

HC_EXTERNAL void HC_CDECL HC_QShow_Text_Path (
	char const				*segment_name,
	float					*x,
	float					*y,
	float					*z);

HC_EXTERNAL void HC_CDECL HC_QSet_Text_Path (
	char const		*segment_name,
	double			x,
	double			y,
	double			z);




HC_EXTERNAL void HC_CDECL HC_QAppend_Texture_Matrix (
	char const *			segment_name,
	float const *			fbfp);

HC_EXTERNAL void HC_CDECL HC_QRotate_Texture_Offaxis (
	char const *			segment_name,
	double					x,
	double					y,
	double					z,
	double					angle);


HC_EXTERNAL void HC_CDECL HC_QRotate_Texture (
	char const *			segment_name,
	double					x,
	double					y,
	double					z);

HC_EXTERNAL void HC_CDECL HC_QScale_Texture (
	char const *			segment_name,
	double					x,
	double					y,
	double					z);

HC_EXTERNAL void HC_CDECL HC_QTranslate_Texture (
	char const *			segment_name,
	double					x,
	double					y,
	double					z);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Texture_Matrix (
	char const *			segment_name,
	float  *			fbfp);

HC_EXTERNAL void HC_CDECL HC_QShow_Texture_Matrix (
	char const *			segment_name,
	float  *			fbfp);

HC_EXTERNAL void HC_CDECL HC_QSet_Texture_Matrix (
	char const *			segment_name,
	float const *			fbfp);



HC_EXTERNAL void HC_CDECL HC_QUnSet_Window_Frame (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Handedness (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Rendering_Options (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Heuristics (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Window (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Unicode_Options (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_User_Options (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_User_Value (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Driver_Options (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Text_Path (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Streaming_Mode (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Text_Spacing (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Marker_Size (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Edge_Weight (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Line_Weight (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Text_Font (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Text_Alignment (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Marker_Symbol (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Edge_Pattern (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Line_Pattern (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Window_Pattern (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Face_Pattern (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Texture_Matrix (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Modelling_Matrix (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Color_Map (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Clip_Region (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Callback (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Selectability (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Color (
	char const *			segment_name);

HC_EXTERNAL void HC_CDECL HC_QUnSet_Visibility (
	char const *			segment_name);




HC_EXTERNAL void HC_CDECL HC_QUnSet_One_User_Data (
	char const *		segment_name,
	HC_POINTER_SIZED_INT	index);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Visibility (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Callback (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Condition (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_User_Index (
	char const *		segment_name,
	long				index);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Unicode_Option (
	char const *		segment_name,
	unsigned short const *	which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_User_Option (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Text_Font (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Selectability (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Rendering_Option (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Color (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Heuristic (
	char const *		segment_name,
	char const *		which);

HC_EXTERNAL void HC_CDECL HC_QUnSet_One_Driver_Option (
	char const *		segment_name,
	char const *		which);





HC_EXTERNAL long HC_CDECL HC_QShow_One_Net_User_Data (
	char const *			segment_name,
	HC_POINTER_SIZED_INT	index,
	void *					data,
	long 					data_length);

HC_EXTERNAL long HC_CDECL HC_QShow_User_Data_Indices (
	char const *			segment_name,
	HC_POINTER_SIZED_INT *	data_indices, 
	long					data_indices_count);

HC_EXTERNAL long HC_CDECL HC_QShow_One_User_Data (
	char const *			segment_name,
	HC_POINTER_SIZED_INT	index,
	void *					data,
	long 					data_size);

HC_EXTERNAL void HC_CDECL HC_QSet_User_Data (
	char const *			segment_name,
	HC_POINTER_SIZED_INT	index,
	void const *			data,
	long					data_length);

HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_QShow_One_Net_User_Index (
	char const *		segment_name,
	long				index,
	void *				value);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_User_Indices (
	char const *		segment_name,
	int *				uicount,
	long *				indices,
	void *				values);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_User_Index_Count (
	char const *		segment_name,
	int *				uicount);

HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_QShow_One_User_Index (
	char const *			segment_name,
	long					index,
	void *					value);

HC_EXTERNAL void HC_CDECL HC_QShow_User_Indices (
	char const *		segment_name,
	int *				uicount,
	long *				indices,
	void *				values);

HC_EXTERNAL void HC_CDECL HC_QShow_User_Index_Count (
	char const *			segment_name,
	int *					uicount);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Uni_Opt_Len (
	char const *			segment_name,
	unsigned short const *	uoname,
	int *					length);
#define  HC_QShow_One_Net_Unicode_Option_Length(string_in1, unsigned_short_in2, int_out3) \
	HC_QShow_One_Net_Uni_Opt_Len(string_in1, unsigned_short_in2, int_out3)

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Unicode_Opt (
	char const *			segment_name,
	unsigned short const *	uoname,
	unsigned short *		options);
#define  HC_QShow_One_Net_Unicode_Option(string_in1, unsigned_short_in2, unsigned_short_out3) \
	HC_QShow_One_Net_Unicode_Opt(string_in1, unsigned_short_in2, unsigned_short_out3)

HC_EXTERNAL void HC_CDECL HC_QShow_One_Uni_Option_Length (
	char const *				segment_name,
	unsigned short const *		uoname,
	int *						length);
#define  HC_QShow_One_Unicode_Option_Length(string_in1, unsigned_short_in2, int_out3) \
	HC_QShow_One_Uni_Option_Length(string_in1, unsigned_short_in2, int_out3)

HC_EXTERNAL void HC_CDECL HC_QShow_One_Unicode_Option (
	char const *				segment_name,
	unsigned short const *		uoname,
	unsigned short *			options);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Unicode_Opt_Length (
	char const *			segment_name,
	int *					length);
#define  HC_QShow_Net_Unicode_Options_Length(string_in1, int_out2) \
	HC_QShow_Net_Unicode_Opt_Length(string_in1, int_out2)

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Unicode_Options (
	char const *			segment_name,
	unsigned short *		options);

HC_EXTERNAL void HC_CDECL HC_QShow_Unicode_Options_Length (
	char const *			segment_name,
	int *					length);

HC_EXTERNAL void HC_CDECL HC_QShow_Unicode_Options (
	char const *			segment_name,
	unsigned short *		options);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_User_Opt_Len (
	char const *		segment_name,
	char const *		uoname,
	int *				length);
#define  HC_QShow_One_Net_User_Option_Length(string_in1, string_in2, int_out3) \
	HC_QShow_One_Net_User_Opt_Len(string_in1, string_in2, int_out3)

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_User_Option (
	char const *			segment_name,
	char const *			uoname,
	char *					options);

HC_EXTERNAL void HC_CDECL HC_QShow_One_User_Option_Length (
	char const *		segment_name,
	char const *		uoname,
	int *				length);

HC_EXTERNAL void HC_CDECL HC_QShow_One_User_Option (
	char const *			segment_name,
	char const *			uoname,
	char *					options);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_User_Opt_Length (
	char const *			segment_name,
	int *					length);
#define  HC_QShow_Net_User_Optionms_Length(string_in1, int_out2) \
	HC_QShow_Net_User_Opt_Length(string_in1, int_out2)

HC_EXTERNAL void HC_CDECL HC_QShow_Net_User_Options (
	char const *			segment_name,
	char *					options);

HC_EXTERNAL void HC_CDECL HC_QShow_User_Options_Length (
	char const *		segment_name,
	int *				length);

HC_EXTERNAL void HC_CDECL HC_QShow_User_Options (
	char const *		segment_name,
	char *				options);

HC_EXTERNAL void HC_CDECL HC_QSet_User_Index (
	char const *			segment_name,
	long					index,
	void const *			value);

HC_EXTERNAL void HC_CDECL HC_QSet_Unicode_Options (
	char const *				segment_name,
	unsigned short const *		options);

HC_EXTERNAL void HC_CDECL HC_QSet_User_Options (
	char const *				segment_name,
	char const *				options);


HC_EXTERNAL void HC_CDECL HC_QSet_User_Value (
	char const *			segment_name,
	HC_POINTER_SIZED_INT	data);

HC_EXTERNAL void HC_CDECL HC_QShow_User_Value (
	char const				*segment_name,
	HC_POINTER_SIZED_INT	*data);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_User_Value (
	char const				*segment_name,
	HC_POINTER_SIZED_INT	*data);



HC_EXTERNAL void HC_CDECL HC_QSet_Window (
	char const		*segment_name,
	double			left,
	double			right,
	double			bottom,
	double			top);

HC_EXTERNAL void HC_CDECL HC_QShow_Window (
	char const		*segment_name,
	float			*left,
	float			*right,
	float			*bottom,
	float			*top);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Window (
	char const		*segment_name,
	float			*left,
	float			*right,
	float			*bottom,
	float			*top);


HC_EXTERNAL void HC_CDECL HC_QSet_Window_Frame (
	char const				*segment_name,
	char const				*spec);

HC_EXTERNAL void HC_CDECL HC_QShow_Window_Frame (
	char const				*segment_name,
	char					*spec);

HC_EXTERNAL void HC_CDECL HC_QShow_Net_Window_Frame (
	char const				*segment_name,
	char					*spec);


HC_EXTERNAL void HC_CDECL HC_QShow_Net_Texture (
	char const * segment_name,
	char const	*name,
	char		*output,
	HC_KEY		*out_image);

HC_EXTERNAL void HC_CDECL HC_QShow_One_Net_Texture (
	char const * segment_name,
	char const	*name,
	char const * opt,
	char		*output);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Circle_By_Radius (
	char const *			segment_name,
	HC_POINT const *		center,
	double					radius_in,
	HC_POINT const *		normal_in);



HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Circle (
	char const *			segment_name,
	HC_POINT const *		start,
	HC_POINT const *		middle,
	HC_POINT const *		end);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Circular_Arc (
	char const *			segment_name,
	HC_POINT const *		start,
	HC_POINT const *		middle,
	HC_POINT const *		end);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Circular_Chord (
	char const *			segment_name,
	HC_POINT const *		start,
	HC_POINT const *		middle,
	HC_POINT const *		end);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Circular_Wedge (
	char const *			segment_name,
	HC_POINT const *		start,
	HC_POINT const *		middle,
	HC_POINT const *		end);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_NURBS_Curve (
	char const *			segment_name,
	int						degree,
	int						control_point_count,
	HC_POINT const *		control_points, 
	float const *			weights,
	float const *			knots,
	double					start_u,
	double					end_u);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Cutting_Plane (
	char const *	segment_name,
	double			a,
	double			b,
	double			c,
	double			d);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Cutting_Section (
	char const *		segment_name,
	int					count,
	HC_PLANE const	*	planes);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Cylinder (
	char const *			segment_name,
	HC_POINT const *		first,
	HC_POINT const *		second,
	double					radius,
	char const *			list);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_PolyCylinder (
	char const *			segment_name,
	int						point_count,
	HC_POINT const *		points,
	int						radius_count,
	float const *			radii,
	char const *			list);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Ellipse (
	char const *			segment_name,
	HC_POINT const *		center,
	HC_POINT const *		major,
	HC_POINT const *		minor);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Elliptical_Arc (
	char const *			segment_name,
	HC_POINT const *		center,
	HC_POINT const *		major,
	HC_POINT const *		minor,
	double					start,
	double					end);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Grid (
	char const *			segment_name,
	char const *			type,
	HC_POINT const *		origin,
	HC_POINT const *		ref1,
	HC_POINT const *		ref2,
	int						count1,
	int						count2); 


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Image (
	char const *				segment_name_,
	double						x,
	double						y,
	double						z,
	char const *				format,
	int							width,
	int							height,
	void const *				bytes);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Compressed_Image (
	char const *				segment_name_,
	double						x,
	double						y,
	double						z,
	char const *				format,
	int							width,
	int							height,
	int							size,
	void *						bytes);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Image_By_Ref (
	char const *				segment_name_,
	double						x,
	double						y,
	double						z,
	char const *				format,
	int							width,
	int							height,
	void const *				bytes);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Infinite_Line (
	char const *			segment_name,
	double					xa,
	double					ya,
	double					za,
	double					xb,
	double					yb,
	double					zb);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Infinite_Ray (
	char const *			segment_name,
	double					xa,
	double					ya,
	double					za,
	double					xb,
	double					yb,
	double					zb);



HC_EXTERNAL void HC_CDECL HC_QRollback_Ink (
	char const *		segment_name,
	int					count);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Ink (
	char const *		segment_name,
	double				x,
	double				y,
	double				z);

HC_EXTERNAL void HC_CDECL HC_QRestart_Ink (
	char const *		segment_name);



HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Distant_Light (
	char const		*segment_name,
	double			x,
	double			y,
	double			z);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Area_Light (
	char const *		segment_name,
	int					count,
	HC_POINT const *	points,
	char const *		list);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Spot_Light (
	char const *	segment_name,
	HC_POINT const	*	position,
	HC_POINT const	*	target,
	char const *	list);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Local_Light (
	char const		*segment_name,
	double			x,
	double			y,
	double			z);



HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Line (
	char const *			segment_name,
	double					xa,
	double					ya,
	double					za,
	double					xb,
	double					yb,
	double					zb);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Marker (
	char const *	segment_name,
	double			x,
	double			y,
	double			z);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Mesh (
	char const *		segment_name,
	int					rows,
	int					columns,
	HC_POINT const	*	points);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Mesh_By_Ref (
	char const *		segment_name,
	int					rows,
	int					columns,
	HC_POINT const	*	points);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_NURBS_Surface (
	char const *			segment_name,
	int						u_degree,
	int						v_degree,
	int						n_u_vertices,
	int						n_v_vertices,
	HC_POINT const *		control_points, 
	float const *			weights,
	float const *			u_knots,
	float const *			v_knots);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Polygon (
	char const *			segment_name,
	int						count,
	HC_POINT const	*		points);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Polyline (
	char const *		segment_name,
	int					count,
	HC_POINT const	*	points); 


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Shell_From_Region (
	char const *	segment_name,
	HC_KEY			key,
	int				region,
	char const *	options);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Sphere (
	char const *		segment_name,
	HC_POINT const *	center,
	double				radius_in,
	HC_POINT const *	axis_in,
	HC_POINT const *	ortho_in);




HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Text (
	char const *	segment_name,
	double			x,
	double			y,
	double			z,
	char const *	string);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Text_With_Encoding (
	char const *		segment_name,
	double				x,
	double				y,
	double				z,
	char const *		encoding,
	void const *		string);

HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Unicode_Text (
	char const *			segment_name,
	double					x,
	double					y,
	double					z,
	unsigned short const *	shorts);



HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Shell_By_Tristrips (
	char const *			segment_name,
	int						point_count,
	HC_POINT const	*		points,
	int						tristrips_list_length,
	int const *				tristrips_list,
	int						tristrips_faces_length,
	int const *				tristrips_faces);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Shell (
	char const *			segment_name,
	int						point_count,
	HC_POINT const	*		points,
	int						face_list_length,
	int const *				face_list);


HC_EXTERNAL HC_KEY HC_CDECL HC_QInsert_Shell_By_Ref (
	char const *			segment_name,
	int						point_count,
	HC_POINT const	*		points,
	int						face_list_length,
	int const*				face_list);





/* K functions */

#define HC_KInsert_Circle(start_,middle_,end_) \
	HC_Insert_Circle ((start_),(middle_),(end_))

#define HC_KInsert_Circle_By_Radius(center_,radius_in_,normal_in_) \
	HC_Insert_Circle_By_Radius ((center_),(radius_in_),(normal_in_))

#define HC_QKInsert_Circle_By_Radius(segment_name_,center_,radius_in_,normal_in_) \
	HC_QInsert_Circle_By_Radius ((segment_name_),(center_),(radius_in_),(normal_in_))

#define HC_QKInsert_Circle(segment_name_,start_,middle_,end_) \
	HC_QInsert_Circle ((segment_name_),(start_),(middle_),(end_))


#define HC_QKInsert_Circular_Arc(segment_name_,start_,middle_,end_) \
	HC_QInsert_Circular_Arc ((segment_name_),(start_),(middle_),(end_))

#define HC_QKInsert_Circular_Chord(segment_name_,start_,middle_,end_) \
	HC_QInsert_Circular_Chord ((segment_name_),(start_),(middle_),(end_))

#define HC_QKInsert_Circular_Wedge(segment_name_,start_,middle_,end_) \
	HC_QInsert_Circular_Wedge ((segment_name_),(start_),(middle_),(end_))

#define HC_KInsert_Circular_Arc(start_,middle_,end_) \
	HC_Insert_Circular_Arc ((start_),(middle_),(end_))

#define HC_KInsert_Circular_Chord(start_,middle_,end_) \
	HC_Insert_Circular_Chord ((start_),(middle_),(end_))

#define HC_KInsert_Circular_Wedge(start_,middle_,end_) \
	HC_Insert_Circular_Wedge ((start_),(middle_),(end_))


#define HC_QKInsert_NURBS_Curve(segment_name_,degree_,control_point_count_, \
	control_points_, weights_,knots_,start_u_,end_u_) \
	HC_QInsert_NURBS_Curve((segment_name_),(degree_),(control_point_count_), \
	(control_points_),(weights_),(knots_),(start_u_),(end_u_))

#define HC_KInsert_NURBS_Curve(degree_,control_point_count_, \
	control_points_, weights_,knots_,start_u_,end_u_) \
	HC_Insert_NURBS_Curve((degree_),(control_point_count_), \
		(control_points_),(weights_),(knots_),(start_u_),(end_u_))


#define HC_QKInsert_Cutting_Plane(segment_,a_,b_,c_,d_) \
	HC_QInsert_Cutting_Plane ((segment_),(a_),(b_),(c_),(d_)) 

#define HC_QKInsert_Cutting_Section(segment_,count_,planes_) \
	HC_QInsert_Cutting_Section ((segment_),(count_),(planes_)) 

#define HC_KInsert_Cutting_Plane(a_,b_,c_,d_) \
	HC_Insert_Cutting_Plane ((a_),(b_),(c_),(d_))

#define HC_KInsert_Cutting_Section(count_,planes_) \
	HC_Insert_Cutting_Section ((count_),(planes_))


#define HC_QKInsert_Cylinder(segment_name_,first_,second_,radius_,list_) \
	HC_QInsert_Cylinder((segment_name_),(first_),(second_),(radius_),(list_))

#define HC_QKInsert_PolyCylinder(segment_name_,point_count_,points_,radius_count_,radii_,list_) \
	HC_QInsert_PolyCylinder((segment_name_),(point_count_),(points_),(radius_count_),(radii_),(list_))

#define HC_KInsert_Cylinder(first_,second_,radius_,list_) \
	HC_Insert_Cylinder((first_),(second_),(radius_),(list_))

#define HC_KInsert_PolyCylinder(point_count_,points_,radius_count_,radii_,list_) \
	HC_Insert_PolyCylinder((point_count_),(points_),(radius_count_),(radii_),(list_))


#define HC_QKInsert_Ellipse(segment_name_,center_,major_,minor_) \
	HC_QInsert_Ellipse((segment_name_),(center_),(major_),(minor_))

#define HC_KInsert_Ellipse(center_,major_,minor_) \
	HC_Insert_Ellipse((center_),(major_),(minor_))


#define HC_QKInsert_Elliptical_Arc(segment_name_,center_,major_,minor_,start_,end_) \
	HC_QInsert_Elliptical_Arc((segment_name_),(center_),(major_),(minor_),(start_),(end_))

#define HC_KInsert_Elliptical_Arc(center_,major_,minor_,start_,end_) \
	HC_Insert_Elliptical_Arc((center_),(major_),(minor_),(start_),(end_))


#define HC_QKInsert_Grid(segment_name_,type_,origin_,ref1_,ref2_,count1_,count2_) \
	HC_QInsert_Grid((segment_name_),(type_),(origin_),(ref1_),(ref2_),(count1_),(count2_))

#define HC_KInsert_Grid(type_,origin_,ref1_,ref2_,count1_,count2_) \
	HC_Insert_Grid((type_),(origin_),(ref1_),(ref2_),(count1_),(count2_))


#define HC_QKInsert_Image(segment_name_,x_,y_,z_,format_,width_,height_,bytes_) \
	HC_QInsert_Image((segment_name_),(x_),(y_),(z_),(format_),(width_),(height_),(bytes_))

#define HC_QKInsert_Compressed_Image(segment_name_,x_,y_,z_,format_,width_,height_, size_, bytes_) \
	HC_QInsert_Compressed_Image((segment_name_),(x_),(y_),(z_),(format_),(width_),(height_),(size_),(bytes_))

#define HC_QKInsert_Image_By_Ref(segment_name_,x_,y_,z_,format_,width_,height_,bytes_) \
	HC_QInsert_Image_By_Ref((segment_name_),(x_),(y_),(z_),(format_),(width_),(height_),(bytes_))


#define HC_KInsert_Image(x_,y_,z_,format_,width_,height_,bytes_) \
	HC_Insert_Image((x_),(y_),(z_),(format_),(width_),(height_),(bytes_))

#define HC_KInsert_Compressed_Image(x_,y_,z_,format_,width_,height_, size_, bytes_) \
	HC_Insert_Compressed_Image((x_),(y_),(z_),(format_),(width_),(height_),(size_),(bytes_))

#define HC_KInsert_Image_By_Ref(x_,y_,z_,format_,width_,height_,bytes_) \
	HC_Insert_Image_By_Ref((x_),(y_),(z_),(format_),(width_),(height_),(bytes_))


#define HC_QKInsert_Infinite_Line(segment_name_,xa_,ya_,za_,xb_,yb_,zb_) \
	HC_QInsert_Infinite_Line((segment_name_),(xa_),(ya_),(za_),(xb_),(yb_),(zb_))

#define HC_QKInsert_Infinite_Ray(segment_name_,xa_,ya_,za_,xb_,yb_,zb_) \
	HC_QInsert_Infinite_Ray((segment_name_),(xa_),(ya_),(za_),(xb_),(yb_),(zb_))

#define HC_KInsert_Infinite_Line(xa_,ya_,za_,xb_,yb_,zb_) \
	HC_Insert_Infinite_Line((xa_),(ya_),(za_),(xb_),(yb_),(zb_))

#define HC_KInsert_Infinite_Ray(xa_,ya_,za_,xb_,yb_,zb_) \
	HC_Insert_Infinite_Ray((xa_),(ya_),(za_),(xb_),(yb_),(zb_))


#define HC_QKInsert_Ink(segment_name_,x_,y_,z_) \
	HC_QInsert_Ink((segment_name_),(x_),(y_),(z_))

#define HC_KInsert_Ink(x_,y_,z_) \
	HC_Insert_Ink((x_),(y_),(z_))

#define HC_QKInsert_Distant_Light(segment_name_,x_,y_,z_) \
	HC_QInsert_Distant_Light((segment_name_),(x_),(y_),(z_))

#define HC_QKInsert_Area_Light(segment_name_,count_,points_,list_) \
	HC_QInsert_Area_Light((segment_name_),(count_),(points_),(list_))

#define HC_QKInsert_Spot_Light(segment_name_,position_,target_,list_) \
	HC_QInsert_Spot_Light((segment_name_),(position_),(target_),(list_))

#define HC_QKInsert_Local_Light(segment_name_,x_,y_,z_) \
	HC_QInsert_Local_Light((segment_name_),(x_),(y_),(z_))

#define HC_KInsert_Distant_Light(x_,y_,z_) \
	HC_Insert_Distant_Light((x_),(y_),(z_))

#define HC_KInsert_Area_Light(count_,points_,list_) \
	HC_Insert_Area_Light((count_),(points_),(list_))

#define HC_KInsert_Spot_Light(position_,target_,list_) \
	HC_Insert_Spot_Light((position_),(target_),(list_))

#define HC_KInsert_Local_Light(x_,y_,z_) \
	HC_Insert_Local_Light((x_),(y_),(z_))


#define HC_QKInsert_Line(segment_name_,xa_,ya_,za_,xb_,yb_,zb_) \
	HC_QInsert_Line((segment_name_),(xa_),(ya_),(za_),(xb_),(yb_),(zb_))

#define HC_KInsert_Line(xa_,ya_,za_,xb_,yb_,zb_) \
	HC_Insert_Line((xa_),(ya_),(za_),(xb_),(yb_),(zb_))


#define HC_QKInsert_Marker(segment_name_,x_,y_,z_) \
	HC_QInsert_Marker((segment_name_),(x_),(y_),(z_))

#define HC_KInsert_Marker(x_,y_,z_) \
	HC_Insert_Marker((x_),(y_),(z_))



#define HC_QKInsert_Mesh(segment_name_,rows_,columns_,points_) \
	HC_QInsert_Mesh((segment_name_),(rows_),(columns_),(points_))

#define HC_QKInsert_Mesh_By_Ref(segment_name_,rows_,columns_,points_) \
	HC_QInsert_Mesh_By_Ref((segment_name_),(rows_),(columns_),(points_))

#define HC_KInsert_Mesh(rows_,columns_,points_) \
	HC_Insert_Mesh((rows_),(columns_),(points_))

#define HC_KInsert_Mesh_By_Ref(rows_,columns_,points_) \
	HC_Insert_Mesh_By_Ref((rows_),(columns_),(points_))


#define HC_QKInsert_NURBS_Surface(segment_name_,u_degree_,v_degree_, \
			n_u_vertices_,n_v_vertices_,control_points_,weights_,u_knots_,v_knots_) \
	HC_QInsert_NURBS_Surface((segment_name_),(u_degree_),(v_degree_), \
		(n_u_vertices_),(n_v_vertices_),(control_points_),(weights_),(u_knots_),(v_knots_))

#define HC_KInsert_NURBS_Surface(u_degree_,v_degree_, \
			n_u_vertices_,n_v_vertices_,control_points_,weights_,u_knots_,v_knots_) \
	HC_Insert_NURBS_Surface((u_degree_),(v_degree_), \
		(n_u_vertices_),(n_v_vertices_),(control_points_),(weights_),(u_knots_),(v_knots_))



#define HC_QKInsert_Polygon(segment_name_,count_,points_) \
	HC_QInsert_Polygon((segment_name_),(count_),(points_))

#define HC_KInsert_Polygon(count_,points_) \
	HC_Insert_Polygon((count_),(points_))


#define HC_QKInsert_Polyline(segment_name_,count_,points_) \
	HC_QInsert_Polyline((segment_name_),(count_),(points_))

#define HC_KInsert_Polyline(count_,points_) \
	HC_Insert_Polyline((count_),(points_))

#define HC_QKInsert_Polyline(segment_name_,count_,points_) \
	HC_QInsert_Polyline((segment_name_),(count_),(points_))



#define HC_QKInsert_Shell_From_Region(segment_name_,key_,region_,options_) \
	HC_QInsert_Shell_From_Region((segment_name_),(key_),(region_),(options_))

#define HC_KInsert_Shell_From_Region(key_,region_,options_) \
	HC_Insert_Shell_From_Region((key_),(region_),(options_))


#define HC_QKInsert_Sphere(segment_name_,center_,radius_in_,axis_in_,ortho_in_) \
	HC_QInsert_Sphere((segment_name_),(center_),(radius_in_),(axis_in_),(ortho_in_))

#define HC_KInsert_Sphere(center_,radius_in_,axis_in_,ortho_in_) \
	HC_Insert_Sphere((center_),(radius_in_),(axis_in_),(ortho_in_))



#define HC_KInsert_String_Cursor(ownerkey_,row_,column_) \
	HC_Insert_String_Cursor((ownerkey_),(row_),(column_))



#define HC_QKInsert_Text(segment_name_,x_,y_,z_,string_) \
	HC_QInsert_Text((segment_name_),(x_),(y_),(z_),(string_))

#define HC_QKInsert_Text_With_Encoding(segment_name_,x_,y_,z_,encoding_,string_) \
	HC_QInsert_Text_With_Encoding((segment_name_),(x_),(y_),(z_),(encoding_),(string_))

#define HC_QKInsert_Unicode_Text(segment_name_,x_,y_,z_,shorts_) \
	HC_QInsert_Unicode_Text((segment_name_),(x_),(y_),(z_),(shorts_))

#define HC_KInsert_Text(x_,y_,z_,string_) \
	HC_Insert_Text((x_),(y_),(z_),(string_))

#define HC_KInsert_Text_With_Encoding(x_,y_,z_,encoding_,string_) \
	HC_Insert_Text_With_Encoding((x_),(y_),(z_),(encoding_),(string_))

#define HC_KInsert_Unicode_Text(x_,y_,z_,shorts_) \
	HC_Insert_Unicode_Text((x_),(y_),(z_),(shorts_))



#define HC_QKInsert_Shell_By_Tristrips(segment_name_,point_count_,points_, \
		tristrips_list_length_,tristrips_list_,tristrips_faces_length_,tristrips_faces_) \
	HC_QInsert_Shell_By_Tristrips((segment_name_),(point_count_),(points_), \
		(tristrips_list_length_),(tristrips_list_),(tristrips_faces_length_),(tristrips_faces_))

#define HC_QKInsert_Shell(segment_name_,point_count_,points_,face_list_length_,face_list_) \
	HC_QInsert_Shell((segment_name_),(point_count_),(points_),(face_list_length_),(face_list_))

#define HC_QKInsert_Shell_By_Ref(segment_name_,point_count_,points_,face_list_length_,face_list_) \
	HC_QInsert_Shell_By_Ref((segment_name_),(point_count_),(points_),(face_list_length_),(face_list_))

#define HC_KInsert_Shell_By_Tristrips(point_count_,points_, \
	tristrips_list_length_,tristrips_list_,tristrips_faces_length_,tristrips_faces_) \
	HC_Insert_Shell_By_Tristrips((point_count_),(points_), \
	(tristrips_list_length_),(tristrips_list_),(tristrips_faces_length_),(tristrips_faces_))

#define HC_KInsert_Shell(point_count_,points_,face_list_length_,face_list_) \
	HC_Insert_Shell((point_count_),(points_),(face_list_length_),(face_list_))

#define HC_KInsert_Shell_By_Ref(point_count_,points_,face_list_length_,face_list_) \
	HC_Insert_Shell_By_Ref((point_count_),(points_),(face_list_length_),(face_list_))






#define HC_KCompute_Capping_Shell(key_count_,shell_keys_,cut_plane_) \
	HC_Compute_Capping_Shell((key_count_),(shell_keys_),(cut_plane_))

#define	HC_KCompute_Boolean_Shell(in_point_count1_, in_points1_, \
		in_normals_as_points1_, in_face_list_length1_,in_face_list1_,in_point_count2_, \
		in_points2_,in_normals_as_points2_,in_face_list_length2_,in_face_list2_, option_string_) \
	HC_Compute_Boolean_Shell((in_point_count1_), (in_points1_), \
		(in_normals_as_points1_), (in_face_list_length1_),(in_face_list1_),(in_point_count2_), \
		(in_points2_),(in_normals_as_points2_),(in_face_list_length2_),(in_face_list2_), (option_string_)) \

#define	HC_KCopy_Geometry(key_, seg_) \
	HC_Copy_Geometry ((key_),(seg_))

#define	HC_KCopy_Segment(oldname_, newname_) \
	HC_Copy_Segment((oldname_), (newname_))

#define	HC_KCopy_Segment_With_Mapping(oldname_, newname_, count_, old_keys_, new_keys_) \
	HC_Copy_Segment_With_Mapping((oldname_), (newname_), (count_), (old_keys_), (new_keys_))

#define HC_KCreate_Segment(name_) \
	HC_Create_Segment((name_))

#define HC_KInclude_Segment(name_) \
	HC_Include_Segment((name_))

#define HC_KInclude_Segment(name_) \
	HC_Include_Segment((name_))

#define HC_KInclude_Segment_By_Key(key_) \
	HC_Include_Segment_By_Key((key_))

#define HC_KShow_Include_Segment(key_) \
	HC_Show_Include_Segment((key_),0)

#define HC_KShow_Style_Segment(key_) \
	HC_Show_Style_Segment((key_),0)

#define HC_KOpen_Segment(name_) \
	HC_Open_Segment((name_))

#define HC_KStyle_Segment(key_) \
	HC_Style_Segment((key_))

#define HC_KRenumber_Key(old_key_, new_key_, scope_) \
	HC_Renumber_Key ((old_key_), (new_key_), (scope_))

#define HC_KShow_Renumbered_Key(key_,status_) \
	HC_Show_Renumbered_Key((key_),(status_))

#define HC_KShow_Original_Key(key_) \
	HC_Show_Original_Key((key_))

#define HC_KConditional_Style(name_,condition_) \
	HC_Conditional_Style((name_),(condition_))

#define HC_KConditional_Style_By_Key(key_,condition_) \
	HC_Conditional_Style_By_Key((key_),(condition_))

#define HC_KConditional_Include(name_,condition_) \
	HC_Conditional_Include((name_),(condition_))

#define HC_KConditional_Include_By_Key(key_,condition_) \
	HC_Conditional_Include_By_Key((key_),(condition_))

#define HC_KReference_Geometry(name_) \
	HC_Reference_Geometry((name_))

#define HC_KReference_Geometry_By_Key(key_) \
	HC_Reference_Geometry_By_Key((key_))

#define HC_KConditional_Reference(name_,condition_) \
	HC_Conditional_Reference((name_),(condition_))

#define HC_KConditional_Reference_By_Key(key_,condition_) \
	HC_Conditional_Reference_By_Key((key_),(condition_))

#define HC_KShow_Reference_Geometry(key_) \
	HC_Show_Reference_Geometry((key_))

#define HC_KShow_Conditional_Reference(key_, condition_) \
	HC_Show_Conditional_Reference((key_), (condition_))

#define	HC_KShow_Owner_By_Key(key_) \
	HC_Show_Owner_By_Key((key_),0)

#define	HC_KShow_Owner_Original_Key(key_) \
	HC_Show_Owner_Original_Key((key_))

#define HC_Compute_Shell_Sweep(shellkey_,direction_in_,front_key_,back_key_,side_key_,options_) \
	HC_Compute_Swept_Shell((shellkey_),(direction_in_),(front_key_),(back_key_),(side_key_),options_) \

#define HC_KCompute_Shell_Sweep(shellkey_,direction_in_,options_) \
	HC_Compute_Swept_Shell((shellkey_),(direction_in_),0,0,0,"merge")

#define HC_KCompute_Swept_Shell(shellkey_, direction_in_, options_) \
	HC_Compute_Swept_Shell((shellkey_),(direction_in_),0,0,0,"merge")

#define HC_KStyle_Segment_By_Key(key_) \
	HC_Style_Segment_By_Key((key_))

// Show_Net

#define HC_Show_Net_Explicit_Color(type_, channel_, rgb_, texture_, options_) \
	HC_PShow_Net_Explicit_Color(0,0, (type_),(channel_),(rgb_),(texture_),(options_))

#define HC_Show_Net_Callback(callbacks_) \
	HC_PShow_Net_Callback(0,0, (callbacks_))

#define HC_Show_Net_Camera(position_, target_, up_, width_, height_, projection_) \
	HC_PShow_Net_Camera(0,0, (position_),(target_),(up_),(width_),(height_),(projection_))

#define HC_Show_Net_Camera_By_Volume(proj_, xmin_, xmax_, ymin_, ymax_) \
	HC_PShow_Net_Camera_By_Volume(0,0, (proj_),(xmin_),(xmax_),(ymin_),(ymax_))

#define HC_Show_Net_Camera_Field(width_, height_) \
	HC_PShow_Net_Camera_Field(0,0, (width_),(height_))

#define HC_Show_Net_Camera_Position(x_, y_, z_) \
	HC_PShow_Net_Camera_Position(0,0, (x_),(y_),(z_))

#define HC_Show_Net_Camera_Projection(projection_) \
	HC_PShow_Net_Camera_Projection(0,0, (projection_))

#define HC_Show_Net_Camera_Target(x_, y_, z_) \
	HC_PShow_Net_Camera_Target(0,0, (x_),(y_),(z_))

#define HC_Show_Net_Camera_Up_Vector(x_, y_, z_) \
	HC_PShow_Net_Camera_Up_Vector(0,0, (x_),(y_),(z_))

#define HC_Show_Net_Color(color_spec_) \
	HC_PShow_Net_Color(0,0, (color_spec_))

#define HC_Show_Net_Color_Map(colors_) \
	HC_PShow_Net_Color_Map(0,0, (colors_))

#define HC_Show_Net_Color_Map_By_Value(color_space_, count_, values_) \
	HC_PShow_Net_Color_Map_By_Value(0,0, (color_space_),(count_),(values_))

#define HC_Show_Net_Color_Map_Count(count_) \
	HC_PShow_Net_Color_Map_Count(0,0, (count_))

#define HC_Show_Net_Driver_Options(list_) \
	HC_PShow_Net_Driver_Options(0,0, (list_))

#define HC_Show_Net_Edge_Pattern(pattern_) \
	HC_PShow_Net_Edge_Pattern(0,0, (pattern_))

#define HC_Show_Net_Edge_Pattern_Explic(pattern_) \
	HC_PShow_Net_Edge_Pattern_Exp(0,0, (pattern_))

#define HC_Show_Net_Edge_Pattern_Explicit(pattern_) \
	HC_PShow_Net_Edge_Pattern_Exp(0,0, (pattern_))

#define HC_Show_Net_Edge_Weight(weight_) \
	HC_PShow_Net_Edge_Weight(0,0, (weight_))

#define HC_Show_Net_Face_Pattern(pattern_) \
	HC_PShow_Net_Face_Pattern(0,0, (pattern_))

#define HC_Show_Net_Handedness(value_) \
	HC_PShow_Net_Handedness(0,0, (value_))

#define HC_Show_Net_Heuristics(list_) \
	HC_PShow_Net_Heuristics(0,0, (list_))

#define HC_Show_Net_Line_Pattern(pattern_) \
	HC_PShow_Net_Line_Pattern(0,0, (pattern_))

#define HC_Show_Net_Line_Pattern_Explic(pattern_) \
	HC_PShow_Net_Line_Pattern_Exp(0,0, (pattern_))

#define HC_Show_Net_Line_Pattern_Explicit(pattern_) \
	HC_PShow_Net_Line_Pattern_Exp(0,0, (pattern_))

#define HC_Show_Net_Line_Weight(weight_) \
	HC_PShow_Net_Line_Weight(0,0, (weight_))

#define HC_Show_Net_Marker_Size(size_) \
	HC_PShow_Net_Marker_Size(0,0, (size_))

#define HC_Show_Net_Marker_Symbol(symbol_) \
	HC_PShow_Net_Marker_Symbol(0,0, (symbol_))

#define HC_Show_Net_Modelling_Matrix(matrix_) \
	HC_PShow_Net_Modelling_Matrix(0,0, (matrix_))

#define HC_Show_Net_Rendering_Options(list_) \
	HC_PShow_Net_Rendering_Options(0,0, (list_))

#define HC_Show_Net_Selectability(list_) \
	HC_PShow_Net_Selectability(0,0, (list_))

#define HC_Show_Net_Text_Alignment(locater_) \
	HC_PShow_Net_Text_Alignment(0,0, (locater_))

#define HC_Show_Net_Text_Font(options_) \
	HC_PShow_Net_Text_Font(0,0, (options_))

#define HC_Show_Net_Text_Path(x_, y_, z_) \
	HC_PShow_Net_Text_Path(0,0, (x_),(y_),(z_))

#define HC_Show_Net_Text_Spacing(spacing_) \
	HC_PShow_Net_Text_Spacing(0,0, (spacing_))

#define HC_Show_Net_Texture_Matrix(matrix_) \
	HC_PShow_Net_Texture_Matrix(0,0, (matrix_))

#define HC_Show_Net_User_Index_Count(count_) \
	HC_PShow_Net_User_Index_Count(0,0, (count_))

#define HC_Show_Net_User_Indices(count_,indices_,values_) \
	HC_PShow_Net_User_Indices(0,0, (count_),(indices_),(values_))

#define HC_Show_Net_User_Options(list_) \
	HC_PShow_Net_User_Options(0,0, (list_))

#define HC_Show_Net_Unicode_Options(options_) \
	HC_PShow_Net_Unicode_Options(0,0, (options_))

#define HC_Show_Net_User_Opt_Length(length_) \
	HC_PShow_Net_User_Opt_Length(0,0, (length_))

#define HC_Show_Net_User_Options_Length(length_) \
	HC_PShow_Net_User_Opt_Length(0,0, (length_))

#define HC_Show_Net_Unicode_Opt_Length(length_) \
	HC_PShow_Net_Unicode_Opt_Length(0,0, (length_))

#define HC_Show_Net_Unicode_Options_Length(length_) \
	HC_PShow_Net_Unicode_Opt_Length(0,0, (length_))

#define HC_Show_Net_User_Value(data_) \
	HC_PShow_Net_User_Value(0,0, (data_))

#define HC_Show_Net_Visibility(list_) \
	HC_PShow_Net_Visibility(0,0, (list_))

#define HC_Show_Net_Window(left_, right_, bottom_, top_) \
	HC_PShow_Net_Window(0,0, (left_),(right_),(bottom_),(top_))

#define HC_Show_Net_Window_Frame(flag_) \
	HC_PShow_Net_Window_Frame(0,0, (flag_))

#define HC_Show_Net_Window_Pattern(pattern_) \
	HC_PShow_Net_Window_Pattern(0,0, (pattern_))

#define HC_Show_Net_Var_Edge_Weight(weight_) \
	HC_PShow_Net_Var_Edge_Weight(0,0, (weight_))

#define HC_Show_Net_Variable_Edge_Weight(weight_) \
	HC_PShow_Net_Var_Edge_Weight(0,0, (weight_))

#define HC_Show_Net_Var_Line_Weight(weight_) \
	HC_PShow_Net_Var_Line_Weight(0,0, (weight_))

#define HC_Show_Net_Variable_Line_Weight(weight_) \
	HC_PShow_Net_Var_Line_Weight(0,0, (weight_))

#define HC_Show_Net_Var_Marker_Size(size_) \
	HC_PShow_Net_Var_Marker_Size(0,0, (size_))

#define HC_Show_Net_Variable_Marker_Size(size_) \
	HC_PShow_Net_Var_Marker_Size(0,0, (size_))

#define HC_Show_Net_Clip_Region(loops_, lengths_, points_, spec_) \
	HC_PShow_Net_Clip_Region(0,0, (loops_),(lengths_),(points_),(spec_))

#define HC_Show_Net_Clip_Region_Size(loops_, total_, spec_) \
	HC_PShow_Net_Clip_Region_Size(0,0, (loops_),(total_),(spec_))

#define HC_Show_Net_Camera_Near_Limit(limit_) \
	HC_PShow_Net_Camera_Near_Limit(0,0, (limit_))

#define HC_Show_Net_Conditions(options_) \
	HC_PShow_Net_Conditions(0,0, (options_))

#define HC_Show_Net_Conditions_Length(length_) \
	HC_PShow_Net_Conditions_Length(0,0, (length_))

#define HC_Show_Net_Texture(txr_, output_, out_image_) \
	HC_PShow_Net_Texture(0,0, (txr_),(output_),(out_image_))

#define HC_Show_Net_Shader(name_, output_, shader_source_) \
	HC_PShow_Net_Shader(0,0, (name_),(output_),(shader_source_))

#define HC_Show_Net_Color_Map_Length(value_) \
	HC_PShow_Net_Color_Map_Length(0,0, (value_))

#define HC_Show_One_Net_Callback(callback_point_, callback_name_) \
	HC_PShow_One_Net_Callback(0,0, (callback_point_),(callback_name_))

#define HC_Show_One_Net_Callback_WD(cbname_, spec_, data_) \
	HC_PShow_One_Net_Callback_WD(0,0, (cbname_),(spec_),(data_))

#define HC_Show_One_Net_Callback_With_Data(cbname_, spec_, data_) \
	HC_PShow_One_Net_Callback_WD(0,0, (cbname_),(spec_),(data_))

#define HC_Show_One_Net_Color(type_, color_) \
	HC_PShow_One_Net_Color(0,0, (type_),(color_))

#define HC_Show_One_Net_Color_By_Index(type_, index_) \
	HC_PShow_One_Net_Color_By_Index(0,0, (type_),(index_))

#define HC_Show_One_Net_Color_By_Value(type_, color_space_, a_, b_, c_) \
	HC_PShow_One_Net_Color_By_Value(0,0, (type_),(color_space_),(a_),(b_),(c_))

#define HC_Show_One_Net_Color_Map(offset_, color_) \
	HC_PShow_One_Net_Color_Map(0,0, (offset_),(color_))

#define HC_Show_One_Net_Color_Map_By_V(offset_, color_space_, a_, b_, c_) \
	HC_PShow_One_Net_Color_Map_By_V(0,0, (offset_),(color_space_),(a_),(b_),(c_))

#define HC_Show_One_Net_Color_Map_By_Value(offset_, color_space_, a_, b_, c_) \
	HC_PShow_One_Net_Color_Map_By_V(0,0, (offset_),(color_space_),(a_),(b_),(c_))

#define HC_Show_One_Net_Driver_Option(type_, value_) \
	HC_PShow_One_Net_Driver_Option(0,0, (type_),(value_))

#define HC_Show_One_Net_Heuristic(type_, value_) \
	HC_PShow_One_Net_Heuristic(0,0, (type_),(value_))

#define HC_Show_One_Net_Rendering_Optio(type_, value_) \
	HC_PShow_One_Net_Rendering_Opti(0,0, (type_),(value_))

#define HC_Show_One_Net_Rendering_Option(type_, value_) \
	HC_PShow_One_Net_Rendering_Opti(0,0, (type_),(value_))

#define HC_Show_One_Net_Selectability(type_, value_) \
	HC_PShow_One_Net_Selectability(0,0, (type_),(value_))

#define HC_Show_One_Net_Text_Font(type_, value_) \
	HC_PShow_One_Net_Text_Font(0,0, (type_),(value_))

#define HC_Show_One_Net_User_Index(index_, value_) \
	HC_PShow_One_Net_User_Index(0,0, (index_),(value_))

#define HC_Show_One_Net_User_Option(type_, value_) \
	HC_PShow_One_Net_User_Option(0,0, (type_),(value_))

#define HC_Show_One_Net_Unicode_Option(requestedOption_, options_) \
	HC_PShow_One_Net_Unicode_Option(0,0, (requestedOption_),(options_))

#define HC_Show_One_Net_Visibility(type_, value_) \
	HC_PShow_One_Net_Visibility(0,0, (type_),(value_))

#define HC_Show_One_Net_Uni_Opt_Len(uoname_, length_) \
	HC_PShow_One_Net_Uni_Opt_Len(0,0, (uoname_),(length_))

#define HC_Show_One_Net_Unicode_Option_Length(uoname_, length_) \
	HC_PShow_One_Net_Uni_Opt_Len(0,0, (uoname_),(length_))

#define HC_Show_One_Net_User_Opt_Len(uoname_, length_) \
	HC_PShow_One_Net_User_Opt_Len(0,0, (uoname_),(length_))

#define HC_Show_One_Net_User_Option_Length(uoname_, length_) \
	HC_PShow_One_Net_User_Opt_Len(0,0, (uoname_),(length_))

#define HC_Show_One_Net_Condition(cname_, options_) \
	HC_PShow_One_Net_Condition(0,0, (cname_),(options_))

#define HC_Show_One_Net_Texture(txr_, opt_, output_) \
	HC_PShow_One_Net_Texture(0,0, (txr_),(opt_), (output_))

#define HC_Show_One_Net_Shader_Option(name_, opt_, output_) \
	HC_PShow_One_Net_Shader_Option(0,0, (name_),(opt_), (output_))

#define HC_Show_One_Net_User_Data(index_, data_, data_length_) \
	HC_PShow_One_Net_User_Data(0,0, (index_),(data_), (data_length_))


#endif




