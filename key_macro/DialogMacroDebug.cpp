#include "stdafx.h"
#include "key_macro.h"
#include "DialogMacroDebug.h"
#include "macro_def.h"

extern const char *GetItemDescription (const sMacroItem &item);

IMPLEMENT_DYNAMIC(CDialogMacroDebug, CDialog)

CDialogMacroDebug::CDialogMacroDebug(CWnd* pParent /*=NULL*/)
	: CDialogCommon(CDialogMacroDebug::IDD, pParent)
{
	_macro = NULL;
}

CDialogMacroDebug::~CDialogMacroDebug()
{
}

void CDialogMacroDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, _listItems);
	DDX_Control(pDX, IDC_EDIT_LOG, _editLog);
}

BEGIN_MESSAGE_MAP(CDialogMacroDebug, CDialog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CDialogMacroDebug::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogMacroDebug::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_MACRO, &CDialogMacroDebug::OnCbnSelchangeComboMacro)
	ON_BN_CLICKED(IDC_RADIO_MACRO, &CDialogMacroDebug::OnBnClickedRadioMacro)
	ON_BN_CLICKED(IDC_RADIO_EVENT, &CDialogMacroDebug::OnBnClickedRadioEvent)
END_MESSAGE_MAP()


void CDialogMacroDebug::Log(const char *msg)
{
	_editLog.PrintLog (msg);
}

BOOL CDialogMacroDebug::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// 윈도우의 위치를 복원한다.
	DlgInScreen (g_ini.debugSX, g_ini.debugSY);
	SetWindowPos (NULL, g_ini.debugSX, g_ini.debugSY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);	

	CheckDlgButton (IDC_RADIO_MACRO, !g_ini.macroOptions.MACRO_DEBUG_LOG ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_RADIO_EVENT,  g_ini.macroOptions.MACRO_DEBUG_LOG ? BST_CHECKED : BST_UNCHECKED);

	OnBnClickedRadioMacro();
	OnBnClickedRadioEvent();

	// 대화상자 컨트롤 초기화
	_editLog.SetWindowText ("");
	_editLog.SetMaxLines (1000);

	SetTimer (1000, 33, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogMacroDebug::OnDestroy()
{
	CDialog::OnDestroy();

	RECT rectWnd;
	GetWindowRect (&rectWnd);
	g_ini.debugSX = rectWnd.left;
	g_ini.debugSY = rectWnd.top;
}

void CDialogMacroDebug::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1000) {
		if (_macro && IsWindowVisible ()) {
			static int sel_prev = -1;

			if (sel_prev != _macro->index) {
				_listItems.SetCurSel(_macro->index);
				_listItems.ShowCaret ();

				sel_prev = _macro->index;
			}

			CString str;

			if (0 < _macro->index && _macro->index < (int)_macro->_item.size()) {
				str.Format ("%d / %d", _macro->index + 1, _macro->_item.size());
			}
			else {
				str.Format ("종료");
			}
			SetDlgItemText (IDC_EDIT1, str);

			str.Format ("%d / %d", _macro->run_count, (int)HIWORD(_macro->options));
			SetDlgItemText (IDC_EDIT2, str);

			str.Format ("%d", _macro->time_remain);
			SetDlgItemText (IDC_EDIT3, str);
		}

		_editLog.Update ();
	}

	CDialog::OnTimer(nIDEvent);
}

void CDialogMacroDebug::OnBnClickedOk()
{
	ShowWindow (SW_HIDE);
	// OnOK();
}

void CDialogMacroDebug::OnBnClickedCancel()
{
	ShowWindow (SW_HIDE);
	// OnCancel();
}

void CDialogMacroDebug::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if (bShow) {
		SendDlgItemMessage (IDC_COMBO_MACRO, CB_RESETCONTENT, 0, 0);

		SendDlgItemMessage (IDC_COMBO_MACRO, WM_SETREDRAW, 0, 0);
		for (uint i=0; i<g_macros.size(); ++i) {
			SendDlgItemMessage (IDC_COMBO_MACRO, CB_INSERTSTRING, i, (LPARAM)g_macros[i].name);
		}
		SendDlgItemMessage (IDC_COMBO_MACRO, WM_SETREDRAW, 1, 0);
		SendDlgItemMessage (IDC_COMBO_MACRO, CB_SETCURSEL, 0);

		OnCbnSelchangeComboMacro();
	}
	else {
		_editLog.SetWindowText ("");
	}
}

void CDialogMacroDebug::OnCbnSelchangeComboMacro()
{
	// TODO: Add your control notification handler code here
	int index = SendDlgItemMessage (IDC_COMBO_MACRO, CB_GETCURSEL, 0, 0);
	if (0 <= index && index < (int)g_macros.size()) {
		_macro = &g_macros[index];

		_listItems.ResetContent();
		for (uint i=0; i<_macro->_item.size(); ++i) {
		   _listItems.InsertString (i, GetItemDescription (_macro->_item[i]));
		}
	}
	else {
		_macro = NULL;
	}
}

void CDialogMacroDebug::ShowDlgItems (bool enable)
{
	GetDlgItem (IDC_COMBO_MACRO)->ShowWindow ( enable ? SW_SHOW : SW_HIDE);
	GetDlgItem (IDC_LIST_ITEMS) ->ShowWindow ( enable ? SW_SHOW : SW_HIDE);
	GetDlgItem (IDC_EDIT1)      ->ShowWindow ( enable ? SW_SHOW : SW_HIDE);
	GetDlgItem (IDC_EDIT2)      ->ShowWindow ( enable ? SW_SHOW : SW_HIDE);
	GetDlgItem (IDC_EDIT3)      ->ShowWindow ( enable ? SW_SHOW : SW_HIDE);
	GetDlgItem (IDC_EDIT_LOG)   ->ShowWindow (!enable ? SW_SHOW : SW_HIDE);
}

void CDialogMacroDebug::OnBnClickedRadioMacro()
{
	if (IsDlgButtonChecked (IDC_RADIO_MACRO)) {
		g_ini.macroOptions.MACRO_DEBUG_LOG = 0;
		ShowDlgItems (true);
	}
}

void CDialogMacroDebug::OnBnClickedRadioEvent()
{
	if (IsDlgButtonChecked (IDC_RADIO_EVENT)) {
		g_ini.macroOptions.MACRO_DEBUG_LOG = 1;
		ShowDlgItems (false);
	}
}
