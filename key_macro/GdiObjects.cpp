//!!! 이 파일은 EveR와 SEROPI 로봇에서 공용으로 사용하는 파일입니다.
//!!! 코드를 편집할 때 주의를 요망합니다.

#include "stdafx.h"
#include "resource.h"
#include "GdiObjects.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HBITMAP HB_LAMP_DIS;
HBITMAP HB_LAMP_OFF;
HBITMAP HB_LAMP_ON;

HCURSOR HC_MOVE; 
HCURSOR HC_CROSS;


void InitGdiObjects ()
{
	HINSTANCE hInst = AfxGetInstanceHandle();

	COLORMAP colorMap;
	colorMap.from = RGB(0xFF,0xFF,0xFF);
	colorMap.to  = ::GetSysColor( COLOR_BTNFACE );

	HB_LAMP_DIS		= ::CreateMappedBitmap(hInst, IDB_BITMAP_LAMP_DISABLED,	0, &colorMap, 1);
	HB_LAMP_OFF		= ::CreateMappedBitmap(hInst, IDB_BITMAP_LAMP_OFF,		0, &colorMap, 1);
	HB_LAMP_ON		= ::CreateMappedBitmap(hInst, IDB_BITMAP_LAMP_ON,		0, &colorMap, 1); 
}

void DeinitGdiObjects ()
{
	DeleteObject (HB_LAMP_DIS);
	DeleteObject (HB_LAMP_OFF);
	DeleteObject (HB_LAMP_ON);
}

BOOL IsObjDeleteable(HGDIOBJ hGdiObj)
{
	if(	(hGdiObj ==  HB_LAMP_DIS) ||
		(hGdiObj ==  HB_LAMP_OFF) ||
		(hGdiObj ==  HB_LAMP_ON)  ){
		return FALSE;
	}
	return TRUE;
}

BOOL ReplaceStaticControlBitmap (HWND hDlg, int ControlID, HBITMAP hBitmap)
{
	HGDIOBJ hOldObject = (HGDIOBJ)SendDlgItemMessage (hDlg, ControlID, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	if (hOldObject) {
		if (IsObjDeleteable(hOldObject)) {
			::DeleteObject (hOldObject);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ReplaceButtonControlBitmap (HWND hDlg, int ControlID, HBITMAP hBitmap)
{
	HGDIOBJ hOldObject = (HGDIOBJ)SendDlgItemMessage (hDlg, ControlID, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	if (hOldObject) {
		if (IsObjDeleteable(hOldObject)) {
			::DeleteObject (hOldObject);
		}
		return TRUE;
	}
	return FALSE;
}

