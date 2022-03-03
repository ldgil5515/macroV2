#include "stdafx.h"
#include "key_macro.h"
#include "macro_def.h"
#include "DialogMouse.h"
#include "key_hook.h"
#include "virtual_key.h"
#include "Common.h"
#include "Ini.h"


CDialogMouse *g_mouseDlg = NULL;

// 프로그램이 실행되는 동안 대화상자 아이템에 
// 설정된 값을 저장하고 있는 변수들을 static으로 선언
static long _flags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
static long _x = 0;
static long _y = 0;

IMPLEMENT_DYNAMIC(CDialogMouse, CDialog)

CDialogMouse::CDialogMouse(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMouse::IDD, pParent)
{
	g_mouseDlg = this;
}

CDialogMouse::~CDialogMouse()
{
	g_mouseDlg = NULL;
}

void CDialogMouse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogMouse, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogMouse::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_MOUSE_POS_ABSOLUTE, &CDialogMouse::OnBnClickedCheckMousePosAbsolute)
END_MESSAGE_MAP()


BOOL CDialogMouse::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (_item.type == MI_NONE) {
		SetWindowText ("마우스 이벤트 추가");
		// 기본값 설정
		_item.type        = MI_MOUSE;
		_item.mouse.flags = _flags;
		_item.mouse.x     = _x;
		_item.mouse.y     = _y;
	}
	else {
		SetWindowText ("마우스 이벤트 편집");
	}

	char message[256+1];
	_snprintf (message, 256, "%s를 눌러 커서 위치를 획득합니다.", GetVkDescFromVK (g_ini.keyMousePosCapture));
	message[256] = '\0';
	SetDlgItemText (IDC_STATIC_MESSAGE, message);

	CheckDlgButton (IDC_CHECK_MOVE,  (_item.mouse.flags & MOUSEEVENTF_MOVE) ? BST_CHECKED : BST_UNCHECKED); 
	SetDlgItemInt (IDC_EDIT_ABSPOS_X, _item.mouse.x);
	SetDlgItemInt (IDC_EDIT_ABSPOS_Y, _item.mouse.y);

	CheckDlgButton (IDC_CHECK_WHEEL, (_item.mouse.flags & MOUSEEVENTF_WHEEL) ? BST_CHECKED : BST_UNCHECKED); 
	SetDlgItemInt  (IDC_EDIT_WHEEL,  (_item.mouse.flags & MOUSEEVENTF_WHEEL) ? (short)HIWORD(_item.mouse.flags) : 0);
	
	int button_select = 0;
	int button_state  = 3;

	if (_item.mouse.flags & (MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP)) {
		button_select = 1;
		button_state  = (_item.mouse.flags >> 1) & 0x03;
	}
	else if (_item.mouse.flags & (MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP)) { 
		button_select = 2;
		button_state  = (_item.mouse.flags >> 3) & 0x03;
	}
	else if (_item.mouse.flags & (MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP)) { 
		button_select = 3;
		button_state  = (_item.mouse.flags >> 5) & 0x03;
	}
	else if (_item.mouse.flags & (MOUSEEVENTF_XDOWN|MOUSEEVENTF_XUP)) {
		if      (HIWORD(_item.mouse.flags) == XBUTTON1) button_select = 4;
		else if (HIWORD(_item.mouse.flags) == XBUTTON2) button_select = 5;
		button_state = (_item.mouse.flags >> 7) & 0x03;
	}

	SendDlgItemMessage (IDC_COMBO_BUTTON_SELECT, CB_SETCURSEL, button_select);

	CheckDlgButton (IDC_RADIO_BUTTON_CLICK, (button_state == 3) ? BST_CHECKED : BST_UNCHECKED);	// 마우스 버튼 클릭
	CheckDlgButton (IDC_RADIO_BUTTON_DOWN,  (button_state == 1) ? BST_CHECKED : BST_UNCHECKED);	// 마우스 버튼 누르기
	CheckDlgButton (IDC_RADIO_BUTTON_UP,    (button_state == 2) ? BST_CHECKED : BST_UNCHECKED); // 마우스 버튼 떼기

	CheckDlgButton (IDC_CHECK_MOUSE_POS_ABSOLUTE, _item.mouse.flags&MOUSEEVENTF_ABSOLUTE               ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_MOUSE_POS_SCRINDEP, _item.mouse.flags&MOUSEEVENTF_SCREEN_INDEPENDENT_POS ? BST_CHECKED : BST_UNCHECKED);

	OnBnClickedCheckMousePosAbsolute();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogMouse::OnMouseEvent (long mx, long my)
{
	if (IsDlgButtonChecked (IDC_CHECK_MOUSE_POS_SCRINDEP)) {
		// TRACE ("%d %d -- ", mx, my);
		ScreenIndependentMousePos (mx, my);
		// TRACE ("%d %d\n", mx, my);
	}

	SetDlgItemInt (IDC_EDIT_CURPOS_X, mx);
	SetDlgItemInt (IDC_EDIT_CURPOS_Y, my);
}

