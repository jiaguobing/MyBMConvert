// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef TASK_QUEUE_DEFINED

Begin_HOOPS_Namespace

enum Task_Type{
	Task_Type_Free_Memory_Task = 1,
	Task_Type_DL_Delete_Task,
	Task_Type_Clean_Grids_Task,
	Task_Type_Standard_Driver_Task,
	Task_Type_Request_Update_Task,
	Task_Type_Request_Shutdown_Task,
	Task_Type_Flush_Derived_Normals_Task,
	Task_Type_DX9_Device_Create_Task,
	Task_Type_DX9_Device_Delete_Task,
	Task_Type_Actor_Image_Message_Pump_Task,
	Task_Type_Actor_Location_Event_Task,
	Task_Type_Actor_Keyboard_Event,
	Task_Type_Actor_Mom_And_Apple_Pie,

	HPSI_Exchange_Import_Notifier_Task,
	HPSI_Exchange_Translation_Notifier_Task,
	HPSI_SKP_Import_Notifier_Task,
	HPSI_Parasolid_Import_Notifier_Task,

	HPSI_Publish_Create_Document_Task,
	HPSI_Publish_Create_Document_Key_Task,
	HPSI_Publish_Save_Document_Key_Task,
	HPSI_Publish_Close_Document_Key_Task,
	HPSI_Publish_Create_Pages_Task,
	HPSI_Publish_Set_Document_Information_Task,
	HPSI_Publish_Set_Document_Permission_Task,
	HPSI_Publish_Set_Document_Passwords_Task,
	HPSI_Publish_Add_Document_JavaScript_Task,
	HPSI_Publish_Add_Document_Attachments_Task,
	HPSI_Publish_Add_Document_Icon_Images_Task,
	HPSI_Publish_Get_Document_Page_Count_Task,
	HPSI_Publish_Remove_Document_Pages_Task,
	HPSI_Publish_Insert_Page_Annotations_Task,
	HPSI_Publish_Insert_Page_Text_Task,
	HPSI_Publish_Insert_Page_Images_Task,
	HPSI_Publish_Insert_Page_Tables_Task,
	HPSI_Publish_Insert_Page_Links_Task,
	HPSI_Publish_Insert_Page_Buttons_Task,
	HPSI_Publish_Insert_Page_Checkboxes_Task,
	HPSI_Publish_Insert_Page_Radio_Buttons_Task,
	HPSI_Publish_Insert_Page_List_Boxes_Task,
	HPSI_Publish_Insert_Page_Drop_Down_Lists_Task,
	HPSI_Publish_Insert_Page_Signatures_Task,
	HPSI_Publish_Insert_Page_Text_Fields_Task,
	HPSI_Publish_Insert_Page_Slide_Tables_Task,
	HPSI_Publish_Set_Page_Annotations_By_Field_Task,
	HPSI_Publish_Set_Page_Button_Icons_By_Field_Task,
	HPSI_Publish_Set_Page_Button_Labels_By_Field_Task,
	HPSI_Publish_Add_Page_List_Items_By_Field_Task,
	HPSI_Publish_Set_Page_JavaScript_Actions_By_Field_Task,
	HPSI_Publish_Set_Page_Visibilities_By_Field_Task,
	HPSI_Publish_Set_Page_Text_Values_By_Field_Task,
	HPSI_Publish_Show_Page_Fields_Task,
	HPSI_Publish_Unload_Exchange_Task,
#ifdef HPS_CORE_BUILD
	HPSI_DBI_Agent_Task,

	HPSI_DBI_Delayed_Init_Task,

	HPSI_DBI_Set_Font_Directories_Task,

	HPSI_DBI_Ensure_Can_Use_Product_Licensed_Task,

	HPSI_DBI_Call_Function_Task,

	HPSI_EventDispatcher_Task,
	HPSI_WindowKeyImpl_Task,
	HPSI_WindowKeySnapshot_Task,
	HPSI_GetDriverData_Task,
	HPSI_StreamImportNotifier_Task,
	HPSI_OBJImportNotifier_Task,
	HPSI_STLImportNotifier_Task,
	HPSI_Knacker_Task,
	HPSI_Window_Event_Service_Task,

	HPSI_DBI_Read_Task,
	HPSI_DBI_Write_Task,

