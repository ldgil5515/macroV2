#include "stdafx.h"
#include "key_macro.h"
#include "virtual_key.h"
#include "DialogMacroStartStop.h"
#include "common.h"


static COLORREF _start_color = 0;
static COLORREF _stop_color = 0;

CDialogMacroStartStop  *g_ssDlg = NULL;


IMPLEMENT_DYNAMIC(CDialogMacroStartStop, CDialog)

CDialogMacroStartStop::CDialogMacroStartStop(CWnd* pParent /*=NULL*/)
	: CDialogCommon(CDialogMacroStartStop::IDD, pParent)
{
	_macro = NULL;
	g_ssDlg = this;
}

CDialogMacroStartStop::~CDialogMacroStartStop()
{
	g_ssDlg = NULL;
}

void CDialogMacroStartStop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogMacroStartStop, CDialog)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_START_R, &CDialogMacroStartStop::OnEnChangeEditStartR)
	ON_EN_CHANGE(IDC_EDIT_START_G, &CDialogMacroStartStop::OnEnChangeEditStartG)
	ON_EN_CHANGE(IDC_EDIT_START_B, &CDialogMacroStartStop::OnEnChangeEditStartB)
	ON_EN_CHANGE(IDC_EDIT_STOP_R, &CDialogMacroStartStop::OnEnChangeEditStopR)
	ON_EN_CHANGE(IDC_EDIT_STOP_G, &CDialogMacroStartStop::OnEnChangeEditStopG)
	ON_EN_CHANGE(IDC_EDIT_STOP_B, &CDialogMacroStartStop::OnEnChangeEditStopB)
	ON_BN_CLICKED(IDC_CHECK_START_PUSH, &CDialogMacroStartStop::OnBnClickedCheckStartOption)
	ON_BN_CLICKED(IDC_CHECK_STOP_PUSH, &CDialogMacroStartStop::OnBnClickedCheckStopOption)
	ON_BN_CLICKED(IDC_CHECK_START_KEY, &CDialogMacroStartStop::OnBnClickedCheckStartKey)
	ON_BN_CLICKED(IDC_CHECK_STOP_KEY, &CDialogMacroStartStop::OnBnClickedCheckStopKey)
	ON_BN_CLICKED(IDC_CHECK_REC_TIME_DELAY, &CDialogMacroStartStop::OnBnClickedCheckTime)
	ON_BN_CLICKED(IDC_CHECK_START_COLOR, &CDialogMacroStartStop::OnBnClickedCheckStartColor)
	ON_BN_CLICKED(IDC_CHECK_STOP_COLOR, &CDialogMacroStartStop::OnBnClickedCheckStopColor)
	ON_BN_CLICKED(IDC_BUTTON_START_COLOR_PICK, &CDialogMacroStartStop::OnBnClickedButtonStartColorPick)
	ON_BN_CLICKED(IDC_BUTTON_STOP_COLOR_PICK, &CDialogMacroStartStop::OnBnClickedButtonStopColorPick)
	ON_BN_CLICKED(IDOK, &CDialogMacroStartStop::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogMacroStartStop::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_START_MODIFIER, &CDialogMacroStartStop::OnBnClickedCheckStartModifier)
	ON_BN_CLICKED(IDC_CHECK_STOP_MODIFIER, &CDialogMacroStartStop::OnBnClickedCheckStopModifier)
END_MESSAGE_MAP()


