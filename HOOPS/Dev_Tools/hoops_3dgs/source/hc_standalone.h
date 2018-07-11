
#ifndef HC_STANDALONE_DEFINED

#ifndef HC_DEFINED
#error must include hc.h first
#endif

HC_EXTERNAL void HC_CDECL HC_Queue_Button_Event (
	const char * keyboard_name, const char * button_name, int status);
HC_EXTERNAL void HC_CDECL HC_Queue_Location_Event (
	const char * locater_name, const char * display_name, double x, double y, const char * action_name, int status);
HC_EXTERNAL void HC_CDECL HC_Queue_Related_Selection (
	const char * segment_name, HC_KEY item_key, int offset1, int offset2, int offset3, double xw, double yw, double zw, double xc, double yc, double zc);
HC_EXTERNAL void HC_CDECL HC_Queue_Selection_Event (
	const char * locater_name, const char * display_name, const char * action_name, int status, const char * segment_name, HC_KEY item_key, int offset1, int offset2, int offset3, double xw, double yw, double zw, double xc, double yc, double zc);
HC_EXTERNAL void HC_CDECL HC_Queue_Special_Event (
	const char * type, const char * value);
HC_EXTERNAL void HC_CDECL HC_Queue_String_Event (
	const char * keyboard_name, const char * string, HC_KEY cursor_key);
HC_EXTERNAL void HC_CDECL HC_Queue_Wakeup_Event (
	double req_interval);
HC_EXTERNAL void HC_CDECL HC_Get_Button (
	char * name);
HC_EXTERNAL void HC_CDECL HC_Get_Location (
	float * x, float * y);
HC_EXTERNAL void HC_CDECL HC_Get_Selection (
	char * name);
HC_EXTERNAL void HC_CDECL HC_Get_String (
	const char * context, char * string);
HC_EXTERNAL void HC_CDECL HC_Get_String_With_Encoding (
	const char * context, const char * encoding, void * string);
HC_EXTERNAL void HC_CDECL HC_Get_Wakeup (
	double seconds);
HC_EXTERNAL void HC_CDECL HC_Await_Event (
	char * type);
HC_EXTERNAL int HC_CDECL HC_Check_For_Events (
	void);
HC_EXTERNAL void HC_CDECL HC_Disable_Button_Events (
	const char * keyboardnam, const char * buttonnam);
HC_EXTERNAL void HC_CDECL HC_Disable_Location_Events (
	const char * locaternam, const char * targetnam, const char * action);
HC_EXTERNAL void HC_CDECL HC_Disable_Selection_Events (
	const char * locaternam, const char * targetnam);
HC_EXTERNAL void HC_CDECL HC_Disable_String_Events (
	const char * keyboardnam, HC_KEY cursorkey);
HC_EXTERNAL void HC_CDECL HC_Disable_Wakeup_Events (
	double req_interval);
HC_EXTERNAL void HC_CDECL HC_Enable_Button_Events (
	const char * keyboardnam, const char * buttonnam);
HC_EXTERNAL void HC_CDECL HC_Enable_Location_Events (
	const char * locaternam, const char * targetnam, const char * action);
HC_EXTERNAL void HC_CDECL HC_Enable_Selection_Events (
	const char * locaternam, const char * targetnam);
HC_EXTERNAL void HC_CDECL HC_Enable_String_Events (
	const char * keyboardnam, HC_KEY cursorkey);
HC_EXTERNAL void HC_CDECL HC_Enable_Wakeup_Events (
	double req_interval);
HC_EXTERNAL void HC_CDECL HC_Flush_All_Events (
	void);
HC_EXTERNAL void HC_CDECL HC_Requeue_Event (
	void);
HC_EXTERNAL void HC_CDECL HC_Show_Event (
	char * type);
HC_EXTERNAL void HC_CDECL HC_Show_Special_Event (
	char * type, char * value);

#define HC_STANDALONE_DEFINED
#endif /* HC_STANDALONE_DEFINED */

