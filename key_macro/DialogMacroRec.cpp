#include "stdafx.h"
#include <math.h>
#include "key_macro.h"
#include "DialogMacroRec.h"
#include "DialogMacroStartStop.h"
#include "virtual_key.h"
#include "key_hook.h"
#include "AppIni.h"
#include "MmTimer.h"
#include "Common.h"


extern const char *GetItemDescription (const sMacroItem &item);

CDialogMacroRec *g_recDlg = NULL;


IMPLEMENT_DYNAMIC(CDialogMacroRec, CDialog)

CDialogMacroRec::CDialogMacroRec(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMacroRec::IDD, pParent)
{
	g_recDlg = this;
}

CDialogMacroRec::~CDialogMacroRec()
{
	g_recDlg = NULL;
}

void CDialogMacroRec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, _listItems);
}


BEGIN_MESSAGE_MAP(CDialogMacroRec, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDialogMacroRec::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogMacroRec::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_REC_STAT, &CDialogMacroRec::OnBnClickedCheckRecStat)
	ON_BN_CLICKED(IDC_CHECK_REC_KEYBOARD, &CDialogMacroRec::OnBnClickedCheckRecKey)
	ON_BN_CLICKED(IDC_CHECK_REC_MOUSE_BUTTON, &CDialogMacroRec::OnBnClickedCheckRecMouse)
	ON_BN_CLICKED(IDC_CHECK_REC_MOUSE_MOVE, &CDialogMacroRec::OnBnClickedCheckRecMousePos)
	ON_BN_CLICKED(IDC_CHECK_REC_MOUSE_WHEEL, &CDialogMacroRec::OnBnClickedCheckRecMouseWheel)
	ON_BN_CLICKED(IDC_CHECK_REC_TIME_DELAY, &CDialogMacroRec::OnBnClickedCheckTime)
	ON_BN_CLICKED(IDC_CHECK_REC_MERGE_UP_DOWN, &CDialogMacroRec::OnBnClickedCheckMerge)
	ON_BN_CLICKED(IDC_CHECK_MOUSE_POS_SCRINDEP, &CDialogMacroRec::OnBnClickedCheckMousePosScrindep)
	ON_BN_CLICKED(IDC_CHECK_MOUSE_POS_ABSOLUTE, &CDialogMacroRec::OnBnClickedCheckMousePosAbsolute)
	ON_BN_CLICKED(IDC_CHECK_REC_MOUSE_BUTTON_POS, &CDialogMacroRec::OnBnClickedCheckRecButtonMove)
	ON_BN_CLICKED(IDC_BUTTON_MACRO_START_STOP, &CDialogMacroRec::OnBnClickedButtonMacroStartStop)
END_MESSAGE_MAP()