BOOL CDialogMacroStartStop::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	char message[1024+1];
	_snprintf (message, 1024, "%s를 눌러 화면 색인식 시작 색과 마우스 위치를 캡쳐합니다.", GetVkDescFromVK(g_ini.keyMacroColorStart));
	message[1024] = '\0';
	SetDlgItemText (IDC_STATIC_MESSAGE1, message);

	_snprintf (message, 1024, "%s를 눌러 화면 색인식 중지 색과 마우스 위치를 캡쳐합니다.", GetVkDescFromVK(g_ini.keyMacroColorStop));
	message[1024] = '\0';
	SetDlgItemText (IDC_STATIC_MESSAGE2, message);

	FillComboBoxVkList (IDC_COMBO_START_KEY,    _macro->start_key & 0xFF);
	FillComboBoxVkList (IDC_COMBO_STOP_KEY,     _macro->stop_key  & 0xFF);

	CheckDlgButton (IDC_CHECK_AUTO_START,       (_macro->options   & MACRO_AUTO_SS    )	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_START_KEY,        (_macro->options   & MACRO_KEY_START  )	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_STOP_KEY,         (_macro->options   & MACRO_KEY_STOP	  )	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_REC_TIME_DELAY,             (_macro->options   & MACRO_TIMER_SS   )	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_START_COLOR,      (_macro->options   & MACRO_COLOR_START)	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_STOP_COLOR,       (_macro->options   & MACRO_COLOR_STOP )	? BST_CHECKED : BST_UNCHECKED);

	CheckDlgButton (IDC_CHECK_START_PUSH,	   !(_macro->start_key & VK_KEY_UP		) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_START_CTRL,		(_macro->start_key & VK_KEY_CTRL	) ? BST_CHECKED : BST_UNCHECKED);	
	CheckDlgButton (IDC_CHECK_START_ALT,		(_macro->start_key & VK_KEY_ALT		) ? BST_CHECKED : BST_UNCHECKED);	
	CheckDlgButton (IDC_CHECK_START_SHIFT,		(_macro->start_key & VK_KEY_SHIFT	) ? BST_CHECKED : BST_UNCHECKED);	
	CheckDlgButton (IDC_CHECK_START_MODIFIER,	(_macro->start_key & VK_MODIFIER_KEY) ? BST_CHECKED : BST_UNCHECKED);	
	
	CheckDlgButton (IDC_CHECK_STOP_PUSH,       !(_macro->stop_key  & VK_KEY_UP		) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_STOP_CTRL,		(_macro->stop_key  & VK_KEY_CTRL	) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_STOP_ALT,			(_macro->stop_key  & VK_KEY_ALT		) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_STOP_SHIFT,		(_macro->stop_key  & VK_KEY_SHIFT	) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_STOP_MODIFIER,	(_macro->stop_key  & VK_MODIFIER_KEY) ? BST_CHECKED : BST_UNCHECKED);
	
	OnBnClickedCheckStartOption();
	OnBnClickedCheckStopOption();

	int flags_start = (_macro->options>> 8)&0x03;
	int flags_stop  = (_macro->options>>10)&0x03;
	FillComboBoxNumber (IDC_COMBO_START_HOUR, 24, (flags_start >= 3) ? (_macro->start_time/(60*60))%24 + 1 : 0);
	FillComboBoxNumber (IDC_COMBO_START_MIN , 60, (flags_start >= 2) ? (_macro->start_time/(   60))%60 + 1 : 0);
	FillComboBoxNumber (IDC_COMBO_START_SEC , 60, (flags_start >= 1) ? (_macro->start_time/(    1))%60 + 1 : 0);
	FillComboBoxNumber (IDC_COMBO_STOP_HOUR , 24, (flags_stop  >= 3) ? (_macro->stop_time /(60*60))%24 + 1 : 0);
	FillComboBoxNumber (IDC_COMBO_STOP_MIN  , 60, (flags_stop  >= 2) ? (_macro->stop_time /(   60))%60 + 1 : 0);
	FillComboBoxNumber (IDC_COMBO_STOP_SEC  , 60, (flags_stop  >= 1) ? (_macro->stop_time /(    1))%60 + 1 : 0);

	_start_color = _macro->start_color & 0x00FFFFFF;
	_stop_color  = _macro->stop_color  & 0x00FFFFFF;

	SetDlgItemInt (IDC_EDIT_START_R, GetRValue (_start_color),  FALSE);
	SetDlgItemInt (IDC_EDIT_START_G, GetGValue (_start_color),  FALSE);
	SetDlgItemInt (IDC_EDIT_START_B, GetBValue (_start_color),  FALSE);
	SetDlgItemInt (IDC_EDIT_START_X, LOWORD (_macro->start_xy), FALSE);
	SetDlgItemInt (IDC_EDIT_START_Y, HIWORD (_macro->start_xy), FALSE);

	SetDlgItemInt (IDC_EDIT_STOP_R, GetRValue (_stop_color),  FALSE);
	SetDlgItemInt (IDC_EDIT_STOP_G, GetGValue (_stop_color),  FALSE);
	SetDlgItemInt (IDC_EDIT_STOP_B, GetBValue (_stop_color),  FALSE);
	SetDlgItemInt (IDC_EDIT_STOP_X, LOWORD (_macro->stop_xy), FALSE);
	SetDlgItemInt (IDC_EDIT_STOP_Y, HIWORD (_macro->stop_xy), FALSE);

	SetDlgItemInt (IDC_EDIT_RGB_TOLERANCE, BOUND(_macro->tolerance, 0, 255), FALSE);
	CheckDlgButton (IDC_CHECK_ONCE,	(_macro->options & MACRO_COLOR_ONCE) ? BST_CHECKED : BST_UNCHECKED);

	EnableWindowItem (TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogMacroStartStop::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CRect rect;
	GetDlgItem(IDC_STATIC_START_COLOR)->GetWindowRect (&rect);
	ScreenToClient(rect);

	CBrush brush_start_color (_start_color);
	CBrush brush_stop_color  (_stop_color);

	HGDIOBJ penOld = dc.SelectObject (::GetStockObject (NULL_PEN));
	HGDIOBJ brushOld = dc.SelectObject (brush_start_color);

	dc.Rectangle (rect.left+1, rect.top+1, rect.right-0, rect.bottom-0);

	GetDlgItem(IDC_STATIC_STOP_COLOR)->GetWindowRect (&rect);
	ScreenToClient(rect);

	dc.SelectObject (brush_stop_color);
	dc.Rectangle (rect.left+1, rect.top+1, rect.right-0, rect.bottom-0);

	dc.SelectObject (penOld);
	dc.SelectObject (brushOld);
}

