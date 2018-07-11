/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public enum TKE_Object_Types {

	
	TKE_Termination					= '\x00',		//!< ////
	TKE_Pause						= '\x01',		//!< ////
													/////////
	TKE_Comment						= ';',			//!< ////
													/////////
	TKE_Font						= 'f',			//!< ////
	TKE_Texture						= 't',			//!< ////
	TKE_Material					= '\x02',		//!< ////
													/////////
	TKE_Open_Segment				= '(',			//!< ////
	TKE_Close_Segment				= ')',			//!< ////
	TKE_Reopen_Segment				= 's',			//!< ////
	TKE_Include_Segment				= '<',			//!< ////
	TKE_Style_Segment				= '{',			//!< ////
	TKE_Named_Style					= 'y',			//!< ////
													/////////
	TKE_Geometry_Attributes			= ':',			//!< ////
	TKE_Renumber_Key_Global			= 'K',			//!< ////
	TKE_Renumber_Key_Local			= 'k',			//!< ////
	TKE_Priority					= '0',			//!< ////
	TKE_Tag							= 'q',			//!< ////
													/////////
	TKE_Bounding					= 'b',			//!< ////
	TKE_Bounding_Info				= 'B',			//!< ////
	TKE_Callback					= '\x07',		//!< ////
	TKE_Camera						= '>',			//!< ////
	TKE_Conditional_Action			= '\'',			//!< ////
	TKE_Conditions					= '?',			//!< ////
	TKE_Color						= '"',			//!< ////
	TKE_Color_By_Index				= '\x08',		//!< ////
	TKE_Color_By_Index_16			= '\x09',		//!< ////
	TKE_Color_By_FIndex				= '\x0A',		//!< ////
	TKE_Color_RGB					= '~',			//!< ////
	TKE_Color_By_Value				= '\x0B',		//!< ////
	TKE_Color_Map					= '\x0C',		//!< ////
	TKE_Edge_Pattern				= '\x0D',		//!< ////
	TKE_Edge_Weight					= '\x0E',		//!< ////
	TKE_Face_Pattern				= 'P',			//!< ////
	TKE_Geometry_Options			= '\x17',		//!< ////
	TKE_Handedness					= 'h',			//!< ////
	TKE_Heuristics					= 'H',			//!< ////
	TKE_Line_Pattern				= '-',			//!< ////
	TKE_Line_Weight					= '=',			//!< ////
	TKE_Marker_Size					= '+',			//!< ////
	TKE_Marker_Symbol				= '@',			//!< ////
	TKE_Modelling_Matrix			= '%',			//!< ////
	TKE_LOD							= '\x19',		//!< ////
	TKE_Rendering_Options			= 'R',			//!< ////
	TKE_Selectability				= '!',			//!< ////
	TKE_Text_Alignment				= '*',			//!< ////
	TKE_Text_Font					= 'F',			//!< ////
	TKE_Text_Path					= '|',			//!< ////
	TKE_Text_Spacing				= ' ',			//!< ////
	TKE_Texture_Matrix				= '$',			//!< ////
	TKE_Unicode_Options				= '\x16',		//!< ////
	TKE_User_Index					= 'n',			//!< ////
	TKE_User_Index_Data				= 'm',			//!< ////
	TKE_User_Options				= 'U',			//!< ////
	TKE_User_Value					= 'v',			//!< ////
	TKE_Visibility					= 'V',			//!< ////
	TKE_Window						= 'W',			//!< ////
	TKE_Window_Frame				= '#',			//!< ////
	TKE_Window_Pattern				= 'p',			//!< ////
	TKE_Glyph_Definition			= 'j',			//!< ////
	TKE_Line_Style					= 'J',			//!< ////
	TKE_Named_Style_Def 			= 'u',			//!< ////
													/////////
	TKE_Area_Light					= 'a',			//!< ////
	TKE_Circle						= 'C',			//!< ////
	TKE_Circular_Arc				= 'c',			//!< ////
	TKE_Circular_Chord				= '\\',			//!< ////
	TKE_Circular_Wedge				= 'w',			//!< ////
	TKE_Cutting_Plane				= '/',			//!< ////
	TKE_Cylinder					= 'Y',			//!< ////
	TKE_Distant_Light				= 'd',			//!< ////
	TKE_Ellipse						= 'E',			//!< ////
	TKE_Elliptical_Arc				= 'e',			//!< ////
	TKE_Grid						= 'g',			//!< ////
	TKE_Image						= 'i',			//!< ////
	TKE_Infinite_Line				= '`',			//!< ////
	TKE_Infinite_Ray				= '\x11',		//!< ////
	TKE_Line						= 'l',			//!< ////
	TKE_Local_Light					= '.',			//!< ////
	TKE_Marker						= 'X',			//!< ////
	TKE_Mesh						= 'M',			//!< ////
	TKE_NURBS_Curve					= 'N',			//!< ////
	TKE_NURBS_Surface				= 'A',			//!< ////
	TKE_PolyCylinder				= 'Q',			//!< ////
	TKE_Polygon						= 'G',			//!< ////
	TKE_Polyline					= 'L',			//!< ////
	TKE_PolyPolyline				= '\x10',		//!< ////
	TKE_Reference					= 'r',			//!< ////
	TKE_Shell						= 'S',			//!< ////
	TKE_Sphere						= '\x1a',		//!< ////
	TKE_Spot_Light					= '^',			//!< ////
	TKE_Text						= 'T',			//!< ////
	TKE_Text_With_Encoding			= 'x',			//!< ////
													/////////
	TKE_Start_User_Data				= '[',			//!< ////
	TKE_Stop_User_Data				= ']',			//!< ////
	TKE_XML							= '\x18',		//!< ////
	TKE_External_Reference			= '\x12',		//!< ////
	TKE_External_Reference_Unicode	= '\x13',		//!< ////
	TKE_URL							= '\x15',		//!< ////
													/////////
	TKE_Start_Compression			= 'Z',			//!< ////
	TKE_Stop_Compression			= 'z',			//!< ////
													/////////
	TKE_Repeat_Object				= '&',			//!< ////
	TKE_View						= '}',			//!< ////
	TKE_Clip_Rectangle				= 'o',			//!< ////
	TKE_Clip_Region					= 'O',			//!< ////
	TKE_Complex_Clip_Region			= '\x0F',		//!< //
													/////////
	TKE_File_Info					= 'I',			//!< ////
	TKE_Dictionary					= 'D',			//!< ////
	TKE_Dictionary_Locater			= '_',			//!< ////
	TKE_Thumbnail					= '\x14',		//!< ////
	TKE_Delete_Object				= '\x7F',		//!< ////
													/////////
	TKE_Tag_Implicit				= TKE_Tag,		//!< //-- obsolete.  this alias provided for source compatibility with old code
	TKE_Streaming_Mode				= ',',			//!< //-- would like this to be obsolete, but...

	TKE_First_User_Opcode			= '\xA0',		//!< //// first opcode value reserved for private use
	TKE_Last_User_Opcode			= '\xEF',		//!< //// last opcode value reserved for private use
	TKE_HW3D_Image					= 0xE0,
													/////////
	TKE_Pseudo_Handler				= '\xFE',		//!< //// pseudo-handler (non-zero value)
	TKE_Extended_Code				= '\xFF'		//!< //// reserved for future expansion
}
