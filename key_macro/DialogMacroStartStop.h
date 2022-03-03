#pragma once
#include "macro_def.h"
#include "DialogCommon.h"

class CDialogMacroStartStop : public CDialogCommon
{
	DECLARE_DYNAMIC(CDialogMacroStartStop)

public:
	CDialogMacroStartStop(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogMacroStartStop();

	void OnkeyMouseEvent (BYTE vk_code, bool vk_pressed, bool key);

	sMacro    *_macro;

	enum { IDD = IDD_DIALOG_MACRO_START };

private:
	void EnableWindowItem(BOOL enable);

	int _vk_start;
	int _vk_stop;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckStartOption();
	afx_msg void OnBnClickedCheckStopOption();
	afx_msg void OnBnClickedCheckStartKey();
	afx_msg void OnBnClickedCheckStopKey();
	afx_msg void OnBnClickedCheckTime();
	afx_msg void OnBnClickedCheckStartColor();
	afx_msg void OnBnClickedCheckStopColor();
	afx_msg void OnBnClickedButtonStartColorPick();
	afx_msg void OnBnClickedButtonStopColorPick();
	afx_msg void OnEnChangeEditStartR();
	afx_msg void OnEnChangeEditStartG();
	afx_msg void OnEnChangeEditStartB();
	afx_msg void OnEnChangeEditStopR();
	afx_msg void OnEnChangeEditStopG();
	afx_msg void OnEnChangeEditStopB();
	afx_msg void OnBnClickedCheckStartModifier();
	afx_msg void OnBnClickedCheckStopModifier();
};
