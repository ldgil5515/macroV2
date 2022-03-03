#pragma once


class CDialogUserLoop : public CDialog
{
	DECLARE_DYNAMIC(CDialogUserLoop)

public:
	CDialogUserLoop(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogUserLoop();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_LOOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
