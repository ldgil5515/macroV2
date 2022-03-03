#pragma once
#include "macro_def.h"
#include "EditEx.h"
#include "afxwin.h"

class CDialogString : public CDialog
{
	DECLARE_DYNAMIC(CDialogString)

public:
	CDialogString(CWnd* pParent = NULL);   // standard constructor

	sMacroItem _item;
	CEditEx _editString;

	enum { IDD = IDD_DIALOG_ADD_STRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckComment();
};