	HPSI_DBI_Agent_Init_Task,

	HPSI_DBI_Queue_Deletion_Task,

	HPSI_DBI_Flush_Segment_Task,

	HPSI_DBI_Delete_By_Keys_Task,
	HPSI_DBI_Style_Segment_Task,
	HPSI_DBI_Style_Portfolio_Task,
	HPSI_DBI_UnSet_Style_Segment_Task,
	HPSI_DBI_Named_Style_Segment_Task,
	HPSI_DBI_Set_Styles_Task,
	HPSI_DBI_Set_Portfolios_Task,
	HPSI_DBI_UnSet_Named_Style_Segment_Task,
	HPSI_DBI_UnSet_Certain_Styles_Task,
	HPSI_DBI_Pop_Named_Style_Segment_Task,
	HPSI_DBI_Set_Style_Condition_Task,
	HPSI_DBI_Flush_Style_Task,

	HPSI_DBI_Gather_Cut_Geometry_Task,

	HPSI_DBI_Set_Conditions_Task,
	HPSI_DBI_UnSet_Conditions_Task,

	HPSI_DBI_Include_Segment_Task,
	HPSI_DBI_Set_Include_Condition_Task,
	HPSI_DBI_Reference_Geometry_Task,
	HPSI_DBI_Set_Reference_Condition_Task,
	HPSI_DBI_Set_Bounding_Task,
	HPSI_DBI_UnSet_Bounding_Everything_Task,

	HPSI_DBI_Include_Target_Task,

	HPSI_DBI_Owner_Segment_Task,
	HPSI_DBI_Owner_Alternate_Segment_Task,
	HPSI_DBI_Owner_Traverse_Segments_Task,
	HPSI_DBI_Create_Segment_Task,
	HPSI_DBI_Resize_Offscreen_Window_Task,
	HPSI_DBI_Create_Portfolio_Task,
	HPSI_DBI_Create_Root_Segment_Task,
	HPSI_DBI_Create_Image_Definition_Task,
	HPSI_DBI_Create_Texture_Definition_Task,
	HPSI_DBI_Create_CubeMap_Definition_Task,
	HPSI_DBI_Create_Shader_Definition_Task,
	HPSI_DBI_Create_Style_Definition_Task,
	HPSI_DBI_Create_Material_Palette_Definition_Task,
	HPSI_DBI_Create_Glyph_Definition_Task,
	HPSI_DBI_Create_Line_Pattern_Definition_Task,


	HPSI_DBI_Import_Named_Style_Definition_Task,
	HPSI_DBI_Import_Material_Palette_Definition_Task,
	HPSI_DBI_Import_All_Style_Definition_Task,

	HPSI_DBI_Import_Image_Definition_Task,
	HPSI_DBI_Import_All_Image_Definition_Task,

	HPSI_DBI_Import_Texture_Definition_Task,
	HPSI_DBI_Import_All_Texture_Definition_Task,

	HPSI_DBI_Set_Texture_Definition_Options_Task,

	HPSI_DBI_Import_Glyph_Definition_Task,
	HPSI_DBI_Import_All_Glyph_Definition_Task,

	HPSI_DBI_Import_Line_Pattern_Definition_Task,
	HPSI_DBI_Import_All_Line_Pattern_Definition_Task,

	HPSI_DBI_Undefine_Texture_From_Portfolio_Task,
	HPSI_DBI_Undefine_Style_From_Portfolio_Task,
	HPSI_DBI_Undefine_Glyph_From_Portfolio_Task,
	HPSI_DBI_Undefine_Line_Pattern_From_Portfolio_Task,
	HPSI_DBI_Undefine_Image_From_Portfolio_Task,
	HPSI_DBI_Undefine_Definition_Task,