BOOL CDialogMacroRec::OnInitDialog()
{
	CDialog::OnInitDialog();

	char message[256+1];
	_snprintf (message, 256, "%s로 매크로 기록을 시작하거나 중지합니다.", GetVkDescFromVK (g_ini.keyMacroRecord));
	message[256] = '\0';
	SetDlgItemText (IDC_STATIC_MESSAGE, message);

	if (!_macro->name[0]) {
		int name_id = 0;
		do {
			_snprintf (_macro->name, 255, "Macro Rec. %d", ++name_id);
			_macro->name[255] = '\0';
		} while (GetMacroID(_macro->name) >= 0);

		_macro->start_key = VK_F2;
		_macro->stop_key  = VK_F3;
		_macro->options   = MAKELONG(MACRO_KEY_START | MACRO_KEY_STOP, 1);

		// 제일 첫줄에 아무것도 아닌 항목을 삽입해 둔다.
		// 이유는, 선택한 항목 뒤에다가 새로운 항목을 추가할 수 있기때문에,
		// 이 MI_NONE이 제일 처음에 없으면 첫줄에 새로운 항목을 추가할 수 없다.
		_macro->_item.resize(1);
		_macro->_item[0].type = MI_NONE;
	}

	for (uint i=0; i<_macro->_item.size(); ++i) {
	   _listItems.InsertString (i, GetItemDescription (_macro->_item[i]));
	}
	_listItems.SetCurSel(0);

	SetTextRecCount();

	CheckDlgButton (IDC_CHECK_REC_KEYBOARD,			(g_ini.recOptions.REC_KEYBOARD			) ? BST_CHECKED : BST_UNCHECKED);	// Keyboard 입력 기록
	CheckDlgButton (IDC_CHECK_REC_MOUSE_BUTTON,		(g_ini.recOptions.REC_MOUSE_BUTTON		) ? BST_CHECKED : BST_UNCHECKED);	// Mouse Button 입력 기록
	CheckDlgButton (IDC_CHECK_REC_MOUSE_BUTTON_POS,	(g_ini.recOptions.REC_MOUSE_BUTTON_POS	) ? BST_CHECKED : BST_UNCHECKED);	// Mouse Button 입력 기록시 커서 위치도 같이 기록
	CheckDlgButton (IDC_CHECK_REC_MOUSE_MOVE,		(g_ini.recOptions.REC_MOUSE_MOVE		) ? BST_CHECKED : BST_UNCHECKED);	// Mouse 위치 변화 기록
	CheckDlgButton (IDC_CHECK_REC_MOUSE_WHEEL,		(g_ini.recOptions.REC_MOUSE_WHEEL		) ? BST_CHECKED : BST_UNCHECKED);	// Mouse 휠 변화 기록
	CheckDlgButton (IDC_CHECK_REC_TIME_DELAY,		(g_ini.recOptions.REC_TIME_DELAY		) ? BST_CHECKED : BST_UNCHECKED);	// Keyboard나 Mouse 이벤트간 시간 간격 기록
	CheckDlgButton (IDC_CHECK_REC_MERGE_UP_DOWN,	(g_ini.recOptions.REC_MERGE_UP_DOWN		) ? BST_CHECKED : BST_UNCHECKED);	// Button Up/Down 이벤트 합치기 허용
	CheckDlgButton (IDC_CHECK_MOUSE_POS_SCRINDEP,	(g_ini.recOptions.SCREEN_INDEP_MOUSE	) ? BST_CHECKED : BST_UNCHECKED);	// Mouse 위치 변화를 화면 크기에 독립적으로 기록(0 ~ 65535)
	CheckDlgButton (IDC_CHECK_MOUSE_POS_ABSOLUTE,   (g_ini.recOptions.MOUSE_POS_ABSOLUTE	) ? BST_CHECKED : BST_UNCHECKED);	// Check일때는 Mouse의 절대 위치를 기록, Uncheck일때는 상대적 위치를 기록
	
	SetDlgItemInt (IDC_EDIT_MOVE, g_ini.recMouseDistance);
	SetDlgItemInt (IDC_EDIT_TIME, g_ini.recEventTimeInterval);

	SetDlgItemInt (IDC_EDIT_MACRO_REPEAT_COUNT2, (int)(ushort)HIWORD(_macro->options));

	_macro_rec = false;
	_rec_time = 0;

	OnBnClickedCheckRecStat();
	OnBnClickedCheckMousePosAbsolute();

	SetTimer (1000, 33, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogMacroRec::SetTextRecCount()
{
	int depth = 0;
	long exec_time = _macro->ExecTime (depth);
	int n_item = _macro->_item.size() - 1;

	char text[256+1];
	if (exec_time >= 100000000) {
		_snprintf (text, 256, "%d개 항목이 기록됨 (순환호출!)", n_item);
	}
	else {
		_snprintf (text, 256, "%d개 항목이 기록됨 (%g 초)", n_item, exec_time/1000.);
	}
	text[256] = '\0';

	SetDlgItemText (IDC_STATIC_REC_COUNT, text);
}

void CDialogMacroRec::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1000) {
		int n_item = _macro->_item.size();

		if (_listItems.GetCount() < n_item) {
			for (int i=_listItems.GetCount(); i<n_item; ++i) {
			   _listItems.InsertString (i, GetItemDescription (_macro->_item[i]));
			}
			_listItems.SetCurSel (n_item-1);
			_listItems.ShowCaret ();

			SetTextRecCount();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDialogMacroRec::OnKeyboardEvent (BYTE vk_code, bool vk_pressed, BYTE scan_code)
{
	if (vk_code == g_ini.keyMacroRecord && vk_pressed == true) {
		MacroRecStateChange (!_macro_rec);
	}

	if (_macro_rec) {
		if (vk_code) {
			if (g_ini.recOptions.REC_KEYBOARD) {	// 0x01 - Keyboard 입력 기록
				char hangul = IsImeHangul() ? 0x10 : 0x00;

				if (vk_pressed) {
					AddKey (vk_code, scan_code, hangul|0x01);	// Key Down
				}
				else {
					AddKey (vk_code, scan_code, hangul|0x02);	// Key Up
				}
			}
		}
	}
}

long CDialogMacroRec::GetMouseButtonFlags (BYTE vk_code, bool vk_pressed)
{
	if      (vk_code == VK_LBUTTON  &&  vk_pressed) return MOUSEEVENTF_LEFTDOWN;
	else if (vk_code == VK_LBUTTON  && !vk_pressed) return MOUSEEVENTF_LEFTUP;
	else if (vk_code == VK_RBUTTON  &&  vk_pressed) return MOUSEEVENTF_RIGHTDOWN;
	else if (vk_code == VK_RBUTTON  && !vk_pressed) return MOUSEEVENTF_RIGHTUP;
	else if (vk_code == VK_MBUTTON  &&  vk_pressed) return MOUSEEVENTF_MIDDLEDOWN;
	else if (vk_code == VK_MBUTTON  && !vk_pressed) return MOUSEEVENTF_MIDDLEUP;
	else if (vk_code == VK_XBUTTON1 &&  vk_pressed) return MAKELONG(MOUSEEVENTF_XDOWN, XBUTTON1);
	else if (vk_code == VK_XBUTTON1 && !vk_pressed) return MAKELONG(MOUSEEVENTF_XUP,   XBUTTON1);
	else if (vk_code == VK_XBUTTON2 &&  vk_pressed) return MAKELONG(MOUSEEVENTF_XDOWN, XBUTTON2);
	else if (vk_code == VK_XBUTTON2 && !vk_pressed) return MAKELONG(MOUSEEVENTF_XUP,   XBUTTON2);
	else    return 0;
}

void CDialogMacroRec::OnMouseEvent (BYTE vk_code, bool vk_pressed, bool move, short wheelDelat, POINT &pt)
{
	if (vk_code == g_ini.keyMacroRecord && vk_pressed == true) {
		MacroRecStateChange (!_macro_rec);
	}

	if (_macro_rec) {
		static int sx = 0;
		static int sy = 0;

		if (g_ini.recOptions.REC_MOUSE_BUTTON) {	// 0x02 - Mouse Button 입력 기록
			long button = GetMouseButtonFlags (vk_code, vk_pressed);

			if (button) {
				int flags = 0;

				if (g_ini.recOptions.REC_MOUSE_BUTTON_POS) { // 0x100 - Mouse Button 입력 기록시 커서 위치도 같이 기록
					flags |= MOUSEEVENTF_MOVE | button;

					if (g_ini.recOptions.MOUSE_POS_ABSOLUTE) {	// 0x80 - Check일때는 Mouse의 절대 위치를 기록, Uncheck일때는 상대적 위치를 기록
						flags |= MOUSEEVENTF_ABSOLUTE;

						if (g_ini.recOptions.SCREEN_INDEP_MOUSE) {	// 0x40 - Mouse 위치 변화를 화면 크기에 독립적으로 기록(0 ~ 65535)
							ScreenIndependentMousePos (pt.x, pt.y);
							flags |= MOUSEEVENTF_SCREEN_INDEPENDENT_POS;
						}
				
						AddMouse (pt.x, pt.y, flags);
						sx = pt.x; 
						sy = pt.y;
					}
					else {
						AddMouse (pt.x - sx, pt.y - sy, flags);
						sx = pt.x; 
						sy = pt.y;
					}
				}
				else {
					flags |= button;
					AddMouse (pt.x, pt.y, flags);
				}
			}
		}
		if (g_ini.recOptions.REC_MOUSE_MOVE) {	// 0x04 - Mouse 위치 변화 기록
			if (move) { 
				int flags = MOUSEEVENTF_MOVE;

				if (g_ini.recOptions.MOUSE_POS_ABSOLUTE) {	// 0x80 - Check일때는 Mouse의 절대 위치를 기록, Uncheck일때는 상대적 위치를 기록
					flags |= MOUSEEVENTF_ABSOLUTE;

					if (g_ini.recOptions.SCREEN_INDEP_MOUSE) {	// 0x40 - Mouse 위치 변화를 화면 크기에 독립적으로 기록(0 ~ 65535)
						ScreenIndependentMousePos (pt.x, pt.y);
						flags |= MOUSEEVENTF_SCREEN_INDEPENDENT_POS;
					}
					if (MouseDistance(pt.x, pt.y) >= g_ini.recMouseDistance) {
						AddMouse (pt.x, pt.y, flags);
						sx = pt.x; 
						sy = pt.y;
					}
				}
				else {
					if (MouseDistance(pt.x, pt.y, sx, sy) >= g_ini.recMouseDistance) {
						AddMouse (pt.x - sx, pt.y - sy, flags);
						sx = pt.x; 
						sy = pt.y;
					}
				}
			}
		}
		if (g_ini.recOptions.REC_MOUSE_WHEEL) {	// 0x08 - Mouse 휠 변화 기록
			if (vk_code == 0x0A || vk_code == 0x0B) {	// Wheel forward or reverse
				AddMouse (pt.x, pt.y, MAKELONG(MOUSEEVENTF_WHEEL, wheelDelat));
			}
		}
	}
}

int CDialogMacroRec::MouseDistance (int px, int py)
{
	if (_macro->_item.size() > 0) {
		sMacroItem &_item = _macro->_item[_macro->_item.size()-1];
		if ((_item.type == MI_MOUSE) && 
			(_item.mouse.flags&MOUSEEVENTF_MOVE)) {
			long mx = _item.mouse.x;
			long my = _item.mouse.y;

			if (_item.mouse.flags&MOUSEEVENTF_SCREEN_INDEPENDENT_POS){
				ScreenDependentMousePos (mx, my);
			}

			int dx = mx - px;
			int dy = my - py;

			return (int)sqrt((double)dx*dx + dy*dy);
		}
	}
	return 1000000;
}

int CDialogMacroRec::MouseDistance (int px, int py, int mx, int my)
{
	int dx = mx - px;
	int dy = my - py;

	return (int)sqrt((double)dx*dx + dy*dy);
}

void CDialogMacroRec::AddTimeDelayIf ()
{
	if (g_ini.recOptions.REC_TIME_DELAY) {	// 0x10 - Keyboard나 Mouse 이벤트간 시간 간격 기록
		DWORD cur_time = CMmTimer::GetTime ();
		
		if ((int)(cur_time - _rec_time) >= g_ini.recEventTimeInterval) {
			AddTimeDelay (cur_time - _rec_time);
		}
		_rec_time = cur_time;
	}
}

bool CDialogMacroRec::LastIsSameKeyDown (long scan_vk_code)
{
	if (_macro->_item.size() > 0) {
		sMacroItem &_item = _macro->_item[_macro->_item.size() - 1];

		if (_item.type == MI_KEYBD &&
			_item.keybd.scan_vk_code == scan_vk_code &&
			(_item.keybd.flags&0x03) == 0x01) return true;
	}
	return false;
}

void CDialogMacroRec::AddKey (BYTE vk_code, BYTE scan_code, long flags)
{
	AddTimeDelayIf ();

	long scan_vk_code = MAKEWORD (vk_code, scan_code);

	if ((g_ini.recOptions.REC_MERGE_UP_DOWN) && 
		(flags & 0x02) && 
		LastIsSameKeyDown(scan_vk_code)) {	// 0x20 - Button Up/Down 이벤트 합치기 허용
		// 이전 키가 눌린 상태라서 누르고 떼기 상태로 바꾼다.
		int last_index = _macro->_item.size() - 1; // 제일 마지막 이벤트
		_macro->_item[last_index].keybd.flags |= 0x02;

		_listItems.DeleteString (last_index);
	}
	else {
		sMacroItem _item;

		_item.type		         = MI_KEYBD;
		_item.keybd.scan_vk_code = scan_vk_code;
		_item.keybd.flags        = flags;

		_macro->_item.push_back (_item);
	}
}

bool CDialogMacroRec::LastIsSameMouseDown (int x, int y, long flags)
{
	if (_macro->_item.size() > 0) {
		sMacroItem &_item = _macro->_item[_macro->_item.size() - 1];

		if (_item.type == MI_MOUSE) {
			if ((_item.mouse.flags&MOUSEEVENTF_LEFTDOWN   && flags&MOUSEEVENTF_LEFTUP)   ||
				(_item.mouse.flags&MOUSEEVENTF_RIGHTDOWN  && flags&MOUSEEVENTF_RIGHTUP)  ||
				(_item.mouse.flags&MOUSEEVENTF_MIDDLEDOWN && flags&MOUSEEVENTF_MIDDLEUP) ||
				(_item.mouse.flags&MOUSEEVENTF_XDOWN      && flags&MOUSEEVENTF_XUP)      ){

				if (_item.mouse.x == x && _item.mouse.y == y) return true;
			}
		}
	}
	return false;
}

void CDialogMacroRec::AddMouse (int mx, int my, long flags)
{
	AddTimeDelayIf ();

	if ((g_ini.recOptions.REC_MERGE_UP_DOWN) && 
		(flags & (MOUSEEVENTF_LEFTUP|MOUSEEVENTF_RIGHTUP|MOUSEEVENTF_MIDDLEUP|MOUSEEVENTF_XUP)) && 
		LastIsSameMouseDown(mx, my, flags)) {	// 0x20 - Button Up/Down 이벤트 합치기 허용
		// 이전 마우스 버튼이 눌린 상태라서 클릭 상태로 바꾼다.
		_macro->_item[_macro->_item.size()-1].mouse.flags |= flags;

		_listItems.DeleteString (_macro->_item.size()-1);
	}
	else {
		sMacroItem _item;

		_item.type        = MI_MOUSE;
		_item.mouse.flags = flags;
		_item.mouse.x     = mx;
		_item.mouse.y     = my;

		_macro->_item.push_back (_item);
	}
}

void CDialogMacroRec::AddTimeDelay (long delay)
{
	sMacroItem _item;

	_item.type       = MI_DELAY;
	_item.delay.delay = delay;
	_item.delay.flags = 0;

	_macro->_item.push_back (_item);
}

void CDialogMacroRec::EnableWindowItem(BOOL enable)
{
	GetDlgItem(IDC_CHECK_REC_KEYBOARD)			->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_REC_MOUSE_BUTTON)		->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_REC_MOUSE_MOVE)		->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_REC_MOUSE_WHEEL)		->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_REC_TIME_DELAY)		->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_REC_MERGE_UP_DOWN)		->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_MOVE)					->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_TIME)					->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_MOUSE_POS_ABSOLUTE)	->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_MOUSE_POS_SCRINDEP)	->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_REC_MOUSE_BUTTON_POS)	->EnableWindow (enable);
	GetDlgItem(IDC_EDIT_MACRO_REPEAT_COUNT2)	->EnableWindow (enable);
	
	if (enable) OnBnClickedCheckMousePosAbsolute();
}

