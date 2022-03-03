#pragma once

#define WM_TRAY_ICON	(WM_USER+1435)

// Tray에 아이콘화 하여 보여주기위한 함수들 
int  TrayIconCreate  (HWND hWnd, HICON hIcon, char *tip);
void TrayIconClose   ();
void TrayIconChange  (HICON hIcon, char *tip);
void TrayIconMessage (char *title, char *info, int timeout);
