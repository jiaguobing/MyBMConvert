// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef PLEASE_DEFINED

Begin_HOOPS_Namespace

/* Driver Flags */
#define	D_ALIVE						(Driver_Flags)0x00000001
#define	D_DEAD						(Driver_Flags)0x00000002
#define	D_EVENTS_ARE_ENABLED		(Driver_Flags)0x00000004
#define	D_UPDATE_EVENTS_NEEDED		(Driver_Flags)0x00000008
#define	D_CAN_AWAIT_EVENTS			(Driver_Flags)0x00000010
#define	D_UNUSED_1					(Driver_Flags)0x00000020
#define	D_HAS_CURSOR				(Driver_Flags)0x00000040
#define	D_HAS_AUTO_CURSOR			(Driver_Flags)0x00000080
#define	D_HAS_LOCATER				(Driver_Flags)0x00000100
#define	D_HAS_KEYBOARD				(Driver_Flags)0x00000200
#define	D_HAS_WAKEUP				(Driver_Flags)0x00000400
#define	D_HAS_DISPLAY				(Driver_Flags)0x00000800
#define	D_HAS_DEVICES				(Driver_Flags)0x00000F00
#define	D_OMNIPOTENT				(Driver_Flags)0x00000FF0
#define	D_BUTTONS_WERE_CHANGED		(Driver_Flags)0x00001000
#define	D_DEPENDENT					(Driver_Flags)0x00002000
#define	D_MIGHT_GET_EXPOSURES		(Driver_Flags)0x00004000
#define	D_ACTIVE					(Driver_Flags)0x00008000
#define	D_SHUTDOWN					(Driver_Flags)0x00010000

#define	D_HAS_ACTOR_GENERATOR		(Driver_Flags)0x01000000

#define	D_RESET_NEEDED				(Driver_Flags)0x80000000

/* Locater Actions */
#define	LA_NONE			(Locater_Actions)0x00000000
#define	LA_UP			(Locater_Actions)0x00000001
#define	LA_DOWN			(Locater_Actions)0x00000002
#define	LA_STILL_UP		(Locater_Actions)0x00000004
#define	LA_STILL_DOWN	(Locater_Actions)0x00000008
#define	LA_ALL_KINDS	(Locater_Actions)0x0000000F

#define HOOPS_LEFT_MOUSE_BUTTON		0x00000001
#define HOOPS_RIGHT_MOUSE_BUTTON	0x00000002
#define HOOPS_MIDDLE_MOUSE_BUTTON	0x00000004
#define HOOPS_XBUTTON1_MOUSE_BUTTON	0x00000008
#define HOOPS_XBUTTON2_MOUSE_BUTTON 0x00000010



/* pleezes (requests of the driver) */
enum Please_Types {
	PLEASE_INITIALIZE,
	PLEASE_FINISH,
	PLEASE_UPDATE_DISPLAY,
	PLEASE_DECLARE,
	PLEASE_FREE,
	PLEASE_POLL_KEYBOARD,
	PLEASE_POLL_LOCATER,
	PLEASE_SELECT,

	PLEASE_ENABLE_UP,
	PLEASE_DISABLE_UP,
	PLEASE_ENABLE_DOWN,
	PLEASE_DISABLE_DOWN,
	PLEASE_ENABLE_STILL_UP,
	PLEASE_DISABLE_STILL_UP,
	PLEASE_ENABLE_STILL_DOWN,
	PLEASE_DISABLE_STILL_DOWN,
	PLEASE_ENABLE_KEYBOARD,
	PLEASE_DISABLE_KEYBOARD,

	PLEASE_ENABLE_CURSOR,
	PLEASE_DISABLE_CURSOR,
	PLEASE_MOVE_CURSOR,
	PLEASE_ENABLE_AUTO_CURSOR,
	PLEASE_DISABLE_AUTO_CURSOR,

	PLEASE_REENABLE_KEYBOARD,
	PLEASE_GATHER_DEVICE_INFO,
	PLEASE_FLUSH_EVENTS,

	PLEASE_CHECK_FOR_EXPOSURES,
	PLEASE_GATHER_FONT_NAMES,
	PLEASE_GATHER_FONT_INFO,
	PLEASE_GATHER_WINDOW_EXTENT,
	PLEASE_SHOW_WINDOW_INFO,

	PLEASE_GATHER_SCREEN_EXTENT,
	PLEASE_DESTROY_OCCLUSION_QUERY,

	PLEASE_CLEAN_SEGMENT_CACHE,

	PLEASE_SHOW_IMAGE_DATA,
	PLEASE_SHOW_AVAILABILITY,	// it is available if it is built in or loadable
	PLEASE_SHOW_USEABLITY,		// too the best of our knowledge it will working

	PLEASE_SNAPSHOT,
	PLEASE_IM_CALL,
	PLEASE_GATHER_CAPPING,
};

