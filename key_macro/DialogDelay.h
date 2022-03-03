#pragma once
#include "macro_def.h"
#include "afxwin.h"
#include "DialogCommon.h"

class CDialogDelay : public CDialogCommon
{
	DECLARE_DYNAMIC(CDialogDelay)

public:
	CDialogDelay(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogDelay();

	enum { IDD = IDD_DIALOG_ADD_DELAY };

	sMacroItem _item;

	void EnableDlgItem (int sel);
	void OnKeyboardEvent (BYTE vk_code, bool vk_pressed);

private:
	CComboBox _comboRandom;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioDelay();
	afx_msg void OnBnClickedRadioTime();
	afx_msg void OnBnClickedRadioKey();
	afx_msg void OnBnClickedRadioColor();
	afx_msg void OnBnClickedCheckPush();
	afx_msg void OnBnClickedButtonColorPick();
	afx_msg void OnEnChangeEditR();
	afx_msg void OnEnChangeEditG();
	afx_msg void OnEnChangeEditB();
public:
	afx_msg void OnBnClickedCheckModifier();
};
