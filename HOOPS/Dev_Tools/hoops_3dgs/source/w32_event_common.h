



template<typename T>
local void w32_check_events (T *	state) 
{
	state->mouse_was_queued = false;

	MSG	msg;
	while (PeekMessage (&msg, state->hWND, 0, 0, PM_NOREMOVE) &&
		GetMessage (&msg, state->hWND, 0, 0)) {
			TranslateMessage (&msg);
			DispatchMessage (&msg);
	}
}

template<typename T>
local void w32_consume_key (
	T *					state,
	UINT 				word_parameter,
	LONG 				long_parameter)
{
	int					key;
	BYTE				virtual_key_states[256];
	int					status = 0;

	// These defines are bases on the MAC defines
#define B_SHIFT			0x00000100
#define B_CONTROL		0x00000200
#define B_NUM_LOCK		0x00000400
#define B_ALT			0x00000800

	// These are unique to the PC and assigned sequentially
#define B_CAPS_LOCK				0x00001000
#define B_SCROLL_LOCK			0x00002000
#define B_LEFT_SHIFT			0x00004000
#define B_RIGB_SHIFT			0x00008000
#define B_RIGB_CONTROL			0x00010000
#define B_LEFT_CONTROL			0x00020000
#define B_RIGB_ALT				0x00040000
#define B_LEFT_ALT				0x00080000


	GetKeyboardState (virtual_key_states);

	if ((virtual_key_states[VK_SHIFT] & 0x80) != 0)
		status |= B_SHIFT;
	if ((virtual_key_states[VK_CONTROL] & 0x80) != 0)
		status |= B_CONTROL;
	if ((virtual_key_states[VK_NUMLOCK] & 0x01) != 0)
		status |= B_NUM_LOCK;
	if ((virtual_key_states[VK_MENU] & 0x80) != 0)
		status |= B_ALT;
	if ((virtual_key_states[VK_CAPITAL] & 0x01) != 0)
		status |= B_CAPS_LOCK;
	if ((virtual_key_states[VK_SCROLL] & 0x01) != 0)
		status |= B_SCROLL_LOCK;
	if ((virtual_key_states[VK_LSHIFT] & 0x80) != 0)
		status |= B_LEFT_SHIFT;
	if ((virtual_key_states[VK_RSHIFT] & 0x80) != 0)
		status |= B_RIGB_SHIFT;
	if ((virtual_key_states[VK_LCONTROL] & 0x80) != 0)
		status |= B_LEFT_CONTROL;
	if ((virtual_key_states[VK_RCONTROL] & 0x80) != 0)
		status |= B_RIGB_CONTROL;
	if ((virtual_key_states[VK_LMENU] & 0x80) != 0)
		status |= B_LEFT_ALT;
	if ((virtual_key_states[VK_RMENU] & 0x80) != 0)
		status |= B_RIGB_ALT;

	/* it's an extended key of some kind */
	switch (word_parameter) {
		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD2:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9: {
			if (status & B_NUM_LOCK)
				key = '0' + word_parameter - VK_NUMPAD0;
			else
				key = FIRST_KEYPAD_KEY + word_parameter - VK_NUMPAD0;
		}	break;

		case VK_INSERT: {
			key = INSERT_KEY;
		}	break;

		case VK_END: {
			key = END_KEY;
		}	break;

		case VK_HOME: {
			key = HOME_KEY;
		}	break;

		case VK_PRIOR: {
			key = PAGE_UP_KEY;
		}	break;

		case VK_NEXT: {
			key = PAGE_DOWN_KEY;
		}	break;

		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
		case VK_F12:
		case VK_F13:
		case VK_F14:
		case VK_F15:
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24: {
			key = FIRST_FUNCTION_KEY + word_parameter - VK_F1 + 1;
		}	break;

		case VK_CLEAR: {
			key = CLEAR_KEY;
		}	break;

		case VK_PAUSE: {
			key = PAUSE_KEY;
		}	break;

		case VK_HELP: {
			key = H_HELP_KEY;
		}	break;

		case VK_TAB: {
			key = TAB_KEY;
		}	break;

		case VK_BACK: {
			key = BACKSPACE_KEY;
		}	break;

		case VK_RETURN: {
			key = RETURN_KEY;
		}	break;

		case VK_ESCAPE: {
			key = ESC_KEY;
		}	break;

		case VK_SPACE: {
			key = SPACE_KEY;
		}	break;

		case VK_DELETE: {
			key = DELETE_KEY;
		}	break;

		case VK_UP: {
			key = UP_ARROW_KEY;
		}	break;

		case VK_DOWN: {
			key = DOWN_ARROW_KEY;
		}	break;

		case VK_LEFT: {
			key = LEFT_ARROW_KEY;
		}	break;

		case VK_RIGHT: {
			key = RIGHT_ARROW_KEY;
		}	break;

		default: {

			char	kb_state[256];
			GetKeyboardState ((LPBYTE)kb_state);

			WORD	key_word;
			UINT	wScanCode = (WORD) LOBYTE (HIWORD (long_parameter));
			if (ToAsciiEx (word_parameter, wScanCode, (PBYTE)kb_state,
				(LPWORD)&key_word, (UINT)0,GetKeyboardLayout(0)) == 1)
				key = (unsigned int) (0xFFL & key_word);
			else
				key = 0;
		} nobreak;
	}

	if (key != 0)
		HI_Queue_Actor_Keyboard_Event (state->dc->actor, key, status);
}





