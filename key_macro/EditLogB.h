#pragma once 
#include <afxwin.h>
#include <vector>
#include <string>
//#include "LockBlock.h"

using namespace std;

class CEditLogB : public CEdit
{
public:
	CEditLogB();
	virtual ~CEditLogB();

private:
	bool _init;
	int _maxLine;
	int _textLength;

	vector<string> _logList;

	CBrush _brush;
	CFont _font;
	//CLock _lock;

public:
	int PrintLog (const char* msg); 

	void Update ();
	void SetMaxLines (int maxLine) { _maxLine = maxLine; _init = false; }

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};