void CDialogMacroStartStop::OnkeyMouseEvent (BYTE vk_code, bool vk_pressed, bool key)
{
	if (IsDlgButtonChecked (IDC_CHECK_START_COLOR) == BST_CHECKED) {
		if (g_ini.keyMacroColorStart == vk_code && vk_pressed) {
			POINT p;
			if (GetCursorPos(&p)) {
				CDC *dc = CDC::FromHandle(::GetDC(NULL));   
				if (dc) {
					COLORREF color = dc->GetPixel (p.x, p.y);
					ReleaseDC (dc);

					SetDlgItemInt (IDC_EDIT_START_R, GetRValue (color), FALSE);
					SetDlgItemInt (IDC_EDIT_START_G, GetGValue (color), FALSE);
					SetDlgItemInt (IDC_EDIT_START_B, GetBValue (color), FALSE);
				}
				SetDlgItemInt (IDC_EDIT_START_X, p.x, FALSE);
				SetDlgItemInt (IDC_EDIT_START_Y, p.y, FALSE);
			}
		}
	}
	if (IsDlgButtonChecked (IDC_CHECK_STOP_COLOR) == BST_CHECKED) {
		if (g_ini.keyMacroColorStop == vk_code && vk_pressed) {
			POINT p;
			if (GetCursorPos(&p)) {
				CDC *dc = CDC::FromHandle(::GetDC(NULL));   
				if (dc) {
					COLORREF color = dc->GetPixel (p.x, p.y);
					ReleaseDC (dc);

					SetDlgItemInt (IDC_EDIT_STOP_R, GetRValue (color), FALSE);
					SetDlgItemInt (IDC_EDIT_STOP_G, GetGValue (color), FALSE);
					SetDlgItemInt (IDC_EDIT_STOP_B, GetBValue (color), FALSE);
				}
				SetDlgItemInt (IDC_EDIT_STOP_X, p.x, FALSE);
				SetDlgItemInt (IDC_EDIT_STOP_Y, p.y, FALSE);
			}
		}
	}
	if (key) {
		if (GetFocus () == GetDlgItem (IDC_COMBO_START_KEY)) {
			if (vk_code && vk_pressed) {
				// 키보드 이벤트를 바로 처리하지 않고 timer를 사용하여 100ms 후에
				// 입력된 가상키로 ComboBox 항목을 선택한다: OnTimer() 함수에서 nIDEvent == 1000 참조
				_vk_start = vk_code;
				SetTimer (1000, 30, NULL);
			}
		}
		if (GetFocus () == GetDlgItem (IDC_COMBO_STOP_KEY)) {
			if (vk_code && vk_pressed) {
				_vk_stop = vk_code;
				SetTimer (1000, 30, NULL);
			}
		}
	}
}

