// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HC_STANDALONE_PROTO_DEFINED

HC_INTERFACE void HC_CDECL HC_Queue_Button_Event (char const *keyboard_name, char const *button_name, int status);
HC_INTERFACE void HC_CDECL HC_Queue_Location_Event (char const *locater_name,
		 char const *display_name,
		 double x,
		 double y,
		 char const *action_name,
		 int status);
HC_INTERFACE void HC_CDECL HC_Queue_Related_Selection (char const *segment_name,
		 Key item_key,
		 int offset1,
		 int offset2,
		 int offset3,
		 double xw,
		 double yw,
		 double zw,
		 double xc,
		 double yc,
		 double zc);
HC_INTERFACE void HC_CDECL HC_Queue_Selection_Event (char const *locater_name,
		 char const *display_name,
		 char const *action_name,
		 int status,
		 char const *segment_name,
		 Key item_key,
		 int offset1,
		 int offset2,
		 int offset3,
		 double xw,
		 double yw,
		 double zw,
		 double xc,
		 double yc,
		 double zc);
HC_INTERFACE void HC_CDECL HC_Queue_Special_Event (char const *type, char const *value);
HC_INTERFACE void HC_CDECL HC_Queue_String_Event (char const *keyboard_name, char const *string, Key cursor_key);
HC_INTERFACE void HC_CDECL HC_Queue_Wakeup_Event (double req_interval);

HC_INTERFACE void HC_CDECL HC_Get_Button (char *name);
HC_INTERFACE void HC_CDECL HC_Get_Location (float *x, float *y);
HC_INTERFACE void HC_CDECL HC_Get_Selection (char *name);
HC_INTERFACE void HC_CDECL HC_Get_String (char const *context, char *string);
HC_INTERFACE void HC_CDECL HC_Get_String_With_Encoding (char const *context, char const *encoding, void *string);
HC_INTERFACE void HC_CDECL HC_Get_Wakeup (double seconds);

HC_INTERFACE void HC_CDECL HC_Show_Special_Event (char *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_Event (char *type);
HC_INTERFACE void HC_CDECL HC_Requeue_Event (void);
HC_INTERFACE void HC_CDECL HC_Flush_All_Events (void);
HC_INTERFACE void HC_CDECL HC_Await_Event (char *type);
HC_INTERFACE int HC_CDECL HC_Check_For_Events (void);
HC_INTERFACE void HC_CDECL HC_Disable_Button_Events (char const *keyboardnam, char const *buttonnam);
HC_INTERFACE void HC_CDECL HC_Disable_Location_Events (char const *locaternam, char const *targetnam, char const *action);
HC_INTERFACE void HC_CDECL HC_Disable_Selection_Events (char const *locaternam, char const *targetnam);
HC_INTERFACE void HC_CDECL HC_Disable_String_Events (char const *keyboardnam, Key cursorkey);
HC_INTERFACE void HC_CDECL HC_Disable_Wakeup_Events (double req_interval);
HC_INTERFACE void HC_CDECL HC_Enable_Button_Events (char const *keyboardnam, char const *buttonnam);
HC_INTERFACE void HC_CDECL HC_Enable_Location_Events (char const *locaternam, char const *targetnam, char const *action);
HC_INTERFACE void HC_CDECL HC_Enable_Selection_Events (char const *locaternam, char const *targetnam);
HC_INTERFACE void HC_CDECL HC_Enable_String_Events (char const *keyboardnam, Key cursorkey);
HC_INTERFACE void HC_CDECL HC_Enable_Wakeup_Events (double req_interval);


#define HC_STANDALONE_PROTO_DEFINED 1
#endif /* HC_STANDALONE_PROTO_DEFINED */


