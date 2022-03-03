#pragma once


#define EX_KEY_MACRO_ITSELF	0x00010000

//#define WM_RESTART	(WM_USER + 1239)
#define WM_MM_TIMER	(WM_USER + 1250)


inline int BOUND (int value, int lo, int hi)
{
	if (value < lo) value = lo;
	if (value > hi) value = hi;
	return value;
}

inline int ROUND (double x)
{
	if (x > 0.) return (int)(x + 0.5);
	if (x < 0.) return (int)(x - 0.5);
	return 0;
}

#define VK_KEY_UP		0x00010000
#define VK_KEY_CTRL		0x00020000
#define VK_KEY_ALT		0x00040000
#define VK_KEY_SHIFT	0x00080000
#define VK_MODIFIER_KEY	0x00100000

inline int GetCtrlAltShiftKeyState ()
{
	int key_state = 0;

	if (GetAsyncKeyState (VK_LSHIFT)    & 0x8000)	key_state |= VK_KEY_SHIFT;
	if (GetAsyncKeyState (VK_RSHIFT)    & 0x8000)	key_state |= VK_KEY_SHIFT;
	if (GetAsyncKeyState (VK_LCONTROL)  & 0x8000)	key_state |= VK_KEY_CTRL;
	if (GetAsyncKeyState (VK_RCONTROL)  & 0x8000)	key_state |= VK_KEY_CTRL;
	if (GetAsyncKeyState (VK_LMENU)     & 0x8000)	key_state |= VK_KEY_ALT;
	if (GetAsyncKeyState (VK_RMENU)     & 0x8000)	key_state |= VK_KEY_ALT;

	return key_state;
}

extern BYTE MouseButton2VkCode (long flags, bool &vk_pressed);

extern bool IsImeHangul ();
extern void SetImeHangul (bool hangul);
extern bool ApplicationAlreadyExist (char *WindowClass, char *Title);

extern void ScreenDependentMousePos (long &mx, long &my);
extern void ScreenIndependentMousePos (long &mx, long &my);

extern CString GetVersionInfo(CString csEntry);