	HPSI_DBI_Insert_Text_With_Encoding_Task,
	HPSI_DBI_Insert_Marker_Task,
	HPSI_DBI_Insert_Polyline_Task,
	HPSI_DBI_Insert_PolyCylinder_Task,
	HPSI_DBI_Insert_Sphere_Task,
	HPSI_DBI_Insert_Circle_Task,
	HPSI_DBI_Insert_Circle_By_Radius_Task,
	HPSI_DBI_Insert_Circular_Arc_Task,
	HPSI_DBI_Insert_Circular_Wedge_Task,
	HPSI_DBI_Insert_Ellipse_Task,
	HPSI_DBI_Insert_Elliptical_Arc_Task,
	HPSI_DBI_Insert_Polygon_Task,
	HPSI_DBI_Insert_Infinite_Line_Task,
	HPSI_DBI_Insert_NURBS_Curve_Task,
	HPSI_DBI_Insert_Cutting_Section_Task,
	HPSI_DBI_Insert_Grid_Task,
	
	HPSI_DBI_Insert_Shell_Task,
	HPSI_DBI_Insert_Mesh_Task,

	HPSI_DBI_Generate_Polyline_From_Geometry_Task,
	HPSI_DBI_Generate_Shell_From_Geometry_Task,

	HPSI_DBI_Set_Line_Attributes_Task,
	HPSI_DBI_UnSet_Line_Attributes_Task,

	HPSI_DBI_UnSet_Edge_Attributes_Task,
	HPSI_DBI_Set_Edge_Attributes_Task,

	HPSI_DBI_Set_Marker_Attributes_Task,
	HPSI_DBI_UnSet_Marker_Attributes_Task,

	HPSI_DBI_Set_Modelling_Matrix_Task,
	HPSI_DBI_Append_Modelling_Matrix_Task,
	HPSI_DBI_UnSet_Modelling_Matrix_Task,

	HPSI_DBI_Set_Texture_Matrix_Task,
	HPSI_DBI_Append_Texture_Matrix_Task,
	HPSI_DBI_UnSet_Texture_Matrix_Task,

	HPSI_DBI_Set_Text_Region_Task,
	HPSI_DBI_UnSet_Text_Region_Task,

	HPSI_DBI_Set_Camera_Task,
	HPSI_DBI_Set_Camera_Projection_Task,
	HPSI_DBI_Set_Camera_Up_Vector_Task,
	HPSI_DBI_Set_Camera_Position_Task,
	HPSI_DBI_Set_Camera_Target_Task,
	HPSI_DBI_Set_Camera_Field_Task,
	HPSI_DBI_Set_Camera_Near_Limit_Task,
	HPSI_DBI_Set_Dolly_Camera_Task,
	HPSI_DBI_Set_Orbit_Camera_Task,
	HPSI_DBI_Set_Pan_Camera_Task,
	HPSI_DBI_Set_Roll_Camera_Task,
	HPSI_DBI_Set_Zoom_Camera_Task,
	HPSI_DBI_UnSet_Camera_Task,

	HPSI_DBI_Set_Handedness_Task,
	HPSI_DBI_UnSet_Handedness_Task,

	HPSI_DBI_UnSet_Performance_Everything_Task,

	HPSI_DBI_Set_Driver_Options_Task,
	HPSI_DBI_UnSet_Driver_Options_Task,

	HPSI_DBI_Define_Highlight_Task,
	HPSI_DBI_UnDefine_Highlight_Task,
	HPSI_DBI_UnDefine_Highlight_With_Options_Task,

	HPSI_DBI_Set_Color_Task,
	HPSI_DBI_UnSet_Color_Task,

	HPSI_DBI_Set_Visibility_Task,
	HPSI_DBI_UnSet_Visibility_Task,

	HPSI_DBI_Set_Text_Position_Task,
	HPSI_DBI_Edit_Text_Task,
	HPSI_DBI_Replace_Text_Task,

	HPSI_DBI_Edit_Sphere_Task,
	HPSI_DBI_Replace_Sphere_Task,

	HPSI_DBI_Set_Priority_Task,
	HPSI_DBI_UnSet_Priority_Task,

	HPSI_DBI_Set_Rendering_Options_Task,
	HPSI_DBI_UnSet_Rendering_Options_Task,

	HPSI_DBI_Set_Attribute_Lock_Task,
	HPSI_DBI_Set_Attribute_Filter_Task,

	HPSI_DBI_Set_Drawing_Attributes_Task,
	HPSI_DBI_UnSet_Drawing_Attributes_Task,

	HPSI_DBI_Set_Window_Color_Task,
	HPSI_DBI_UnSet_Window_Color_Task,

