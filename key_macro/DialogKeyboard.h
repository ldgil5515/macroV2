#pragma once
#include "macro_def.h"
#include "DialogCommon.h"

class CDialogKeyboard : public CDialogCommon
{
	DECLARE_DYNAMIC(CDialogKeyboard)

public:
	CDialogKeyboard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogKeyboard();

	void OnKeyboardEvent (BYTE vk_code, bool vk_pressed);

	int _vk_input;
	sMacroItem _item;

	enum { IDD = IDD_DIALOG_ADD_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
};
