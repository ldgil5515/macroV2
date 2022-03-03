#include "stdafx.h"
#include "macro_def.h"
#include "resource.h"
#include "DialogKeyboard.h"
#include "DialogMouse.h"
#include "DialogDelay.h"
#include "DialogMacroRec.h"
#include "DialogMacroEdit.h"
#include "DialogMacroStartStop.h"
#include "DialogKeyMouseStatus.h"
#include "key_macroDlg.h"
#include "common.h"
#include <queue>

using namespace std;

extern CDialogKeyboard  *g_keyDlg;
extern CDialogMouse     *g_mouseDlg;
extern CDialogDelay		*g_delayDlg;
extern CDialogMacroRec  *g_recDlg;
extern CDialogMacroEdit *g_editDlg;
extern CDialogMacroStartStop *g_ssDlg;
extern CDialogKeyMouseStatus *g_statusDlg;
extern Ckey_macroDlg		 *g_macroDlg;


BYTE MouseButton2VkCode_ (WORD buttonFlags, WORD buttonData, bool &vk_pressed);

void OnRawKeyboard (RAWKEYBOARD &keyboard)
{
	/*
	MakeCode - The scan code from the key depression. The scan code for keyboard overrun is KEYBOARD_OVERRUN_MAKE_CODE. 
	Flags - Flags for scan code information. It can be one or more of the following.
		RI_KEY_BREAK1 - The key is up.
		RI_KEY_E0 - This is the left version of the key.
		RI_KEY_E1 - This is the right version of the key.
		RI_KEY_MAKE - The key is down.
	Reserved - Reserved; must be zero. 
	VKey - Windows message compatible virtual-key code. For more information, see Virtual Key Codes. 
	Message - The corresponding window message, for example WM_KEYDOWN, WM_SYSKEYDOWN, and so forth. 
	ExtraInformation - The device-specific additional information for the event. 
	*/

	if (keyboard.ExtraInformation & EX_KEY_MACRO_ITSELF) return;
	/*
	TRACE ("KEYBD: MakeCode=%X, Flags=%X, VKey=%X, Message=%X\n", 
		(int)keyboard.MakeCode, 
		(int)keyboard.Flags, 
		(int)keyboard.VKey, 
		(int)keyboard.Message);
	*/
	BYTE vk_code   = 0;
	BYTE scan_code = (BYTE)keyboard.MakeCode;
	bool vk_pressed= false;

	if (keyboard.Message == WM_KEYDOWN || keyboard.Message == WM_SYSKEYDOWN) {
		vk_code = (BYTE)keyboard.VKey;
		vk_pressed = true;
	}
	if (keyboard.Message == WM_KEYUP || keyboard.Message == WM_SYSKEYUP) {
		vk_code = (BYTE)keyboard.VKey;  
		vk_pressed = false;
	}

	if (g_statusDlg)	g_statusDlg ->OnKeyboardEvent (vk_code, vk_pressed);

	if (g_delayDlg)      g_delayDlg ->OnKeyboardEvent (vk_code, vk_pressed);
	else if (g_keyDlg)   g_keyDlg   ->OnKeyboardEvent (vk_code, vk_pressed);
	else if (g_ssDlg)    g_ssDlg    ->OnkeyMouseEvent (vk_code, vk_pressed, true);
	else if (g_editDlg)  g_editDlg  ->OnKeyMouseEvent (vk_code, vk_pressed, true);
	else if (g_recDlg)   g_recDlg   ->OnKeyboardEvent (vk_code, vk_pressed, scan_code);
	else if (g_macroDlg) g_macroDlg ->OnKeyMouseEvent (vk_code, vk_pressed, true);
}