	HPSI_DBI_Set_All_Subwindow_Options_Task,
	HPSI_DBI_UnSet_All_Subwindow_Options_Task,
	HPSI_DBI_UnSet_Window_Background_Task,
	HPSI_DBI_UnSet_Window_Border_Task,
	HPSI_DBI_UnSet_Subwindow_Task,

	HPSI_DBI_Set_Text_Attributes_Task,
	HPSI_DBI_UnSet_Text_Everything_Task,

	HPSI_DBI_UnSet_Text_Font_Task,
	HPSI_DBI_UnSet_Text_Spacing_Task,
	HPSI_DBI_UnSet_Text_Alignment_Task,
	HPSI_DBI_UnSet_Text_Path,

	HPSI_DBI_Set_Selection_Options_Task,

	HPSI_DBI_Set_Heuristics_Task,
	HPSI_DBI_UnSet_Heuristics_Task,

	HPSI_DBI_Set_Clip_Region_Task,
	HPSI_DBI_UnSet_Clip_Region_Task,

	HPSI_DBI_Set_Selectability_Task,
	HPSI_DBI_UnSet_Selectability_Task,

	HPSI_DBI_Insert_Spot_Light_Task,
	HPSI_DBI_Replace_Spot_Light_Task,
	HPSI_DBI_Insert_Distant_Light_Task,
	HPSI_DBI_Replace_Distant_Light_Task,
	HPSI_DBI_Edit_Spotlight_Task,
	HPSI_DBI_Edit_Distant_Light_Task,
	HPSI_DBI_Set_Camera_Relative_Light_Task,

	HPSI_DBI_Edit_Circle_By_Radius_Task,
	HPSI_DBI_Replace_Circle_Task,
	
	HPSI_DBI_Edit_Circular_Arc_Task,
	HPSI_DBI_Replace_Circular_Arc_Task,
	
	HPSI_DBI_Edit_Circular_Wedge_Task,
	HPSI_DBI_Replace_Circular_Wedge_Task,

	HPSI_DBI_Edit_Cutting_Section_Task,
	HPSI_DBI_Replace_Cutting_Section_Task,
	HPSI_DBI_Set_Cutting_Section_Visualization_Task,

	HPSI_DBI_Edit_Ellipse_Task,
	HPSI_DBI_Replace_Ellipse_Task,
			
	HPSI_DBI_Edit_Elliptical_Arc_Task,
	HPSI_DBI_Replace_Elliptical_Arc_Task,

	HPSI_DBI_Edit_Infinite_Line_Task,
	HPSI_DBI_Replace_Infinite_Line_Task,

	HPSI_DBI_Edit_Polyline_Task,
	HPSI_DBI_Replace_Polyline_Task,

	HPSI_DBI_Edit_Polygon_Task,
	HPSI_DBI_Replace_Polygon_Task,

	HPSI_DBI_Edit_Marker_Task,
	HPSI_DBI_Replace_Marker_Task,

	HPSI_DBI_Edit_NURBS_Curve_Task,
	HPSI_DBI_Edit_NURBS_Curve_Parameters_Task,
	HPSI_DBI_Replace_NURBS_Curve_Task,

	HPSI_DBI_Edit_Shell_Points_Task,
	HPSI_DBI_Edit_Shell_Faces_Task,
	HPSI_DBI_Replace_Shell_Task,
	HPSI_DBI_Optimize_Shell_Task,
	HPSI_DBI_Compute_Shell_Relation_Task,

	HPSI_DBI_Replace_NURBS_Surface_Task,
	HPSI_DBI_Insert_NURBS_Surface_Task,
	HPSI_DBI_Edit_NURBS_Surface_Task,
	HPSI_DBI_Set_NURBS_Surface_Trims_Task,
	HPSI_DBI_UnSet_NURBS_Surface_Trims_Task,

	HPSI_DBI_Edit_Mesh_Task,
	HPSI_DBI_Replace_Mesh_Task,

	HPSI_DBI_Edit_PolyCylinder_Points_Task,
	HPSI_DBI_Edit_PolyCylinder_Radii_Task,
	HPSI_DBI_Replace_PolyCylinder_Task,

	HPSI_DBI_Replace_Grid_Task,
	HPSI_DBI_Edit_Grid_Task,