void CDialogMouse::OnkeyMouseEvent ()
{
	int mx = GetDlgItemInt (IDC_EDIT_CURPOS_X);
	int my = GetDlgItemInt (IDC_EDIT_CURPOS_Y);

	if (IsDlgButtonChecked(IDC_CHECK_MOUSE_POS_ABSOLUTE)) {
		SetDlgItemInt (IDC_EDIT_ABSPOS_X, mx);
		SetDlgItemInt (IDC_EDIT_ABSPOS_Y, my);
	}
	else {
		static int sx = 0;
		static int sy = 0;

		SetDlgItemInt (IDC_EDIT_ABSPOS_X, mx-sx);
		SetDlgItemInt (IDC_EDIT_ABSPOS_Y, my-sy);
		sx = mx; sy = my;
	}
}

void CDialogMouse::OnBnClickedOk()
{
	_flags = 0;

	if (IsDlgButtonChecked (IDC_CHECK_MOVE))  _flags |= MOUSEEVENTF_MOVE;
	if (IsDlgButtonChecked (IDC_CHECK_WHEEL)) _flags |= MOUSEEVENTF_WHEEL;

	_x = GetDlgItemInt (IDC_EDIT_ABSPOS_X);
	_y = GetDlgItemInt (IDC_EDIT_ABSPOS_Y);

	int button_select = SendDlgItemMessage (IDC_COMBO_BUTTON_SELECT, CB_GETCURSEL, 0, 0);

	int button_state = 0;
	if (IsDlgButtonChecked (IDC_RADIO_BUTTON_CLICK))   button_state = 3;	// 마우스 버튼 클릭
	if (IsDlgButtonChecked (IDC_RADIO_BUTTON_DOWN))    button_state = 1;	// 마우스 버튼 누르기
	if (IsDlgButtonChecked (IDC_RADIO_BUTTON_UP))      button_state = 2;	// 마우스 버튼 떼기

	if (button_select > 0) {
		if (button_select == 1 && button_state == 1) _flags |= MOUSEEVENTF_LEFTDOWN;
		if (button_select == 1 && button_state == 2) _flags |= MOUSEEVENTF_LEFTUP;
		if (button_select == 1 && button_state == 3) _flags |= MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP;

		if (button_select == 2 && button_state == 1) _flags |= MOUSEEVENTF_RIGHTDOWN;
		if (button_select == 2 && button_state == 2) _flags |= MOUSEEVENTF_RIGHTUP;
		if (button_select == 2 && button_state == 3) _flags |= MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP;

		if (button_select == 3 && button_state == 1) _flags |= MOUSEEVENTF_MIDDLEDOWN;
		if (button_select == 3 && button_state == 2) _flags |= MOUSEEVENTF_MIDDLEUP;
		if (button_select == 3 && button_state == 3) _flags |= MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP;
	}

	// 마우스의 휠 이벤트와 X1, X2 버튼은 서로 변수를 공유하기 때문에 같이 사용할 수 없다.
	if (_flags & MOUSEEVENTF_WHEEL) {
		_flags |= MAKELONG(0, GetDlgItemInt (IDC_EDIT_WHEEL));
	}
	else if (button_select > 3) {
		if (button_select == 4 && button_state == 1) _flags |= MAKELONG(MOUSEEVENTF_XDOWN                , XBUTTON1);
		if (button_select == 4 && button_state == 2) _flags |= MAKELONG(MOUSEEVENTF_XUP                  , XBUTTON1);
		if (button_select == 4 && button_state == 3) _flags |= MAKELONG(MOUSEEVENTF_XDOWN|MOUSEEVENTF_XUP, XBUTTON1);

		if (button_select == 5 && button_state == 1) _flags |= MAKELONG(MOUSEEVENTF_XDOWN                , XBUTTON2);
		if (button_select == 5 && button_state == 2) _flags |= MAKELONG(MOUSEEVENTF_XUP                  , XBUTTON2);
		if (button_select == 5 && button_state == 3) _flags |= MAKELONG(MOUSEEVENTF_XDOWN|MOUSEEVENTF_XUP, XBUTTON2);
	}

	if (IsDlgButtonChecked (IDC_CHECK_MOUSE_POS_ABSOLUTE)) _flags |= MOUSEEVENTF_ABSOLUTE;
	if (IsDlgButtonChecked (IDC_CHECK_MOUSE_POS_SCRINDEP)) _flags |= MOUSEEVENTF_SCREEN_INDEPENDENT_POS;

	_item.mouse.flags = _flags;
	_item.mouse.x     = _x;
	_item.mouse.y     = _y;

	// 설정된 플래그를 검사할 때는 다음 두 플래그를 빼고 검사
	long check_flags = _flags & ~(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_SCREEN_INDEPENDENT_POS);
	if (!check_flags) {
		AfxMessageBox ("마우스의 아무런 기능도 선택되지 않았습니다.");
		return;
	}
	OnOK();
}

void CDialogMouse::OnBnClickedCheckMousePosAbsolute()
{
	if (IsDlgButtonChecked(IDC_CHECK_MOUSE_POS_ABSOLUTE)) {
		SetDlgItemText (IDC_CHECK_MOVE, "절대위치:");
		GetDlgItem(IDC_CHECK_MOUSE_POS_SCRINDEP)->EnableWindow (TRUE);
	}
	else {
		SetDlgItemText (IDC_CHECK_MOVE, "상대위치:");
		CheckDlgButton (IDC_CHECK_MOUSE_POS_SCRINDEP, BST_UNCHECKED);
		GetDlgItem(IDC_CHECK_MOUSE_POS_SCRINDEP)->EnableWindow (FALSE);
	}
}
