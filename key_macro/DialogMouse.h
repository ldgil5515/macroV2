#pragma once
#include "macro_def.h"

class CDialogMouse : public CDialog
{
	DECLARE_DYNAMIC(CDialogMouse)

public:
	CDialogMouse(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogMouse();

	void OnMouseEvent (long x, long y);
	void OnkeyMouseEvent ();

	sMacroItem _item;

	enum { IDD = IDD_DIALOG_ADD_MOUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckMousePosAbsolute();
};
