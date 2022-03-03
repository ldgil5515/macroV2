#pragma once
#include "macro_def.h"
#include "DialogCommon.h"
#include "EditLogB.h"
#include "afxwin.h"


class CDialogMacroDebug : public CDialogCommon
{
	DECLARE_DYNAMIC(CDialogMacroDebug)

public:
	CDialogMacroDebug(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogMacroDebug();

	enum { IDD = IDD_DIALOG_MACRO_DEBUG };

	void Log(const char *msg);

private:
	sMacro    *_macro;
	CListBox _listItems;
	CEditLogB _editLog;

	void ShowDlgItems (bool enable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboMacro();
	afx_msg void OnBnClickedRadioMacro();
	afx_msg void OnBnClickedRadioEvent();
};
