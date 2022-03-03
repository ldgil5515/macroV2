#pragma once
#include "DialogCommon.h"

class CDialogKeyMouseStatus : public CDialogCommon
{
	DECLARE_DYNAMIC(CDialogKeyMouseStatus)

public:
	CDialogKeyMouseStatus(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogKeyMouseStatus();

	enum { IDD = IDD_DIALOG_KEY_MOUSE_STATUS };

	void OnKeyboardEvent (BYTE vk_code, bool vk_pressed);
	void OnMouseEvent (BYTE vk_code, bool vk_pressed, const POINT &pt);

	void UpdateControlKeyStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
};
