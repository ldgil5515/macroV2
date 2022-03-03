#pragma once

class CHyperLink : public CStatic
{
public:
	CHyperLink();
	virtual ~CHyperLink();

	void SetURL (CString url);
	void SetToolTipText (CString toolTipText);
	void SetLinkCursor (HCURSOR hCursor) { _hCursor = hCursor; }

private:
	bool		_visited;
	HCURSOR		_hCursor;
    CFont		_font;
	CString		_url;
	CToolTipCtrl _tooltip;

	DECLARE_MESSAGE_MAP()
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnStnClicked();
};
