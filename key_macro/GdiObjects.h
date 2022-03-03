//!!! 이 파일은 EveR와 SEROPI 로봇에서 공용으로 사용하는 파일입니다.
//!!! 코드를 편집할 때 주의를 요망합니다.

#pragma once

extern HBITMAP HB_LAMP_DIS;
extern HBITMAP HB_LAMP_OFF;
extern HBITMAP HB_LAMP_ON;

extern void InitGdiObjects ();
extern void DeinitGdiObjects ();

extern BOOL IsObjDeleteable(HGDIOBJ hGdiObj);
extern BOOL ReplaceStaticControlBitmap (HWND hDlg, int ControlID, HBITMAP hBitmap);
extern BOOL ReplaceButtonControlBitmap (HWND hDlg, int ControlID, HBITMAP hBitmap);
