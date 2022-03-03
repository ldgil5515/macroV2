#include "stdafx.h"
#include "key_macro.h"
#include "DialogCS.h"

// 프로그램이 실행되는 동안 대화상자 아이템에 
// 설정된 값을 저장하고 있는 변수들을 static으로 선언
static long _flags = 0x01;


IMPLEMENT_DYNAMIC(CDialogCS, CDialog)

CDialogCS::CDialogCS(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCS::IDD, pParent)
{
}

void CDialogCS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogCS, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogCS::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogCS::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CDialogCS::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (_item.type == MI_NONE) {
		SetWindowText ("독점실행구간 추가");
		// 기본값 설정
		_item.type = MI_LOCK;
		_item.lock.flags = _flags;
	}
	else {
		SetWindowText ("독점실행구간 편집");
	}

	int lock = _item.lock.flags&0x01;
	CheckDlgButton (IDC_RADIO_CS_LEAVE, !lock ? BST_CHECKED : BST_UNCHECKED);	// 0x00 - 독점 실행구간에서 나옴
	CheckDlgButton (IDC_RADIO_CS_ENTER,  lock ? BST_CHECKED : BST_UNCHECKED);	// 0x01 - 독점 실행구간으로 들어감

	return TRUE;
}

void CDialogCS::OnBnClickedOk()
{
	_flags = 0;
	if (IsDlgButtonChecked (IDC_RADIO_CS_LEAVE)) _flags = 0x00;	// 0x00 - 독점 실행구간에서 나옴
	if (IsDlgButtonChecked (IDC_RADIO_CS_ENTER)) _flags = 0x01;	// 0x01 - 독점 실행구간으로 들어감

	_item.lock.flags = _flags;

	OnOK();
}

void CDialogCS::OnBnClickedCancel()
{
	OnCancel();
}