void CDialogMacroStartStop::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1000) {
		if (_vk_start >= 0 && GetFocus() == GetDlgItem(IDC_COMBO_START_KEY)) {
			SendDlgItemMessage (IDC_COMBO_START_KEY, CB_SETCURSEL, GetVkIndex (_vk_start));
		}
		if (_vk_stop  >= 0 && GetFocus() == GetDlgItem(IDC_COMBO_STOP_KEY )) {
			SendDlgItemMessage (IDC_COMBO_STOP_KEY, CB_SETCURSEL, GetVkIndex (_vk_stop));
		}
		_vk_start = -1;
		_vk_stop  = -1;

		KillTimer (1000);
	}

	CDialog::OnTimer(nIDEvent);
}


void CDialogMacroStartStop::OnDestroy()
{
	CDialogCommon::OnDestroy();

}

void CDialogMacroStartStop::OnBnClickedCheckStartOption()
{
	if (IsDlgButtonChecked (IDC_CHECK_START_PUSH) == BST_CHECKED) {
		SetDlgItemText (IDC_CHECK_START_PUSH, "누를 때");
		_macro->start_key &= ~VK_KEY_UP;
	}
	else {
		SetDlgItemText (IDC_CHECK_START_PUSH, "뗄 때");
		_macro->start_key |= VK_KEY_UP;
	}
}

void CDialogMacroStartStop::OnBnClickedCheckStopOption()
{
	if (IsDlgButtonChecked (IDC_CHECK_STOP_PUSH) == BST_CHECKED) {
		SetDlgItemText (IDC_CHECK_STOP_PUSH, "누를 때");
		_macro->stop_key &= ~VK_KEY_UP;
	}
	else {
		SetDlgItemText (IDC_CHECK_STOP_PUSH, "뗄 때");
		_macro->stop_key |= VK_KEY_UP;
	}
}

void CDialogMacroStartStop::OnBnClickedButtonStartColorPick()
{
	CColorDialog dlg(_start_color);
	if (dlg.DoModal() == IDOK) {
		_start_color = dlg.GetColor();
		InvalidateRect (NULL, FALSE);
	
		SetDlgItemInt (IDC_EDIT_START_R, GetRValue (_start_color), FALSE);
		SetDlgItemInt (IDC_EDIT_START_G, GetGValue (_start_color), FALSE);
		SetDlgItemInt (IDC_EDIT_START_B, GetBValue (_start_color), FALSE);
	}
}

void CDialogMacroStartStop::OnBnClickedButtonStopColorPick()
{
	CColorDialog dlg(_stop_color);
	if (dlg.DoModal() == IDOK) {
		_stop_color = dlg.GetColor();
		InvalidateRect (NULL, FALSE);

		SetDlgItemInt (IDC_EDIT_STOP_R, GetRValue (_stop_color), FALSE);
		SetDlgItemInt (IDC_EDIT_STOP_G, GetGValue (_stop_color), FALSE);
		SetDlgItemInt (IDC_EDIT_STOP_B, GetBValue (_stop_color), FALSE);
	}
}

