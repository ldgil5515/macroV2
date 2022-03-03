#include "stdafx.h"
#include "key_macro.h"
#include "virtual_key.h"
#include "DialogKeyboard.h"
#include "key_hook.h"
#include "AppIni.h"

CDialogKeyboard  *g_keyDlg = NULL;

// 프로그램이 실행되는 동안 대화상자 아이템에 
// 설정된 값을 저장하고 있는 변수들을 static으로 선언
static long _vk_code = 0x30;
static long _flags = 0x03;
// 숨겨진 기능, 사용자가 설정할 수 없다.
static bool _extended = false;	// Extended key(Function 키, 숫자 키패드의 키)임을 표시


IMPLEMENT_DYNAMIC(CDialogKeyboard, CDialog)

CDialogKeyboard::CDialogKeyboard(CWnd* pParent /*=NULL*/)
	: CDialogCommon(CDialogKeyboard::IDD, pParent)
{
	g_keyDlg = this;
}

CDialogKeyboard::~CDialogKeyboard()
{
	g_keyDlg = NULL;
}

void CDialogKeyboard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogKeyboard, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogKeyboard::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CDialogKeyboard::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (_item.type == MI_NONE) {
		SetWindowText ("키보드 이벤트 추가");
		// 기본값 설정
		_item.type             = MI_KEYBD;
		_item.keybd.scan_vk_code = MAKEWORD(_vk_code, 0);
		_item.keybd.flags        = _flags;
	}
	else {
		SetWindowText ("키보드 이벤트 편집");
	}

	FillComboBoxVkList (IDC_COMBO_KEYBOARD, _item.keybd.scan_vk_code);

	CheckDlgButton (IDC_RADIO_KEY_3,  ((_item.keybd.flags&0x03) == 0x03) ? BST_CHECKED : BST_UNCHECKED);	// 누르고 떼기
	CheckDlgButton (IDC_RADIO_KEY_1,  ((_item.keybd.flags&0x03) == 0x01) ? BST_CHECKED : BST_UNCHECKED);	// 누르기
	CheckDlgButton (IDC_RADIO_KEY_2,  ((_item.keybd.flags&0x03) == 0x02) ? BST_CHECKED : BST_UNCHECKED);	// 떼기
	CheckDlgButton (IDC_CHECK_HANGUL, ((_item.keybd.flags&0x10) == 0x10) ? BST_CHECKED : BST_UNCHECKED);	// 한글 입력 상태
	_extended = ((_item.keybd.flags&0x10) == 0x20);														// 확장키

	GetDlgItem(IDC_CHECK_HANGUL)->EnableWindow (g_ini.macroOptions.CHECK_IME_HAN_ENG);

	GetDlgItem(IDC_COMBO_KEYBOARD)->SetFocus ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogKeyboard::OnKeyboardEvent (BYTE vk_code, bool vk_pressed)
{
	if (vk_code && vk_pressed) {
		// 키보드 이벤트를 바로 처리하지 않고 timer를 사용하여 30ms 후에
		// 입력된 가상키로 ComboBox 항목을 선택한다: OnTimer() 함수에서 nIDEvent == 1000 참조
		_vk_input = vk_code;

		SetTimer (1000, 30, NULL);
	}
}

void CDialogKeyboard::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1000) {
		if (GetFocus() == GetDlgItem(IDC_COMBO_KEYBOARD)) {
			SendDlgItemMessage (IDC_COMBO_KEYBOARD, CB_SETCURSEL, GetVkIndex (_vk_input));
		}
		KillTimer (1000);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDialogKeyboard::OnBnClickedOk()
{
	// flags에 대한 설명:
	// bit 0, 1 - 1일때 key 누르기만, 2일때 key 떼기만, 3일때 key 누르고 떼기 설정

	int index = SendDlgItemMessage (IDC_COMBO_KEYBOARD, CB_GETCURSEL, 0, 0);
	if (index != -1) {
		_vk_code = GetVkCode(index);
	}

	if      (IsDlgButtonChecked (IDC_RADIO_KEY_3)) _flags = 0x03;	// 누르고 떼기
	else if (IsDlgButtonChecked (IDC_RADIO_KEY_1)) _flags = 0x01;	// 누르기
	else if (IsDlgButtonChecked (IDC_RADIO_KEY_2)) _flags = 0x02;	// 떼기

	if (IsDlgButtonChecked (IDC_CHECK_HANGUL)) _flags |= 0x10;	// 한글입력상태 체크
	if (_extended)                             _flags |= 0x20;	// Extended key임을 표시

	_item.keybd.scan_vk_code = MAKEWORD (_vk_code, 0);	// 편집한 경우, Scan code는 잃어버린다.
	_item.keybd.flags        = _flags;

	OnOK();
}