/*
	PLEASE_DECLARE
	Called at Set_Driver time; the driver should do things that
	need to be done extremely early, like defining new Driver Options.
	The driver should NOT do anything to a device yet.

	The return status is ignored.

	PLEASE_INITIALIZE
	Called at the first Update for the driver. The driver should
	allocate memory and initialize the device.

	If the return status is False then the driver will be marked
	dead and will not be called again (except for PLEASE_FREE).

	PLEASE_UPDATE_ACTOR_LIST
	If the driver needs to do anything in response to a structural
	change in the database, it should do it here. This is independent
	of whether the database change implies a display change.

	If the return is False, this request will not be made of the
	driver again.

	PLEASE_UPDATE_DISPLAY
	The driver will do whatever it takes to bring the screen display
	up to date.

	If the return is False, this request will not be made again nor
	will any cursor request be made.

	PLEASE_FINISH
	The driver should shut down the display. May be called from
	Reset_System, and so freeing memory may not be a good idea.

	The return status is ignored.

	PLEASE_FREE
	Occurs when the last traces of the particular driver are removed
	from the database, if driver_data is not null. The driver should
	free all its memory and return.

	The return status is ignored.


	The driver should return False to indicate that it can't wait on
	the particular set of events currently enabled.

	PLEASE_POLL
	The driver should check the enabled input for devices, and ask for
	events to be queued appropriately.

		For PLEASE_AWAIT_EVENTS and PLEASE_POLL, the driver can get away
		with queueing "STILL_DOWN" events even if they really should be
		"DOWN" events: the Queue_Location_Event routine will observe
		what's going on. Likewise for STILL_UP and UP.

	This function will only be called if input devices have previously
	been successfully enabled. It must return True.

	PLEASE_SELECT
	The driver should find a partially filled-in "Selection_Event"
	structure passed in, and should do a Hit-Test.

	If something is successfully selected, the Selection_Event
	structure should be completed and the return status should
	be True. (The caller will actually queue the event.)

	PLEASE_ENABLE_UP - PLEASE_DISABLE_UP
	PLEASE_ENABLE_DOWN - PLEASE_DISABLE_DOWN
	PLEASE_ENABLE_STILL_UP - PLEASE_DISABLE_STILL_UP
	PLEASE_ENABLE_STILL_DOWN - PLEASE_DISABLE_STILL_DOWN
	PLEASE_ENABLE_KEYBOARD - PLEASE_DISABLE_KEYBOARD
		 The driver should start/stop thinking about generating events of
		 the specified types. (Attempting to queue non-enabled events does
		 not actually hurt anything though.) The actual event checking and
		 queueing will occur on subsequent PLEASE_AWAIT_EVENTS or
		 PLEASE_POLL requests.

	If the return status should be False if the driver does not have
	any device of the appropriate type.


	PLEASE_ENABLE_AUTO_CURSOR - PLEASE_DISABLE_AUTO_CURSOR
	The driver should start/stop tracking a cursor automatically on its
	screen in synch with its own locater device.

	If the return is False, the driver cannot track its cursor itself.

	PLEASE_ENABLE_CURSOR - PLEASE_DISABLE_CURSOR
	The driver should get ready to receive cursor coordinates coming
	from somewhere else.

	If the driver return False, then no cursor is possible (the user
	is warned) and no selection requests will occur.

	PLEASE_MOVE_CURSOR
	The driver should actually update the previously-enabled soft
	cursor. The relevant numbers are in a Location_Event structure
	passed in.

	The return must be True.

	PLEASE_CHECK_FOR_EXPOSURES
	The window-system driver should check for window-system exposure,
	move, resize, etc events, and call the appropriate HD_ routines.

	The False is returned, the system is not a window system or there
	is no need to check for exposures.



	PLEASE_SHOW_IMAGE_DATA
	Requests that the driver write its GPU copy of image data into
	the data pointer of an Image (for example, so that it can be 
	shown and/or written to a file.

	The return should always be True.
*/

/* return value from driver indicates whether it was able to accommodate
   the request. */

/* note: various Ascii-machine drivers assume that the definitions
	 up to and including 'DELETE_KEY' are in the usual Ascii order.
 */


/* NOTE: enables for function-type keys are referenced as
		 256+(CODE&0xFF).  There is currently no enable mechanism
		 for keys other than function & ASCII
*/

#define ENABLEINDEX(code)	(((code)&0xFF00)==0x8000?0x0100|((code)&0x00FF):(code)&0xFF00?0:(code))

#define	FIRST_KEY			0
#define	FIRST_CONTROL_KEY	(Button)0
#define	LAST_CONTROL_KEY	(Button)0x1F
#define	FIRST_PRINTING_KEY	(Button)0x20 /* include space */
#define	LAST_PRINTING_KEY	(Button)0x7E
#define	DELETE_KEY			(Button)0x7F
#define	FIRST_PRINTING_KEY2	(Button)0xA1
#define	LAST_PRINTING_KEY2	(Button)0xFF

