#include "stdafx.h"
#include "key_macro.h"
#include "DialogUserLoop.h"


IMPLEMENT_DYNAMIC(CDialogUserLoop, CDialog)

CDialogUserLoop::CDialogUserLoop(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogUserLoop::IDD, pParent)
{
}

CDialogUserLoop::~CDialogUserLoop()
{
}

void CDialogUserLoop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogUserLoop, CDialog)
END_MESSAGE_MAP()

