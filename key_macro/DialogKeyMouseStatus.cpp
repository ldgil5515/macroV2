#include "stdafx.h"
#include "key_macro.h"
#include "DialogKeyMouseStatus.h"
#include "macro_item.h"
#include "GdiObjects.h"
#include "Common.h"
#include "AppIni.h"
#include "virtual_key.h"


IMPLEMENT_DYNAMIC(CDialogKeyMouseStatus, CDialog)

CDialogKeyMouseStatus::CDialogKeyMouseStatus(CWnd* pParent /*=NULL*/)
	: CDialogCommon(CDialogKeyMouseStatus::IDD, pParent)
{
}

CDialogKeyMouseStatus::~CDialogKeyMouseStatus()
{
}

void CDialogKeyMouseStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogKeyMouseStatus, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogKeyMouseStatus::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogKeyMouseStatus::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CDialogKeyMouseStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// 윈도우의 위치를 복원한다.
	DlgInScreen (g_ini.statusSX, g_ini.statusSY);
	SetWindowPos (NULL, g_ini.statusSX, g_ini.statusSY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	

	SetTimer (1000, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogKeyMouseStatus::OnDestroy()
{
	CDialog::OnDestroy();

	RECT rectWnd;
	GetWindowRect (&rectWnd);
	g_ini.statusSX = rectWnd.left;
	g_ini.statusSY = rectWnd.top;
}

void CDialogKeyMouseStatus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (IsWindowVisible ()) {
		UpdateControlKeyStatus();
	}

	CDialog::OnTimer(nIDEvent);
}

void CDialogKeyMouseStatus::UpdateControlKeyStatus()
{
	static int key_state_prev = 0;
	int key_state = GetCtrlAltShiftKeyState ();

	if (key_state_prev != key_state) {
		ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_CTRL , (key_state & VK_KEY_CTRL) ? HB_LAMP_ON : HB_LAMP_OFF);
		ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_ALT  , (key_state & VK_KEY_ALT) ? HB_LAMP_ON : HB_LAMP_OFF);
		ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_SHIFT, (key_state & VK_KEY_SHIFT) ? HB_LAMP_ON : HB_LAMP_OFF);

		key_state_prev = key_state;
	}

	static int hangul_prev = -1;
	int hangul = IsImeHangul ();

	if (hangul_prev != hangul) {
		SetDlgItemText (IDC_EDIT_IME_HAN_ENG, hangul ? "한글" : "영어");

		hangul_prev = hangul;
	}
}


void CDialogKeyMouseStatus::OnKeyboardEvent (BYTE vk_code, bool vk_pressed)
{
	char buff[256];
	sprintf (buff, "%s - %s", 
		GetVkDescFromVK (vk_code), 
		vk_pressed ? "누름" : "뗌");

	SetDlgItemText (IDC_EDIT_INPUT_KEY, buff);
}

void CDialogKeyMouseStatus::OnMouseEvent (BYTE vk_code, bool vk_pressed, const POINT &pt)
{
	char buff[256];
	sprintf (buff, "%d, %d", pt.x, pt.y);

	SetDlgItemText (IDC_EDIT_MOUSE_POS, buff);

	if (vk_code == VK_LBUTTON)      ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_LEFT,   vk_pressed ? HB_LAMP_ON : HB_LAMP_OFF);
	else if (vk_code == VK_MBUTTON) ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_MIDDLE, vk_pressed ? HB_LAMP_ON : HB_LAMP_OFF);
	else if (vk_code == VK_RBUTTON) ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_RIGHT,  vk_pressed ? HB_LAMP_ON : HB_LAMP_OFF);
	else if (vk_code == VK_XBUTTON1)ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_X1,     vk_pressed ? HB_LAMP_ON : HB_LAMP_OFF);
	else if (vk_code == VK_XBUTTON2)ReplaceStaticControlBitmap (m_hWnd, IDC_STATIC_LAMP_X2,     vk_pressed ? HB_LAMP_ON : HB_LAMP_OFF);
}

void CDialogKeyMouseStatus::OnBnClickedOk()
{
	ShowWindow (SW_HIDE);
	// OnOK();
}

void CDialogKeyMouseStatus::OnBnClickedCancel()
{
	ShowWindow (SW_HIDE);
	// OnCancel();
}
