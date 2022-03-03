#include "stdafx.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CEditEx::CEditEx ()
{
	ctrl = false;
}

void CEditEx::DoDataExchange(CDataExchange* pDX)
{
	CEdit::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Ctrl+A ±¸Çö
	if (ctrl && nChar == 'A') SetSel(0, -1, TRUE);
	
	if (nChar == VK_CONTROL) ctrl = true;

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL) ctrl = false;

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

