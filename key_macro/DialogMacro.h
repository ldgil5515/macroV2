#pragma once
#include "macro_def.h"

class CDialogMacro : public CDialog
{
	DECLARE_DYNAMIC(CDialogMacro)

public:
	CDialogMacro(CWnd* pParent = NULL);   // standard constructor

	int _caller_index;
	sMacroItem _item;

	enum { IDD = IDD_DIALOG_ADD_MACRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
