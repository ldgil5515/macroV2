#pragma once
#include "macro_def.h"

class CDialogCS : public CDialog
{
	DECLARE_DYNAMIC(CDialogCS)

public:
	CDialogCS(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DIALOG_ADD_CS };

	sMacroItem _item;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