void CDialogMacroStartStop::OnBnClickedOk()
{
	WORD macro_options = 0; 

	if (IsDlgButtonChecked (IDC_CHECK_AUTO_START) == BST_CHECKED)	macro_options |= MACRO_AUTO_SS;
	if (IsDlgButtonChecked (IDC_CHECK_START_KEY)  == BST_CHECKED)	macro_options |= MACRO_KEY_START;
	if (IsDlgButtonChecked (IDC_CHECK_STOP_KEY)   == BST_CHECKED)	macro_options |= MACRO_KEY_STOP;
	if (IsDlgButtonChecked (IDC_CHECK_REC_TIME_DELAY)       == BST_CHECKED)	macro_options |= MACRO_TIMER_SS;
	if (IsDlgButtonChecked (IDC_CHECK_START_COLOR)== BST_CHECKED)	macro_options |= MACRO_COLOR_START;
	if (IsDlgButtonChecked (IDC_CHECK_STOP_COLOR) == BST_CHECKED)	macro_options |= MACRO_COLOR_STOP;

	int index = SendDlgItemMessage (IDC_COMBO_START_KEY, CB_GETCURSEL, 0, 0);
	if (index != -1)												_macro->start_key = GetVkCode(index);
	if (IsDlgButtonChecked (IDC_CHECK_START_PUSH)	 != BST_CHECKED)_macro->start_key |= VK_KEY_UP;
	if (IsDlgButtonChecked (IDC_CHECK_START_CTRL)	 == BST_CHECKED)_macro->start_key |= VK_KEY_CTRL;
	if (IsDlgButtonChecked (IDC_CHECK_START_ALT)	 == BST_CHECKED)_macro->start_key |= VK_KEY_ALT;
	if (IsDlgButtonChecked (IDC_CHECK_START_SHIFT)	 == BST_CHECKED)_macro->start_key |= VK_KEY_SHIFT;
	if (IsDlgButtonChecked (IDC_CHECK_START_MODIFIER)== BST_CHECKED)_macro->start_key |= VK_MODIFIER_KEY;

	index = SendDlgItemMessage (IDC_COMBO_STOP_KEY, CB_GETCURSEL, 0, 0);
	if (index != -1)												_macro->stop_key = GetVkCode(index);
	if (IsDlgButtonChecked (IDC_CHECK_STOP_PUSH)	!= BST_CHECKED)	_macro->stop_key |= VK_KEY_UP;
	if (IsDlgButtonChecked (IDC_CHECK_STOP_CTRL)	== BST_CHECKED)	_macro->stop_key |= VK_KEY_CTRL;
	if (IsDlgButtonChecked (IDC_CHECK_STOP_ALT)		== BST_CHECKED)	_macro->stop_key |= VK_KEY_ALT;
	if (IsDlgButtonChecked (IDC_CHECK_STOP_SHIFT)	== BST_CHECKED)	_macro->stop_key |= VK_KEY_SHIFT;
	if (IsDlgButtonChecked (IDC_CHECK_STOP_MODIFIER)== BST_CHECKED)	_macro->stop_key |= VK_MODIFIER_KEY;

	int hour = SendDlgItemMessage (IDC_COMBO_START_HOUR, CB_GETCURSEL) - 1;
	int min  = SendDlgItemMessage (IDC_COMBO_START_MIN,  CB_GETCURSEL) - 1;
	int sec  = SendDlgItemMessage (IDC_COMBO_START_SEC,  CB_GETCURSEL) - 1;

	// 시/분/초 로 표시되는 시각을 msec 단위의 시간으로 바꾼다.
	int time_start = 0;
	int flags_start = 0;
	if (sec >= 0) {	
		time_start += sec;	 
		flags_start = 1; 
	
		if (min >= 0) {	
			time_start += min*60;	 
			flags_start = 2; 
		
			if (hour >= 0) {	
				time_start += hour*60*60;	 
				flags_start = 3; 
			}
		}
	}
	_macro->start_time = time_start;
	macro_options |= flags_start<<8;

	hour = SendDlgItemMessage (IDC_COMBO_STOP_HOUR, CB_GETCURSEL) - 1;
	min  = SendDlgItemMessage (IDC_COMBO_STOP_MIN,  CB_GETCURSEL) - 1;
	sec  = SendDlgItemMessage (IDC_COMBO_STOP_SEC,  CB_GETCURSEL) - 1;

	// 시/분/초 로 표시되는 시각을 msec 단위의 시간으로 바꾼다.
	int time_end = 0;
	int flags_end = 0;
	if (sec >= 0) {	
		time_end += sec;	 
		flags_end = 1; 
	
		if (min >= 0) {	
			time_end += min*60;	 
			flags_end = 2; 
		
			if (hour >= 0) {	
				time_end += hour*60*60;	 
				flags_end = 3; 
			}
		}
	}
	_macro->stop_time = time_end;
	macro_options |= flags_end<<10;

	_macro->start_color = _start_color = RGB (
		BOUND(GetDlgItemInt (IDC_EDIT_START_R, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_START_G, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_START_B, 0, FALSE), 0, 255));
	_macro->stop_color = _stop_color = RGB (
		BOUND(GetDlgItemInt (IDC_EDIT_STOP_R, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_STOP_G, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_STOP_B, 0, FALSE), 0, 255));
	_macro->start_xy = MAKELONG (
		BOUND(GetDlgItemInt (IDC_EDIT_START_X, 0, FALSE), 0, 65535),
		BOUND(GetDlgItemInt (IDC_EDIT_START_Y, 0, FALSE), 0, 65535));
	_macro->stop_xy = MAKELONG (
		BOUND(GetDlgItemInt (IDC_EDIT_STOP_X,  0, FALSE), 0, 65535),
		BOUND(GetDlgItemInt (IDC_EDIT_STOP_Y,  0, FALSE), 0, 65535));

	_macro->tolerance = BOUND (GetDlgItemInt (IDC_EDIT_RGB_TOLERANCE, 0, FALSE), 0, 255);

	if (IsDlgButtonChecked (IDC_CHECK_ONCE) == BST_CHECKED)	macro_options |= MACRO_COLOR_ONCE;

	_macro->options = MAKELONG (macro_options, HIWORD(_macro->options));

	if (IsDlgButtonChecked (IDC_CHECK_REC_TIME_DELAY) == BST_CHECKED) {
		if (flags_start == 0 && flags_end == 0) {
			AfxMessageBox ("시간 사용이 선택되었지만 시작 시각과 중지 시각이 설정되지 않았습니다.");
			return;
		}
		else if (flags_start != flags_end) {
			AfxMessageBox ("시작 시각과 중지 시각의 시/분/초 스케일이 일치하지 않습니다.");
			return;
		}
	}

	OnOK();
}