template<typename T>
local void w32_consume_mouse (T * state, UINT event, UINT wParam, LONG lParam)
{
	UNREFERENCED(wParam);

	switch (event) {
		case WM_RBUTTONDOWN: {
			state->mouse_buttons |= HOOPS_RIGHT_MOUSE_BUTTON;
		}	break;
		case WM_MBUTTONDOWN: {
			state->mouse_buttons |= HOOPS_MIDDLE_MOUSE_BUTTON;
		}	break;
		case WM_LBUTTONDOWN: {
			state->mouse_buttons |= HOOPS_LEFT_MOUSE_BUTTON;
		}	break;

		case WM_RBUTTONUP: {
			state->mouse_buttons &= ~HOOPS_RIGHT_MOUSE_BUTTON;
		}	break;
		case WM_MBUTTONUP: {
			state->mouse_buttons &= ~HOOPS_MIDDLE_MOUSE_BUTTON;
		}	break;
		case WM_LBUTTONUP: {
			state->mouse_buttons &= ~HOOPS_LEFT_MOUSE_BUTTON;
		}	break;
	}

	state->mouse_x = LOWORD (lParam);
	state->mouse_y = state->yfudge - HIWORD (lParam);

	HD_Queue_Pixel_Location_Event (state->dc, state->mouse_buttons, state->mouse_x, state->mouse_y);
	state->mouse_was_queued = true;
}


template<typename T>
INLINE T * w32_state_from_dc(Display_Context const * dc)
{
	return static_cast<T*>(dc->data2);
}


template<typename T>
local bool w32_request_location (Display_Context const * dc, int * button, int * x, int * y)
{
	T *	state = w32_state_from_dc<T>(dc);

	if (state != null) {

		if (!state->suppress_check_events)
			w32_check_events (state);
		// This next line is QUITE necessary to refresh the pointer
		state = w32_state_from_dc<T>(dc);

		// This function must always report a mouse "state", so if check events did not report an activity via
		// Queue_Pixel_Location_Event, we must report one here. We will just re-report the last mouse state that we saw.
		if (state == null || state->mouse_was_queued)
			return false;
		else {
			*button = state->mouse_buttons;
			*x = state->mouse_x;
			*y = state->mouse_y;
			return true;
		}
	}

	return false;
}

template<typename T>
local bool w32_request_keyboard (Display_Context const * dc, int * button, int * status)
{
	T *	state = w32_state_from_dc<T>(dc);

	UNREFERENCED(status);
	UNREFERENCED(button);

	if (state != null) {

		if (!state->suppress_check_events)
			w32_check_events (state);
		// This next line is QUITE necessary to refresh the pointer
		
		// !!! well it would be if we did anything else
		//state = w32_state_from_dc<T>(dc);

		// check events will always have reported any keystrokes to Hoops via Queue_Keyboard_Event.  Since Hoops doesn't
		// need to know the "state" of the keyboard, this function can safely return false.
	}
	return false;
}