	HPSI_DBI_MSet_Vertex_Colors_By_Value_Task,
	HPSI_DBI_MSet_Spec_Vert_Col_By_Value_Task,
	HPSI_DBI_MSet_Vertex_Colors_By_FIndex_Task,
	HPSI_DBI_MSet_Spec_Vert_Col_By_FIndex_Task,
	HPSI_DBI_MUnSet_Vertex_Colors_Task,
	HPSI_DBI_MUnSet_Spec_Vertex_Colors_Task,


	HPSI_DBI_MSet_Face_Colors_By_Value_Task,
	HPSI_DBI_MSet_Spec_Face_Colors_By_Value_Task,
	HPSI_DBI_MSet_Face_Colors_By_FIndex_Task,
	HPSI_DBI_MSet_Spec_Face_Colors_By_FIndex_Task,
	HPSI_DBI_MUnSet_Face_Colors_Task,
	HPSI_DBI_MUnSet_Spec_Face_Colors_Task,

	HPSI_DBI_Set_User_Data_Task,
	HPSI_DBI_Set_Bulk_User_Data_Task,
	HPSI_DBI_UnSet_User_Data_Task,
	HPSI_DBI_UnSet_All_User_Data_Task,

	HPSI_DBI_MSet_Vertex_Normals_Task,
	HPSI_DBI_MSet_Specific_Vertex_Normals_Task,

	HPSI_DBI_MUnSet_Vertex_Normals_Task,
	HPSI_DBI_MUnSet_Specific_Vertex_Normals_Task,

	HPSI_DBI_MSet_Face_Normals_Task,
	HPSI_DBI_MSet_Spec_Face_Normals_Task,

	HPSI_DBI_MUnSet_Face_Normals_Task,
	HPSI_DBI_MUnSet_Spec_Face_Normals_Task,

	HPSI_DBI_MSet_Vertex_Parameters_Task,
	HPSI_DBI_MSet_Specific_Vertex_Parameters_Task,

	HPSI_DBI_MUnSet_Vertex_Parameters_Task,
	HPSI_DBI_MUnSet_Specific_Vertex_Parameters_Task,

	HPSI_DBI_MUnSet_Vertex_Visibilities_Task,
	HPSI_DBI_MUnSet_Specific_Vertex_Visibilities_Task,

	HPSI_DBI_Vertex_Visibilities_Task,
	HPSI_DBI_Specific_Vertex_Visibilities_Task,

	HPSI_DBI_MSet_Face_Visibilities_Task,
	HPSI_DBI_MSet_Specific_Face_Visibilities_Task,

	HPSI_DBI_MUnSet_Face_Visibilities_Task,
	HPSI_DBI_MUnSet_Specific_Face_Visibilities_Task,

	HPSI_DBI_UnSet_Mesh_Vertex_Everything_Task,
	HPSI_DBI_UnSet_Mesh_Face_Everything_Task,
	HPSI_DBI_UnSet_Mesh_Everything_Task,

	HPSI_DBI_UnSet_Shell_Vertex_Everything_Task,
	HPSI_DBI_UnSet_Shell_Face_Everything_Task,
	HPSI_DBI_UnSet_Shell_Everything_Task,

	HPSI_DBI_Compute_Text_Extent_By_Path,
	HPSI_DBI_Compute_Transform_By_Path,
	HPSI_DBI_Compute_Selection_Task,
	HPSI_DBI_Compute_Selection_By_Shell_Task,
	HPSI_DBI_Compute_Selection_By_Ray_Task,
	HPSI_DBI_Compute_Selection_By_Volume_Task,

	HPSI_DBI_Move_By_Key_Task,
	HPSI_DBI_Copy_By_Key_Task,
	HPSI_DBI_Optimize_Task,

	HPSI_DBI_Compute_Max_Texture_Size_Task,
	HPSI_DBI_Write_Image_Task,
	HPSI_DBI_Hardcopy_File_Task,

	HPSI_DBI_Rename_Segment_Task,
	HPSI_DBI_Is_Driver_Available_Task,
#endif
};



class Task_Notifier_Data;
typedef	Counted_Pointer_Pooled<Task_Notifier_Data>	Task_Notifier;

class Task : public CMO {

public:

	Task() {killed=false;};
	virtual ~Task();