void CDialogMacroRec::MacroRecStateChange(bool rec)
{
	g_ini.recMouseDistance = GetDlgItemInt (IDC_EDIT_MOVE);
	g_ini.recEventTimeInterval = GetDlgItemInt (IDC_EDIT_TIME);

	if (rec) {
		CheckDlgButton (IDC_CHECK_REC_STAT, BST_CHECKED);
		SetDlgItemText (IDC_CHECK_REC_STAT, "매크로 기록 중");

		EnableWindowItem (FALSE);

		_macro_rec = true;
		_rec_time = CMmTimer::GetTime();
	}
	else {
		_macro_rec = false;

		CheckDlgButton (IDC_CHECK_REC_STAT, BST_UNCHECKED);
		SetDlgItemText (IDC_CHECK_REC_STAT, "기록 대기 중");

		EnableWindowItem (TRUE);
	}
}

void CDialogMacroRec::MacroRecOptionChanged()
{
	g_ini.recOptions.REC_KEYBOARD			= (IsDlgButtonChecked (IDC_CHECK_REC_KEYBOARD)			== BST_CHECKED);	// Keyboard 입력 기록
	g_ini.recOptions.REC_MOUSE_BUTTON		= (IsDlgButtonChecked (IDC_CHECK_REC_MOUSE_BUTTON)		== BST_CHECKED);	// Mouse Button 입력 기록
	g_ini.recOptions.REC_MOUSE_MOVE			= (IsDlgButtonChecked (IDC_CHECK_REC_MOUSE_MOVE)		== BST_CHECKED);	// Mouse 위치 변화 기록
	g_ini.recOptions.REC_MOUSE_WHEEL		= (IsDlgButtonChecked (IDC_CHECK_REC_MOUSE_WHEEL)		== BST_CHECKED);	// Mouse 휠 변화 기록
	g_ini.recOptions.REC_TIME_DELAY			= (IsDlgButtonChecked (IDC_CHECK_REC_TIME_DELAY)		== BST_CHECKED);	// Keyboard나 Mouse 이벤트간 시간 간격 기록
	g_ini.recOptions.REC_MERGE_UP_DOWN		= (IsDlgButtonChecked (IDC_CHECK_REC_MERGE_UP_DOWN)		== BST_CHECKED);	// Button Up/Down 이벤트 합치기 허용
	g_ini.recOptions.SCREEN_INDEP_MOUSE		= (IsDlgButtonChecked (IDC_CHECK_MOUSE_POS_SCRINDEP)	== BST_CHECKED);	// Mouse 위치 변화를 화면 크기에 독립적으로 기록(0 ~ 65535)
	g_ini.recOptions.MOUSE_POS_ABSOLUTE		= (IsDlgButtonChecked (IDC_CHECK_MOUSE_POS_ABSOLUTE)	== BST_CHECKED);	// Check일때는 Mouse의 절대 위치를 기록, Uncheck일때는 상대적 위치를 기록
	g_ini.recOptions.REC_MOUSE_BUTTON_POS	= (IsDlgButtonChecked (IDC_CHECK_REC_MOUSE_BUTTON_POS)	== BST_CHECKED);	// Mouse Button 입력 기록시 커서 위치도 같이 기록
}

