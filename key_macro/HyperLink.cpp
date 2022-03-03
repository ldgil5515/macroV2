#include "stdafx.h"
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TOOLTIP	(1)

CHyperLink::CHyperLink()
{
	_visited = FALSE;
	_hCursor = NULL;
	_url = "";
}

CHyperLink::~CHyperLink()
{
}

void CHyperLink::SetURL(CString url)
{
	url.TrimLeft();
	url.TrimRight();
	_url = url;
}

void CHyperLink::SetToolTipText(CString toolTipText)
{
	_tooltip.UpdateTipText(toolTipText, this, ID_TOOLTIP);
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	ON_CONTROL_REFLECT(STN_CLICKED, &CHyperLink::OnStnClicked)
END_MESSAGE_MAP()


HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(_visited ? RGB(128, 0, 128) : RGB(0, 0, 255));
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);

	// return (HBRUSH)m_brBack;
}

BOOL CHyperLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(_hCursor);

	return TRUE;
}

void CHyperLink::PreSubclassWindow() 
{
	// We want to get mouse clicks via STN_CLICKED
	DWORD dwStyle = GetStyle();
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);

	_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

    LOGFONT log;
    GetFont()->GetLogFont(&log);
    log.lfUnderline = TRUE;

    _font.CreateFontIndirect(&log);
    SetFont(&_font);

	CRect rect; 
	GetClientRect(rect);

	_tooltip.Create(this);
	_tooltip.AddTool(this, _url, rect, ID_TOOLTIP);

	CStatic::PreSubclassWindow();
}

BOOL CHyperLink::PreTranslateMessage(MSG* pMsg) 
{
	_tooltip.RelayEvent(pMsg);
	
	return CStatic::PreTranslateMessage(pMsg);
}

void CHyperLink::OnStnClicked()
{
	_visited = TRUE;
    Invalidate();

	ShellExecute(NULL, "open", _url, NULL, NULL, SW_SHOWNORMAL);
}