#define ESC_KEY				(Button)0x1b
#define RETURN_KEY			(Button)0x0d
#define TAB_KEY				(Button)0x09
#define SPACE_KEY			(Button)0x20
#define BACKSPACE_KEY		(Button)0x08
#define	PAUSE_KEY			(Button)0x13
#define	CLEAR_KEY			(Button)0x0B

#define	FIRST_ARROW_KEY		LEFT_ARROW_KEY
#define	LEFT_ARROW_KEY		(Button)0x8001
#define	UP_ARROW_KEY		(Button)0x8002
#define	RIGHT_ARROW_KEY		(Button)0x8003
#define	DOWN_ARROW_KEY		(Button)0x8004
#define	LAST_ARROW_KEY		DOWN_ARROW_KEY

#define	FIRST_MISC_KEY		INSERT_KEY
#define	INSERT_KEY			(Button)0x8005
#define	HOME_KEY			(Button)0x8006
#define	PAGE_UP_KEY			(Button)0x8007
#define	PAGE_DOWN_KEY		(Button)0x8008
#define	END_KEY				(Button)0x8009
#define	H_HELP_KEY			(Button)0x8010
#define	LAST_MISC_KEY		H_HELP_KEY

#define	FIRST_KEYPAD_KEY	(Button)0x8020
#define	LAST_KEYPAD_KEY		(Button)0x805F

#define	FIRST_FUNCTION_KEY	(Button)0x8060
#define	LAST_FUNCTION_KEY	(Button)0x809F

#define	LAST_KEY			511

#define	CONTROL(x)			(Button)(x - '@' + FIRST_CONTROL_KEY)


struct Gather_Device_Info {
	Name	item;			// what's wanted
	Name	data;			// result
};

struct Gather_Font_Names {
	bool						generic;
	bool						specific;
	bool						families;
	bool						defined;
	Gather_Font_Names(bool generic, bool specific, bool families, bool defined):
		generic(generic), specific(specific), families(families), defined(defined) {}
	
	std::vector<Name_Const, CMO_Allocator<Name_Const> >	names;
	
	void gather_font(Name_Const const & font_name) {
		names.push_back(font_name);
	}
	void sort() {
		std::sort(names.begin(), names.end());
	}

};

#define GFI_EXISTS				0
#define GFI_SIZES				1
#define GFI_POINTS_TO_SRU		2
#define GFI_PROPORTIONAL		3
#define GFI_SCALEABLE			4
#define GFI_MAX_CHARACTER		5
#define GFI_GENERIC_NAME		6
#define GFI_CHARACTER_WIDTHS	7
#define GFI_TEXT_EXTENT			8
#define GFI_ROTATABLE			9
#define GFI_WIDTH_SCALEABLE		10
#define GFI_SLANTABLE			11
#define GFI_DEFINED				12
#define GFI_ENCODING			13
#define GFI_TEXT_OUTLINE		14
#define GFI_OUTLINEABLE			15
#define GFI_FAMILY_NAME			16


struct Gather_Font_Info {

	Name					specified;
	char *					returnbuf;
	Point *					points;
	int *					face_list;
	Segment const * const *	path;
	Attribute const *		attributes;

	KName				text;	// for TEXT_EXTENT
	Text const *		dbtext; // for TEXT_EXTENT

	int					item;	// what's wanted

	// for most things...
	Generic_Font_Name	generic;	// avoid database.h - don't


	float				height;	// for GFI_CHARACTER_WIDTHS
	
	int					returnlen;
	Text_Encoding		encoding;

	/* for TEXT_OUTLINE */
	int	point_count;

	int	face_list_length;

	// for TEXT_EXTENT
	float	xfrac;
	float	yfrac;
	float	x_width;
	float	x_height;
	float	left;
	float	right;
	float	bottom;
	float	top;

	int		segcount;	
};


struct Gather_Window_Extent {
	Segment *				context;
	Segment const * const *	path;
	int						segcount;

	IntRectangle			window_pixels;
	Int_XY					screen_pixels;
	Vector2D				resolution;
};

struct Gather_Screen_Extent : public Rectangle {
	Segment const * const *	path;
	int						segcount;
	Type_Flags				visibility;
	HFSB_Flags				detailed;
	bool					visibility_specified;
};



#define GCG_Segment_Only		0x0001
#define GCG_Ignore_Matrix		0x0002
#define GCG_Refine_Cuts			0x0004

struct Gather_Capping_Geometry {
	Segment  *				target;
	Segment const *			segment;
	Include_Path			path;
	Geometry const *		geometry;
	Cutting_Plane const *	cutter;
	int						flags;
	int						offset;

	int						gathered;
};


struct Selection_Sample {
	IntRectangle			area;
	Selection_ID *			ids;
	uint32_t				id_count;
	uint32_t				current;

	int						highlight_action;
};

struct Snapshot_Info {
	int						width;
	int						height;
	int						bpp;
	unsigned char *			bytes;
};

struct IM_Call_Info {
	intptr_t				result;
	int						request;
};

End_HOOPS_Namespace

#define PLEASE_DEFINED
#endif

