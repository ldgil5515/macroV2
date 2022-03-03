#include "stdafx.h"
#include "key_macro.h"
#include "DialogConfig.h"
#include "virtual_key.h"
#include "AppIni.h"


IMPLEMENT_DYNAMIC(CDialogConfig, CDialog)

CDialogConfig::CDialogConfig(CWnd* pParent /*=NULL*/)
	: CDialogCommon(CDialogConfig::IDD, pParent)
{
}

void CDialogConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogConfig, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogConfig::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CDialogConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText (IDC_EDIT_TITLE, g_ini.title);
	SendDlgItemMessage (IDC_COMBO_HANGUL_KEYBOARD,		 CB_SETCURSEL, g_ini.hangulKeyboard);

	FillComboBoxVkList (IDC_COMBO_MACRO_RUN_EDIT_KEY,    g_ini.keyMacroRun);
	FillComboBoxVkList (IDC_COMBO_RECORD_START_STOP_KEY, g_ini.keyMacroRecord);
	FillComboBoxVkList (IDC_COMBO_MOUSE_CAPTURE_KEY,     g_ini.keyMousePosCapture);
	FillComboBoxVkList (IDC_COMBO_COLOR_START_KEY,       g_ini.keyMacroColorStart);
	FillComboBoxVkList (IDC_COMBO_COLOR_STOP_KEY,        g_ini.keyMacroColorStop);

	CheckDlgButton (IDC_CHECK_ALWAYS_ON_TOP,		g_ini.macroOptions.ALWAYS_ON_TOP	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_RUN_SEL_MACRO,		g_ini.macroOptions.RUN_SEL_MACRO	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_TRAY_AT_STARTUP,		g_ini.macroOptions.TRAY_AT_STARTUP	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_MACRO_NOTIFY,			g_ini.macroOptions.MACRO_NOTIFY		? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_START_STOP_MARK,		g_ini.macroOptions.START_STOP_MARK	? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_IME_HAN_ENG,			g_ini.macroOptions.CHECK_IME_HAN_ENG? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_RELEASE_ALL_KEYS,		g_ini.macroOptions.RELEASE_ALL_KEYS ? BST_CHECKED : BST_UNCHECKED);

	SetDlgItemInt (IDC_EDIT_SEQUENCE_START,			g_ini.sequenceStart, FALSE);

	if (g_ini.eventDelay < 1) g_ini.eventDelay = 1;
	SetDlgItemInt (IDC_EDIT_EVENT_MIN_DELAY, g_ini.eventDelay);

	return TRUE;
}

void CDialogConfig::OnBnClickedOk()
{
	GetDlgItemText (IDC_EDIT_TITLE, g_ini.title, MAX_PATH);

	g_ini.hangulKeyboard = SendDlgItemMessage (IDC_COMBO_HANGUL_KEYBOARD, CB_GETCURSEL, 0, 0);

	g_ini.macroOptions.ALWAYS_ON_TOP	= IsDlgButtonChecked (IDC_CHECK_ALWAYS_ON_TOP);
	g_ini.macroOptions.TRAY_AT_STARTUP	= IsDlgButtonChecked (IDC_CHECK_TRAY_AT_STARTUP);
	g_ini.macroOptions.RUN_SEL_MACRO	= IsDlgButtonChecked (IDC_CHECK_RUN_SEL_MACRO);
	g_ini.macroOptions.MACRO_NOTIFY	    = IsDlgButtonChecked (IDC_CHECK_MACRO_NOTIFY);
	g_ini.macroOptions.START_STOP_MARK  = IsDlgButtonChecked (IDC_CHECK_START_STOP_MARK);
	g_ini.macroOptions.CHECK_IME_HAN_ENG= IsDlgButtonChecked (IDC_CHECK_IME_HAN_ENG);
	g_ini.macroOptions.RELEASE_ALL_KEYS = IsDlgButtonChecked (IDC_CHECK_RELEASE_ALL_KEYS);

	g_ini.sequenceStart = GetDlgItemInt (IDC_EDIT_SEQUENCE_START, 0, FALSE);

	g_ini.eventDelay = GetDlgItemInt (IDC_EDIT_EVENT_MIN_DELAY);
	if (g_ini.eventDelay < 1) g_ini.eventDelay = 1;

	int index = SendDlgItemMessage (IDC_COMBO_MACRO_RUN_EDIT_KEY, CB_GETCURSEL, 0, 0);
	if (index != -1) g_ini.keyMacroRun = GetVkCode(index);

	index = SendDlgItemMessage (IDC_COMBO_RECORD_START_STOP_KEY, CB_GETCURSEL, 0, 0);
	if (index != -1) g_ini.keyMacroRecord = GetVkCode(index);

	index = SendDlgItemMessage (IDC_COMBO_MOUSE_CAPTURE_KEY, CB_GETCURSEL, 0, 0);
	if (index != -1) g_ini.keyMousePosCapture = GetVkCode(index);

	index = SendDlgItemMessage (IDC_COMBO_COLOR_START_KEY, CB_GETCURSEL, 0, 0);
	if (index != -1) g_ini.keyMacroColorStart = GetVkCode(index);

	index = SendDlgItemMessage (IDC_COMBO_COLOR_STOP_KEY, CB_GETCURSEL, 0, 0);
	if (index != -1) g_ini.keyMacroColorStop = GetVkCode(index);

	OnOK();
}

void CDialogConfig::OnBnClickedCancel()
{
	OnCancel();
}