void CDialogMacroRec::OnBnClickedButtonMacroStartStop()
{
	CDialogMacroStartStop dlg;

	dlg._macro = _macro;
	dlg.DoModal ();
}

void CDialogMacroRec::OnBnClickedCheckRecStat()
{
	MacroRecStateChange (IsDlgButtonChecked (IDC_CHECK_REC_STAT) == BST_CHECKED);

	GetDlgItem (IDC_LIST_ITEMS)->SetFocus ();
}

void CDialogMacroRec::OnBnClickedCheckRecKey()			{ MacroRecOptionChanged(); }
void CDialogMacroRec::OnBnClickedCheckRecMouse()		{ MacroRecOptionChanged(); }
void CDialogMacroRec::OnBnClickedCheckRecMousePos()		{ MacroRecOptionChanged(); }
void CDialogMacroRec::OnBnClickedCheckRecMouseWheel()	{ MacroRecOptionChanged(); }
void CDialogMacroRec::OnBnClickedCheckTime()			{ MacroRecOptionChanged(); }
void CDialogMacroRec::OnBnClickedCheckMerge()			{ MacroRecOptionChanged(); }
void CDialogMacroRec::OnBnClickedCheckMousePosScrindep(){ MacroRecOptionChanged(); }
void CDialogMacroRec::OnBnClickedCheckRecButtonMove()	{ MacroRecOptionChanged(); }

void CDialogMacroRec::OnBnClickedCheckMousePosAbsolute()
{
	if (IsDlgButtonChecked(IDC_CHECK_MOUSE_POS_ABSOLUTE)) {
		GetDlgItem(IDC_CHECK_MOUSE_POS_SCRINDEP)->EnableWindow (TRUE);
	}
	else {
		CheckDlgButton (IDC_CHECK_MOUSE_POS_SCRINDEP, BST_UNCHECKED);
		GetDlgItem(IDC_CHECK_MOUSE_POS_SCRINDEP)->EnableWindow (FALSE);
	}
	MacroRecOptionChanged();
}

void CDialogMacroRec::OnBnClickedOk()
{
	MacroRecStateChange (false);

	int repeat_count = GetDlgItemInt (IDC_EDIT_MACRO_REPEAT_COUNT2);
	repeat_count = BOUND(repeat_count, 0, 65535);

	// _macro->options = MAKELONG(0, repeat_count);
	_macro->options = MAKELONG(LOWORD(_macro->options), repeat_count);

	OnOK();
}

void CDialogMacroRec::OnBnClickedCancel()
{
	MacroRecStateChange (false);

	OnCancel();
}

