#pragma once
#include "macro_def.h"

class CDialogMacroRec : public CDialog
{
	DECLARE_DYNAMIC(CDialogMacroRec)

public:
	CDialogMacroRec(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogMacroRec();

	void OnKeyboardEvent (BYTE vk_code, bool vk_pressed, BYTE scan_code);
	void OnMouseEvent (BYTE vk_code, bool vk_pressed, bool move, short wheelDelat, POINT &pt);

	sMacro *_macro;
	CListBox _listItems;

	enum { IDD = IDD_DIALOG_MACRO_REC };

private:
	bool _macro_rec;
	DWORD _rec_time;

	void SetTextRecCount();

	void MacroRecStateChange(bool rec);
	void MacroRecOptionChanged();

	void AddKey (BYTE vk_code, BYTE scan_code, long flags);
	void AddMouse (int x, int y, long flags);
	void AddTimeDelay (long delay);
	void AddTimeDelayIf ();

	void EnableWindowItem(BOOL enable);
	void UpdateLastItem();
	int MouseDistance (int px, int py, int mx, int my);
	int MouseDistance (int px, int py);
	bool LastIsSameKeyDown (long scan_vk_code);
	bool LastIsSameMouseDown (int x, int y, long flags);
	long GetMouseButtonFlags (BYTE vk_code, bool vk_pressed);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckRecStat();
	afx_msg void OnBnClickedCheckRecKey();
	afx_msg void OnBnClickedCheckRecMouse();
	afx_msg void OnBnClickedCheckRecMousePos();
	afx_msg void OnBnClickedCheckTime();
	afx_msg void OnBnClickedCheckRecMouseWheel();
	afx_msg void OnBnClickedCheckMerge();
	afx_msg void OnBnClickedCheckMousePosScrindep();
	afx_msg void OnBnClickedCheckMousePosAbsolute();
	afx_msg void OnBnClickedCheckRecButtonMove();
public:
	afx_msg void OnBnClickedButtonMacroStartStop();
};