void CDialogMacroStartStop::OnBnClickedCancel()
{
	OnCancel();
}

void CDialogMacroStartStop::OnBnClickedCheckStartKey()   { EnableWindowItem (TRUE); }
void CDialogMacroStartStop::OnBnClickedCheckStopKey()    { EnableWindowItem (TRUE); }
void CDialogMacroStartStop::OnBnClickedCheckTime()       { EnableWindowItem (TRUE); }
void CDialogMacroStartStop::OnBnClickedCheckStartColor() { EnableWindowItem (TRUE); }
void CDialogMacroStartStop::OnBnClickedCheckStopColor()  { EnableWindowItem (TRUE); }

void CDialogMacroStartStop::OnEnChangeEditStartR()
{
	uchar color	= BOUND(GetDlgItemInt (IDC_EDIT_START_R, 0, FALSE), 0, 255);
	_start_color = RGB(color, GetGValue (_start_color), GetBValue (_start_color));

	InvalidateRect (NULL, FALSE);
}

void CDialogMacroStartStop::OnEnChangeEditStartG()
{
	uchar color	= BOUND(GetDlgItemInt (IDC_EDIT_START_G, 0, FALSE), 0, 255);
	_start_color = RGB(GetRValue (_start_color), color, GetBValue (_start_color));

	InvalidateRect (NULL, FALSE);
}

void CDialogMacroStartStop::OnEnChangeEditStartB()
{
	uchar color	= BOUND(GetDlgItemInt (IDC_EDIT_START_B, 0, FALSE), 0, 255);
	_start_color = RGB(GetRValue (_start_color), GetGValue (_start_color), color);

	InvalidateRect (NULL, FALSE);
}

void CDialogMacroStartStop::OnEnChangeEditStopR()
{
	uchar color	= BOUND(GetDlgItemInt (IDC_EDIT_STOP_R, 0, FALSE), 0, 255);
	_stop_color = RGB(color, GetGValue (_stop_color), GetBValue (_stop_color));

	InvalidateRect (NULL, FALSE);
}

void CDialogMacroStartStop::OnEnChangeEditStopG()
{
	uchar color	= BOUND(GetDlgItemInt (IDC_EDIT_STOP_G, 0, FALSE), 0, 255);
	_stop_color = RGB(GetRValue (_stop_color), color, GetBValue (_stop_color));

	InvalidateRect (NULL, FALSE);
}

