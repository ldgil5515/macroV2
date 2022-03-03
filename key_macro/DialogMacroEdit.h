#pragma once
#include "macro_def.h"
#include "ListBoxEx.h"

class CDialogMacroEdit : public CDialog
{
	DECLARE_DYNAMIC(CDialogMacroEdit)

public:
	CDialogMacroEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogMacroEdit();

	void OnKeyMouseEvent (BYTE vk_code, bool vk_pressed, bool key);

	sMacro    *_macro;
	int _macro_index;

	enum { IDD = IDD_DIALOG_MACRO_EDIT };

private:
	DWORD _macro_exec_time;
	int   _dlg_cx;
	int   _dlg_cy;

	int LbGetAddIndex ();
	bool CheckMacroLoop ();
	void SetTextRecCount();
	void AddItem (int sel, sMacroItem &_item);
	void EditItem (int sel, sMacroItem &_item);
	void EnableWindowItem(BOOL enable);
	void MessagePump();

private:
	CListBoxEx _listItems;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnDblclkListItems();
	afx_msg void OnBnClickedCheckHideMacroTest();
	afx_msg void OnBnClickedButtonAddKey();
	afx_msg void OnBnClickedButtonAddMouse();
	afx_msg void OnBnClickedButtonAddDelay();
	afx_msg void OnBnClickedButtonAddString();
	afx_msg void OnBnClickedButtonAddMacro();
	afx_msg void OnBnClickedButtonAddCs();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonDisplayMouse();
	afx_msg void OnBnClickedButtonEraseMouse();
	afx_msg void OnBnClickedButtonTestRun();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonMacroStartStop();
	afx_msg void OnBnClickedButtonAddLoop();
};