	virtual void Do_Task(bool exiting)=0;

	virtual Task_Type Type()=0;

	virtual	bool Drop(Task const * test_task) const { UNREFERENCED(test_task); return false; }

	virtual	intptr_t Freshen() const { return 0; }

	HOOPS_API Task_Notifier Notifier() const;

	void	Merged(Task *	merged_task);

	Task * Get_Next() { return next_task; };

private:
	friend class Task_List;
	friend class Task_Queue;

	Task *	next_task;

	mutable Task_Notifier	notifier_;
	bool	killed;
};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable: 4714)	// The given function was selected for inline expansion, but the compiler did not perform the inlining.
#endif

class Task_Notifier_Data : public Counted_Object_Pooled {

public:
	Task_Notifier_Data (Memory_Pool * memory_pool)
		: Counted_Object_Pooled (memory_pool), go(0), waiters(0), also_notifiy(null) {
	}

	~Task_Notifier_Data() {
		if (waiters > 0) {
			ASSERT(0);
			Notify();
			while (waiters>0)
				THREAD_YIELD();
		}

		if (also_notifiy != null)
			also_notifiy->Wait();

	}

	INLINE void Wait() {
		locked_increment((uint32_t *)&waiters);
		while (go == 0)
			THREAD_YIELD();
		locked_decrement((uint32_t *)&waiters);
	}

	INLINE bool IsDone() const {

		if (go > 0) {

			if (also_notifiy != null)
				return also_notifiy->IsDone();

			return true;
		}
		return false;
	}

	INLINE void Notify() {
		go = 1;
	}

private:

	volatile uint32_t go;
	volatile uint32_t waiters;

	friend class Task;
	mutable Task_Notifier	also_notifiy;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

INLINE Task::~Task() {
	if (notifier_ != null)
		notifier_->Notify();
};


class HOOPS_API Task_List : public CMO {

public:

	INLINE Task_List() : tasks(null), tasks_tail(null) {}

	INLINE ~Task_List() {
		// clear task list
		Task *	task;
		while ((task = tasks) != null) {
			tasks = task->next_task;
			task->Do_Task(true);
		}
	}

	void Add_Task(Task * task);

	INLINE Task * Get_Task() {
		Lock();
		Task * task = Get_Task_Unlocked();
		Unlock();
		return task;
	}

	Task * Get_Task_Unlocked();

	INLINE bool Empty() {return (tasks == 0);}

	INLINE void Lock() {task_list_benaphore.Lock();}

	INLINE void Unlock() {task_list_benaphore.Unlock();}

//#define VALIDATE_TASK_LIST
#ifdef VALIDATE_TASK_LIST
	void Validate();
#endif

private:
	Task *				tasks;
	Task *				tasks_tail;
	Benaphore			task_list_benaphore;
};

#define	TQF_NONE			0
#define TQF_HIGHPRIORITY	(1<<0)
#define TQF_INTERNAL		(1<<1)
#define TQF_FRESHEN			(1<<2)

class HOOPS_API Task_Queue : public CMO {

public:

	Task_Queue(const char * name, unsigned char in_flags);

	~Task_Queue();

	void Add_Task(Task * task);

	Task * Get_Task();

	INLINE bool Empty() {return task_list.Empty();}

	INLINE bool Exiting() {return euthanize > 0;}

	INLINE volatile uint32_t ActiveThreadCount() {return active_thread_count;}

	INLINE void ThreadExiting() {locked_decrement((uint32_t *)&euthanize);}

	INLINE bool IsHighPriority() {return ANYBIT(flags, TQF_HIGHPRIORITY);}
	INLINE bool IsInternal() {return ANYBIT(flags, TQF_INTERNAL);}
	INLINE bool IsFreshen() {return ANYBIT(flags, TQF_FRESHEN);}

	INLINE const char * Get_Name() {return &name[0];}

	void Set_Name(const char * name);

private:
	Task_List			task_list;

	Semaphore			task_list_counter;

	HThreadHandle		thread_handle;

	int					flags;
	char				name[32];

	volatile uint32_t	euthanize;
	volatile uint32_t	active_thread_count;
};

End_HOOPS_Namespace

#define TASK_QUEUE_DEFINED
#endif