void CDialogMacroStartStop::OnEnChangeEditStopB()
{
	uchar color	= BOUND(GetDlgItemInt (IDC_EDIT_STOP_B, 0, FALSE), 0, 255);
	_stop_color = RGB(GetRValue (_stop_color), GetGValue (_stop_color), color);

	InvalidateRect (NULL, FALSE);
}

void CDialogMacroStartStop::EnableWindowItem(BOOL enable)
{
	BOOL enable2;

	enable = (IsDlgButtonChecked (IDC_CHECK_AUTO_START) == BST_CHECKED);

	enable = (IsDlgButtonChecked (IDC_CHECK_START_KEY) == BST_CHECKED);
	enable2 = enable && (IsDlgButtonChecked (IDC_CHECK_START_MODIFIER) == BST_CHECKED);
	GetDlgItem(IDC_COMBO_START_KEY)   ->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_START_PUSH)  ->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_START_CTRL)  ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_START_ALT)   ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_START_SHIFT) ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_START_MODIFIER)->EnableWindow (enable);
	
	enable = (IsDlgButtonChecked (IDC_CHECK_STOP_KEY) == BST_CHECKED);
	enable2 = enable && (IsDlgButtonChecked (IDC_CHECK_STOP_MODIFIER) == BST_CHECKED);
	GetDlgItem(IDC_COMBO_STOP_KEY)    ->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_STOP_PUSH)   ->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_STOP_CTRL)   ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_STOP_ALT)    ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_STOP_SHIFT)  ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_STOP_MODIFIER)->EnableWindow (enable);
	
	enable = (IsDlgButtonChecked (IDC_CHECK_REC_TIME_DELAY) == BST_CHECKED);
	GetDlgItem(IDC_COMBO_START_HOUR)  ->EnableWindow (enable);
	GetDlgItem(IDC_COMBO_START_MIN)   ->EnableWindow (enable);
	GetDlgItem(IDC_COMBO_START_SEC)   ->EnableWindow (enable);
	GetDlgItem(IDC_COMBO_STOP_HOUR)   ->EnableWindow (enable);
	GetDlgItem(IDC_COMBO_STOP_MIN)    ->EnableWindow (enable);
	GetDlgItem(IDC_COMBO_STOP_SEC)    ->EnableWindow (enable);

	enable = (IsDlgButtonChecked (IDC_CHECK_START_COLOR) == BST_CHECKED);
	GetDlgItem(IDC_EDIT_START_R)      ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_START_G)      ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_START_B)      ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_START_X)      ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_START_Y)      ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_START_COLOR_PICK)->EnableWindow (enable);

	enable = (IsDlgButtonChecked (IDC_CHECK_STOP_COLOR) == BST_CHECKED);
	GetDlgItem(IDC_EDIT_STOP_R)       ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_STOP_G)       ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_STOP_B)       ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_STOP_X)       ->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_STOP_Y)       ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_STOP_COLOR_PICK) ->EnableWindow (enable);

	enable = true;
	GetDlgItem(IDC_EDIT_RGB_TOLERANCE)->EnableWindow (enable);
}

void CDialogMacroStartStop::OnBnClickedCheckStartModifier()
{
	BOOL enable = (IsDlgButtonChecked (IDC_CHECK_START_KEY) == BST_CHECKED);
	BOOL enable2 = enable && (IsDlgButtonChecked (IDC_CHECK_START_MODIFIER) == BST_CHECKED);

	GetDlgItem(IDC_CHECK_START_CTRL)  ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_START_ALT)   ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_START_SHIFT) ->EnableWindow (enable2);
}

void CDialogMacroStartStop::OnBnClickedCheckStopModifier()
{
	BOOL enable = (IsDlgButtonChecked (IDC_CHECK_STOP_KEY) == BST_CHECKED);
	BOOL enable2 = enable && (IsDlgButtonChecked (IDC_CHECK_STOP_MODIFIER) == BST_CHECKED);

	GetDlgItem(IDC_CHECK_STOP_CTRL)   ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_STOP_ALT)    ->EnableWindow (enable2);
	GetDlgItem(IDC_CHECK_STOP_SHIFT)  ->EnableWindow (enable2);
}
