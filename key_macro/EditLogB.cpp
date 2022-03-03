#include "stdafx.h"
#include "EditLogB.h"

CEditLogB::CEditLogB() :
	_init (false),
	_maxLine (1000),
	_textLength(0)
{
	_logList.reserve (_maxLine);
}

CEditLogB::~CEditLogB()
{
}


BEGIN_MESSAGE_MAP(CEditLogB, CEdit)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CEditLogB::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	// Intercept Ctrl + Z (Undo), Ctrl + X (Cut), Ctrl + C (Copy), Ctrl + V (Paste) and Ctrl + A (Select All)
	// before CEdit base class gets a hold of them.
	if (pMsg->message == WM_KEYDOWN && ::GetKeyState(VK_CONTROL) < 0)
		switch (pMsg->wParam) {
		//case 'Z': Undo();        return TRUE;
		//case 'X': Cut();         return TRUE;
		//case 'C': Copy();        return TRUE;
		//case 'V': Paste();       return TRUE;
		case 'A': SetSel(0, -1); return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CEditLogB::OnDestroy() 
{
	CEdit::OnDestroy();
	
	// _font.DeleteObject ();
	//_brush.DeleteObject ();
}

int CEditLogB::PrintLog (const char* msg) 
{
	//LOCK_BLOCK (_lock);

	_logList.push_back (msg);
	return 0;
}

void CEditLogB::Update () 
{
	//LOCK_BLOCK (_lock);

	if (!_init) {
		SetLimitText(_maxLine*1024);
		_init = true;
	}

	int lineCount = GetLineCount ();
	if (_maxLine < lineCount) {
		LockWindowUpdate();

		SetSel (0, LineIndex (lineCount - _maxLine*8/10));
		ReplaceSel ("");

		_textLength = GetWindowTextLength();
		LineScroll(_maxLine, 0);

		UnlockWindowUpdate();
	}

	for (int i=0, n=_logList.size (); i<n; ++i) {
		string &smsg = _logList[i];

		SetSel (_textLength, _textLength);
		ReplaceSel (smsg.c_str ());

		_textLength += smsg.length ();
	}
	_logList.clear ();
}

