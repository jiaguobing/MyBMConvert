// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef TAGS_DEFINED


Begin_HOOPS_Namespace

struct Alias;
struct Anything;
struct Attribute;
struct Attribute_Lock;
struct Area_Light;
//class Bank_Lookup;
struct Freetype_Font;
struct outline_cache_t;
class Callback;
struct Callback_List;
class Callback_Name_Data;
class Camera;
class Camera_Handle;
struct Circle;
struct Circular_Arc;
class Clip_Region;
class Color;
class Color_List;
class Color_Map;
struct Color_Name;
struct Color_Def;
class Conditions;
class Conditional;
class Condition_Bits;
struct Conditional_Action;
struct Constant_Option_Table_Info;
struct Constant_Option_Type;
struct Convex_Clip_Region;
struct Cut_Geometry_Set;
struct Cutting_Plane;
struct Cutting_Plane_Instance;
struct Cutting_Plane_Set;
struct Cutting_Plane_Options;
struct Cut_Geometry_Pieces;
struct Contour;
struct Cylinder;
struct Deferred_Text;
struct Actor;
struct Driver_Color;
struct Driver_Config;
class Texture_Control_List;
struct Display_Context;
struct Display_List;
struct Deferred_Draw_Tree;
struct Actor;
class Driver_Options;
struct Error_Handler;
struct Exit_Handler;
class Edge_Pattern;
class Edge_Weight;
struct Ellipse;
struct Elliptical_Arc;
struct Error_Control;
struct Event;
class Face_Pattern;
struct Fast_Lookup;
struct File_Data;
struct Find_Attribute_Control;
struct Find_Contents_Data;
struct Font;
struct Font_Family;
struct Font_Instance;
struct Font_Instance_Char_Data;
class Font_Name;
struct Font_Names_Data;
struct Frame_Buffer_Info;
struct Gather_Capping_Geometry;
struct Gather_Device_Info;
struct Gather_Font_Info;
struct Gather_Font_Names;
struct Gather_Screen_Extent;
struct Gather_Text_Extent;
struct Gather_Window_Extent;
struct Generic_Arc;
class Generic_Size;
struct Geometry;
struct Geometry_Reference;
struct Glyph_Data;
class Glyph_Defs;
struct Grid;
class Handedness;
class Heuristics;
struct Hidden;
struct Hidden_Progress;
struct Highlight_Geometry;
struct Highlight_Map;
struct Highlight_Options;
struct Highlight_Segment;
struct HL_Data;
struct Hidden_Payload;
struct Hidden_Cache;
struct Hidden_Slab;
struct Hidden_Point;
struct Hidden_Tristrip;
struct Hidden_Tristrip_Heap;
struct Hidden_Polymarker;
struct HLS;
class HM_Mutex;
class HM_RWLock;
class HM_Semaphore;
class HM_Thread;
struct ICU_Entry_Point;
struct Image;
struct Image_Options;
struct Include_Chain;
struct Int_XY;
struct Include;
struct Isoline_Pieces;
struct KAttribute;
struct KName;
struct KOption_Type;
struct Key_List;
struct Light;
struct Line_Style_Data;
struct Line_Style_Item;
class Line_Style_Defs;
struct Line_Style_Parallel;
class Line_Pattern;
class Line_Weight;
struct LEA_Pattern;
struct LVA_Symbol;
struct LOD_Node;
struct Log_Font_Item;
struct Local_Vertex_Attributes;
struct Marker;
class Marker_Symbol;
class Marker_Size;
struct Memory_Block;
struct Mesh;
struct Metafile;
class Mimic_Info;
class Modelling_Matrix;
struct Multiline;
struct Multimarker;
class Name;
class Name_Const;
struct NameHasher;
struct Named_Style_Data;
class Named_Style_Defs;
struct Named_Style_Tracker_Node;
class Named_Material;
class Named_Material_Handle;
struct NURBS_Curve;
struct NURBS_Curve_Control;
struct NURBS_Surface;
struct NURBS_Surface_Config;
struct One_Face_Attributes;
struct One_Edge_Attributes;
struct Open_Item;
struct Option_Hash_Table;
struct Option_Type;
struct Option_Value;
struct Path_Token;
struct Path_Token_List;
struct Pending_Special;
struct Pending_Redraw;
struct Pending_Activity;
class Point_Interceptor;
struct PolyCylinder;
struct PolyCylinder_Attributes;
struct Polyedge;
struct Polygon_Collector;
struct Polyhedron;
struct Polyhedron_Drawing_Data;
struct Polygon;
struct Polyline;
struct Polymarker;
struct Quickmoved_Segment;
struct Read_Metafile_Data;
struct Region_Edge;
struct Renderer_Actions;
class Rendering_Options;
struct Segment;
struct Segment_Cache;
struct Intersecting_PSeg;
struct Segstuff;
class Selectability;
struct Selection_Context;
class Selection_Event;
struct Selection_Extras;
struct Selection_Item;
struct Selection_Item_Heap;
struct Selection_Event_CD;
struct Selection_Sample;
struct Search_Header;
struct Search_List;
struct Set_And_Clear_Flags_Control;
struct Shell;
struct Base_Shadow;
struct Shadow_Map;
struct Show_Window_Info;
struct Simple_Shadow;
struct Span;
struct Span_Anchor;
struct Span_Appearance;
struct Sphere;
class Deferred_Item;
struct Spot_Light_Info;
struct Spot_Light;
struct Stencil_Bitmap;
struct String_Cursor;
struct String_Event;
class Streaming_Mode;
struct Stroked_Char;
struct Style;
struct Subsegment;
struct System_Data;
struct Text;
class Text_Alignment;
class Text_Font;
class Text_Path;
class Text_Spacing;
struct Text_Info;
struct Strip_Common;
struct Text_Karacter_Info;
struct Text_Region;
struct Texture;
struct Texture_Parameters;
class Texture_Matrix;
class Texture_Defs;
struct Thread_Data;
struct Tinting;
struct Tristrip;
class User_Options;
class User_Value;
struct User_Face_Pattern;
struct UV_Edge;
struct UV_Region;
struct UV_Region_Set;
struct UV_Trim;
class Visibility;
struct Wakeup;
struct Wedge_Tree;
class Window;
class Window_Frame;
class Window_Pattern;
struct Window_Tree;
struct Windows_World;
struct Windows_World_Entry;
struct Wing;
struct World;
struct XBits;
struct Xref;

