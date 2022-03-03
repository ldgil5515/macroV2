#pragma once
#include "afxwin.h"
#include "Hyperlink.h"
#include "DialogCommon.h"

class Ckey_macroDlg : public CDialogCommon
{
public:
	Ckey_macroDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~Ckey_macroDlg();

	enum { IDD = IDD_KEY_MACRO_DIALOG };

	void OnKeyMouseEvent (BYTE vk_code, bool vk_pressed, bool key);

private:
	DWORD _macro_exec_time;
	char  _cmd_line[1024+1];

	CListBox _listMacros;
	CListBox _listMacros2;
	CHyperLink _staticHomepage;

	void ToggleKeyMessage ();
	void SetTextRecCount();
	void EnableDlgItem(BOOL enable);
	void CheckAlwaysontop();
	void StartStopMark ();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	HICON m_hIcon;
	HICON m_hIcon2;

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioRun();
	afx_msg void OnBnClickedRadioEdit();
	afx_msg void OnBnClickedButtonAddMacro();
	afx_msg void OnBnClickedButtonDeleteMacro();
	afx_msg void OnBnClickedButtonCopyMacro();
	afx_msg void OnBnClickedButtonEditMacro();
	afx_msg void OnBnClickedButtonMacroFile();
	afx_msg void OnBnClickedButtonRecMacro();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnLbnDblclkListMacros();
	afx_msg void OnBnClickedButtonKeyMouseStatus();
	afx_msg void OnBnClickedButtonMacroDebug();
};