void OnRawMouse (RAWMOUSE &mouse)
{
	/*
	usFlags - The mouse state. This member can be any reasonable combination of the following. 
		MOUSE_ATTRIBUTES_CHANGED - Mouse attributes changed; application needs to query the mouse attributes.
		MOUSE_MOVE_RELATIVE - Mouse movement data is relative to the last mouse position.
		MOUSE_MOVE_ABSOLUTE1 - Mouse movement data is based on absolute position.
		MOUSE_VIRTUAL_DESKTOP - Mouse coordinates are mapped to the virtual desktop (for a multiple monitor system).
 
	ulButtons - Reserved.
	usButtonFlags - The transition state of the mouse buttons. This member can be one or more of the following values. 
		RI_MOUSE_BUTTON_1_DOWN - Left button changed to down.
		RI_MOUSE_BUTTON_1_UP - Left button changed to up.
		RI_MOUSE_BUTTON_2_DOWN - Right button changed to down.
		RI_MOUSE_BUTTON_2_UP - Right button changed to up.
		RI_MOUSE_BUTTON_3_DOWN - Middle button changed to down.
		RI_MOUSE_BUTTON_3_UP - Middle button changed to up.
		RI_MOUSE_BUTTON_4_DOWN - XBUTTON1 changed to down.
		RI_MOUSE_BUTTON_4_UP - XBUTTON1 changed to up.
		RI_MOUSE_BUTTON_5_DOWN - XBUTTON2 changed to down.
		RI_MOUSE_BUTTON_5_UP - XBUTTON2 changed to up.
		RI_MOUSE_WHEEL (0x0400) - Raw input comes from a mouse wheel. The wheel delta is stored in usButtonData.

	usButtonData - If usButtonFlags is RI_MOUSE_WHEEL, this member is a signed value that specifies the wheel delta. 
	ulRawButtons - The raw state of the mouse buttons. (입력 안들어옴)
	lLastX - The motion in the X direction. This is signed relative motion or absolute motion, depending on the value of usFlags. 
	lLastY - The motion in the Y direction. This is signed relative motion or absolute motion, depending on the value of usFlags. 
	ulExtraInformation - The device-specific additional information for the event. 
	*/
	if (mouse.ulExtraInformation & EX_KEY_MACRO_ITSELF) return;

	POINT pt;
	GetCursorPos(&pt);
	/*
	TRACE ("MOUSE: Flags=%X, ButtonFlags=%X, ButtonData=%X, X=%d, Y=%d, Pos=(%d, %d)\n", 
		(int)mouse.usFlags, 
		(int)mouse.usButtonFlags,
		(int)mouse.usButtonData,
		(int)mouse.lLastX,
		(int)mouse.lLastY,
		(int)pt.x, 
		(int)pt.y);
	*/
	bool vk_pressed = false;
	BYTE vk_code = MouseButton2VkCode_ (mouse.usButtonFlags, mouse.usButtonData, vk_pressed);
	bool move = (mouse.lLastX || mouse.lLastY) ? true : false;
	short wheelDelta = mouse.usButtonData;

	if (g_statusDlg)	 g_statusDlg->OnMouseEvent    (vk_code, vk_pressed, pt);

	if (g_delayDlg)		 g_delayDlg ->OnKeyboardEvent (vk_code, vk_pressed);
	else if (g_mouseDlg) g_mouseDlg ->OnMouseEvent    (pt.x, pt.y);
	else if (g_ssDlg)    g_ssDlg    ->OnkeyMouseEvent (vk_code, vk_pressed, false);
	else if (g_editDlg)  g_editDlg  ->OnKeyMouseEvent (vk_code, vk_pressed, false);
	else if (g_recDlg)   g_recDlg   ->OnMouseEvent    (vk_code, vk_pressed, move, wheelDelta, pt);
	else if (g_macroDlg) g_macroDlg ->OnKeyMouseEvent (vk_code, vk_pressed, false);
}

BYTE MouseButton2VkCode_ (WORD buttonFlags, WORD buttonData, bool &vk_pressed)
{
	BYTE vk_code = 0;
	vk_pressed = false;

	if      (buttonFlags & RI_MOUSE_BUTTON_1_DOWN) { vk_code = VK_LBUTTON;  vk_pressed = true;  }
	else if (buttonFlags & RI_MOUSE_BUTTON_1_UP  ) { vk_code = VK_LBUTTON;  vk_pressed = false; }
	else if (buttonFlags & RI_MOUSE_BUTTON_2_DOWN) { vk_code = VK_RBUTTON;  vk_pressed = true;  }
	else if (buttonFlags & RI_MOUSE_BUTTON_2_UP  ) { vk_code = VK_RBUTTON;  vk_pressed = false; }
	else if (buttonFlags & RI_MOUSE_BUTTON_3_DOWN) { vk_code = VK_MBUTTON;  vk_pressed = true;  }
	else if (buttonFlags & RI_MOUSE_BUTTON_3_UP  ) { vk_code = VK_MBUTTON;  vk_pressed = false; }
	else if (buttonFlags & RI_MOUSE_BUTTON_4_DOWN) { vk_code = VK_XBUTTON1; vk_pressed = true;  }
	else if (buttonFlags & RI_MOUSE_BUTTON_4_UP  ) { vk_code = VK_XBUTTON1; vk_pressed = false; }
	else if (buttonFlags & RI_MOUSE_BUTTON_5_DOWN) { vk_code = VK_XBUTTON2; vk_pressed = true;  }
	else if (buttonFlags & RI_MOUSE_BUTTON_5_UP  ) { vk_code = VK_XBUTTON2; vk_pressed = false; }
	else if (buttonFlags & RI_MOUSE_WHEEL        ) { vk_code = ((short)buttonData > 0) ? 0x0A : 0x0B;  vk_pressed = true; }

	return vk_code;
}