class CMO;
class CMO_Pooled;
class CMO_Pooled_Copyable;

class Task;
class Task_Queue;
class Driver_DL;
class User_Option_Compressor;
class Bounding;
class Internal_Bounding;
class Include_Path;

enum class Decimate_Algorithm { Fast, Nice };
class Begin_Decimate_Config;
class Decimate_Config;
class Shell_Optimization_Config;

template <typename F>
class Collapse_Predicate;


typedef unsigned char	Clip_Case;


template <typename T> class Counted_Pointer;
typedef Counted_Pointer<Glyph_Data>	Glyph;
typedef Counted_Pointer<Line_Style_Data> Line_Style;
typedef Counted_Pointer<Named_Style_Data> Named_Style;
typedef Counted_Pointer<Font_Names_Data> Font_Name_List;
typedef Counted_Pointer<Callback_Name_Data>	Callback_Name;

template <typename T> class Rendition_Pointer;

struct Internal_Net_Rendition;
typedef Rendition_Pointer<Internal_Net_Rendition>	Net_Rendition;

struct Internal_Transform_Rendition;
typedef Rendition_Pointer<Internal_Transform_Rendition>	Transform_Rendition;

template <typename F>	struct Elliptical_Arc_Data;
typedef Elliptical_Arc_Data<float>		Elliptical_Arc_Single;

template <typename F>	struct Intersection_3D;
typedef Intersection_3D<float>		Intersection;
typedef Intersection_3D<double>		DIntersection;

template <typename F>	struct Loop_Info_3D;
typedef Loop_Info_3D<float>		Loop_Info;
typedef Loop_Info_3D<double>	DLoop_Info;
template <typename F>	struct Loop_Containment_Tree_3D;
typedef Loop_Containment_Tree_3D<float>		Loop_Containment_Tree;
typedef Loop_Containment_Tree_3D<double>	DLoop_Containment_Tree;

template <typename T> class Basis_Pair_3D;
typedef Basis_Pair_3D<float>	Basis_Pair;
typedef Basis_Pair_3D<double>	DBasis_Pair;

End_HOOPS_Namespace




#define VList HVList
template <typename T> class VList;

#define VHash HVHash
template <typename T1, typename T2> class VHash;

template <typename T, int size = 16> class Trivial_LRU;

template <typename K, typename T, class Hash, class Pred, class Alloc> class Unordered;


Begin_HOOPS_Namespace

struct Geometry_Highlight;
struct Geometry_Highlight_Info;

End_HOOPS_Namespace


#define TAGS_DEFINED
#endif
