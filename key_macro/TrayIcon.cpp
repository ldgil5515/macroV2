#include "stdafx.h"
#include "TrayIcon.h"

#define ID_TRAY_LOAD		4567

static NOTIFYICONDATA _notify_icon_data;

int TrayIconCreate(HWND hWnd, HICON hIcon, char *tip) 
{
	_notify_icon_data.cbSize = sizeof(NOTIFYICONDATA);
	_notify_icon_data.hWnd = hWnd;
	_notify_icon_data.uID = ID_TRAY_LOAD;
	_notify_icon_data.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	_notify_icon_data.hIcon = hIcon;
	_notify_icon_data.uCallbackMessage = WM_TRAY_ICON;
	strncpy (_notify_icon_data.szTip, tip, sizeof(_notify_icon_data.szTip));

	return Shell_NotifyIcon (NIM_ADD, &_notify_icon_data);
}

void TrayIconClose() 
{
	Shell_NotifyIcon (NIM_DELETE, &_notify_icon_data);
}

void TrayIconChange(HICON hIcon, char *tip) 
{
	_notify_icon_data.uFlags = NIF_ICON|NIF_TIP;
	_notify_icon_data.hIcon = hIcon;
	strncpy (_notify_icon_data.szTip, tip, sizeof(_notify_icon_data.szTip));

	Shell_NotifyIcon (NIM_MODIFY, &_notify_icon_data);
}

void TrayIconMessage (char *title, char *info, int timeout)
{
	_notify_icon_data.uFlags = NIF_INFO;
	_notify_icon_data.uTimeout = timeout; //트레이 아이콘이 포커스를 잃은 뒤 사라지기 까지의 시간.
	_notify_icon_data.dwInfoFlags = NIIF_USER | NIIF_NOSOUND; //풍선에 뜰 아이콘을 사용자 지정 아이콘으로.
	strncpy(_notify_icon_data.szInfoTitle, title, sizeof(_notify_icon_data.szInfoTitle) ); //풍선의 타이틀.
	strncpy(_notify_icon_data.szInfo, info, sizeof(_notify_icon_data.szInfo));

	Shell_NotifyIcon (NIM_MODIFY, &_notify_icon_data);
}
