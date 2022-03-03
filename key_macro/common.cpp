#include "stdafx.h"
#include "macro_item.h"
#include <Imm.h>

#pragma comment (lib, "Version.lib")


void ScreenDependentMousePos (long &mx, long &my)
{
	mx = (long)(mx*GetSystemMetrics(SM_CXSCREEN)/65536.);
	my = (long)(my*GetSystemMetrics(SM_CYSCREEN)/65536.);
}

void ScreenIndependentMousePos (long &mx, long &my)
{
	mx = (mx > 0) ? 
		(long)((mx+0.5)*65536./GetSystemMetrics(SM_CXSCREEN)) : 
		(long)((mx-0.5)*65536./GetSystemMetrics(SM_CXSCREEN)) ;
	my = (my > 0) ? 
		(long)((my+0.5)*65536./GetSystemMetrics(SM_CYSCREEN)) : 
		(long)((my-0.5)*65536./GetSystemMetrics(SM_CYSCREEN)) ;
}

BYTE MouseButton2VkCode (long flags, bool &vk_pressed)
{
	BYTE vk_code = 0;
	vk_pressed = false;

	if      (flags & MOUSEEVENTF_LEFTDOWN)  { vk_code = VK_LBUTTON;  vk_pressed = true;  }
	else if (flags & MOUSEEVENTF_LEFTUP)    { vk_code = VK_LBUTTON;  vk_pressed = false; }
	else if (flags & MOUSEEVENTF_RIGHTDOWN) { vk_code = VK_RBUTTON;  vk_pressed = true;  }
	else if (flags & MOUSEEVENTF_RIGHTUP)   { vk_code = VK_RBUTTON;  vk_pressed = false; }
	else if (flags & MOUSEEVENTF_MIDDLEDOWN){ vk_code = VK_MBUTTON;  vk_pressed = true;  }
	else if (flags & MOUSEEVENTF_MIDDLEUP)  { vk_code = VK_MBUTTON;  vk_pressed = false; }
	else if (flags & MOUSEEVENTF_WHEEL)     { vk_code = ((short)HIWORD(flags) > 0) ? 0x0A : 0x0B; vk_pressed = true; }
	else if (flags & MOUSEEVENTF_XDOWN) {
		if (HIWORD(flags) & XBUTTON1)		{ vk_code = VK_XBUTTON1; vk_pressed = true;  }
		if (HIWORD(flags) & XBUTTON2)		{ vk_code = VK_XBUTTON2; vk_pressed = true;  }
	}
	else if (flags & MOUSEEVENTF_XUP) {
		if (HIWORD(flags) & XBUTTON1)		{ vk_code = VK_XBUTTON1; vk_pressed = false; }
		if (HIWORD(flags) & XBUTTON2)		{ vk_code = VK_XBUTTON2; vk_pressed = false; }
	}
	return vk_code;
}


bool IsImeHangul()
{
	HWND hWnd = ::GetForegroundWindow();
	if (hWnd) {
		HWND hWndIme = ImmGetDefaultIMEWnd(hWnd);
		if (hWndIme) {
			int ret = ::SendMessage(hWndIme, WM_IME_CONTROL, 0x5, 0);
			return ret ? true : false;
		}
	}
	return false;
}

void SetImeHangul (bool hangul)
{
	for (int i=0; i<10; i++) {
		if (IsImeHangul() != hangul) {
			keybd_event (VK_HANGUL, MapVirtualKey (VK_HANGUL, MAPVK_VK_TO_VSC), KEYEVENTF_EXTENDEDKEY, EX_KEY_MACRO_ITSELF);
			Sleep (33);
			keybd_event (VK_HANGUL, MapVirtualKey (VK_HANGUL, MAPVK_VK_TO_VSC), KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, EX_KEY_MACRO_ITSELF);
			Sleep (33);
		}
	}
}

bool ApplicationAlreadyExist (char *WindowClass, char *Title)
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, Title);   // Create mutex
	switch(GetLastError()) {
	case ERROR_SUCCESS:			break;
	case ERROR_ALREADY_EXISTS:	return true;
	default:					return false;
	}
	return false;
}

CString GetVersionInfo(CString csEntry)
{
	CString csRet = "";

	HRSRC hResInfo = FindResource (NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	
	if (hResInfo) {
		HGLOBAL hResData = LoadResource (NULL, hResInfo);
		DWORD dwSize = SizeofResource (NULL, hResInfo);

		if (hResData) {
			LPVOID pRes = LockResource (hResData);

			if (pRes) {
				// 리소스를 직접 엑세스 할 수 없다. 메모리를 복사해 온다.
				LPVOID pResCopy = LocalAlloc(LMEM_FIXED, dwSize+1000);	// dwSize보다 더 크게... VerQueryValue() 함수가 pResCopy 메모리에 뭔가를 쓰는듯
				CopyMemory (pResCopy, pRes, dwSize);
				UnlockResource (hResData);
				FreeResource (hResData);

				UINT vLen;
				LPVOID retbuf = NULL;
				char query[256];

				BOOL retVal = VerQueryValue (pResCopy, "\\VarFileInfo\\Translation", &retbuf, &vLen);
				if (retVal && vLen==4) {
					DWORD langD = *(DWORD *)retbuf;
	
					sprintf(query, "\\StringFileInfo\\%02X%02X%02X%02X\\%s",
						(langD>> 8)&0xFF, (langD>> 0)&0xFF, (langD>>24)&0xFF, (langD>>16)&0xFF, (LPCTSTR)csEntry);
				}
				else {
					sprintf(query, "\\StringFileInfo\\%04X04B0\\%s", GetUserDefaultLangID(), (LPCTSTR)csEntry);
				}

				retVal = VerQueryValue (pResCopy, query, &retbuf, &vLen);
				if (retVal) {
					csRet = (char*)retbuf;
				}

				LocalFree(pResCopy);
			}
		}
	}

	return csRet;
}
