#include "stdafx.h"
#include "key_macro.h"
#include "DialogDelay.h"
#include "virtual_key.h"
#include "Common.h"
#include <assert.h>

// 프로그램이 실행되는 동안 대화상자 아이템에 
// 설정된 값을 저장하고 있는 변수들을 static으로 선언
static long _delay = 10;
static long _flags = 0;
static long _xy    = 0;

CDialogDelay *g_delayDlg = NULL;


IMPLEMENT_DYNAMIC(CDialogDelay, CDialog)

CDialogDelay::CDialogDelay(CWnd* pParent /*=NULL*/)
	: CDialogCommon(CDialogDelay::IDD, pParent)
{
	g_delayDlg = this;
}

CDialogDelay::~CDialogDelay()
{
	g_delayDlg = NULL;
}

void CDialogDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RANDOM, _comboRandom);
}


BEGIN_MESSAGE_MAP(CDialogDelay, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CDialogDelay::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_DELAY, &CDialogDelay::OnBnClickedRadioDelay)
	ON_BN_CLICKED(IDC_RADIO_TIME, &CDialogDelay::OnBnClickedRadioTime)
	ON_BN_CLICKED(IDC_RADIO_KEY, &CDialogDelay::OnBnClickedRadioKey)
	ON_BN_CLICKED(IDC_RADIO_COLOR, &CDialogDelay::OnBnClickedRadioColor)
	ON_BN_CLICKED(IDC_CHECK_PUSH, &CDialogDelay::OnBnClickedCheckPush)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_PICK, &CDialogDelay::OnBnClickedButtonColorPick)
	ON_EN_CHANGE(IDC_EDIT_R, &CDialogDelay::OnEnChangeEditR)
	ON_EN_CHANGE(IDC_EDIT_G, &CDialogDelay::OnEnChangeEditG)
	ON_EN_CHANGE(IDC_EDIT_B, &CDialogDelay::OnEnChangeEditB)
	ON_BN_CLICKED(IDC_CHECK_MODIFIER, &CDialogDelay::OnBnClickedCheckModifier)
END_MESSAGE_MAP()


BOOL CDialogDelay::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (_item.type == MI_NONE) {
		SetWindowText ("대기/시간지연 추가");
		// 기본값 설정
		_item.type = MI_DELAY;
		_item.delay.delay = _delay;
		_item.delay.flags = _flags;
		_item.delay.xy    = _xy;
	}
	else {
		SetWindowText ("대기/시간지연 편집");
	}

	// 라디오버튼 선택
	CheckDlgButton (IDC_RADIO_DELAY, ((_item.delay.flags&0xF1) == 0x00) ? BST_CHECKED : BST_UNCHECKED);	// 0x00 - 시간지연(delay) 설
	CheckDlgButton (IDC_RADIO_TIME,  ((_item.delay.flags&0xF1) == 0x01) ? BST_CHECKED : BST_UNCHECKED);	// 0x01 - 지연시각(time) 설정
	CheckDlgButton (IDC_RADIO_KEY,   ((_item.delay.flags&0xF1) == 0x10) ? BST_CHECKED : BST_UNCHECKED);	// 0x10 - 키입력 대기
	CheckDlgButton (IDC_RADIO_COLOR, ((_item.delay.flags&0xF1) == 0x20) ? BST_CHECKED : BST_UNCHECKED);	// 0x20 - 화면 색 변환 대기
	
	// 시간 지연 추가
	char text[32+1];
	for (int i=0; i<=100; i+=10) {
		_snprintf (text, 32, "+- %d%%", i);
		text[32] = '\0';

		_comboRandom.AddString (text);
	}
	if ((_item.delay.flags&0xF1) == 0x00) {
		SetDlgItemDouble (IDC_EDIT_DELAY, (double)_item.delay.delay/1000.);
		_comboRandom.SetCurSel (BOUND(_item.delay.xy, 0, 10));
	}
	else {
		SetDlgItemDouble (IDC_EDIT_DELAY, 1.);
		_comboRandom.SetCurSel (0);
	}

	// 지연 시각 추가
	if ((_item.delay.flags&0xF1) == 0x01) {
		int flag = (_item.delay.flags>>1)&0x03;
		FillComboBoxNumber (IDC_COMBO_HOUR, 24, (flag >= 3) ? (_item.delay.delay/(60*60*1000))%24 + 1 : 0);
		FillComboBoxNumber (IDC_COMBO_MIN , 60, (flag >= 2) ? (_item.delay.delay/(   60*1000))%60 + 1 : 0);
		FillComboBoxNumber (IDC_COMBO_SEC , 60, (flag >= 1) ? (_item.delay.delay/(      1000))%60 + 1 : 0);
	}
	else {
		FillComboBoxNumber (IDC_COMBO_HOUR, 24, 0);
		FillComboBoxNumber (IDC_COMBO_MIN , 60, 0);
		FillComboBoxNumber (IDC_COMBO_SEC , 60, 0);
	}

	// 키 입력 대기 추가
	if ((_item.delay.flags&0xF1) == 0x10) {
		FillComboBoxVkList (IDC_COMBO_KEY, LOBYTE(_item.delay.delay));

		CheckDlgButton (IDC_CHECK_PUSH,	   !(_item.delay.delay & VK_KEY_UP		) ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton (IDC_CHECK_CTRL,		(_item.delay.delay & VK_KEY_CTRL	) ? BST_CHECKED : BST_UNCHECKED);	
		CheckDlgButton (IDC_CHECK_ALT,		(_item.delay.delay & VK_KEY_ALT		) ? BST_CHECKED : BST_UNCHECKED);	
		CheckDlgButton (IDC_CHECK_SHIFT,	(_item.delay.delay & VK_KEY_SHIFT	) ? BST_CHECKED : BST_UNCHECKED);	
		CheckDlgButton (IDC_CHECK_MODIFIER, (_item.delay.delay & VK_MODIFIER_KEY) ? BST_CHECKED : BST_UNCHECKED);	
	}
	else {
		FillComboBoxVkList (IDC_COMBO_KEY,   '1');

		CheckDlgButton (IDC_CHECK_PUSH,	  BST_UNCHECKED);
		CheckDlgButton (IDC_CHECK_CTRL,	  BST_UNCHECKED);	
		CheckDlgButton (IDC_CHECK_ALT,	  BST_UNCHECKED);	
		CheckDlgButton (IDC_CHECK_SHIFT,  BST_UNCHECKED);	
		CheckDlgButton (IDC_CHECK_MODIFIER,  BST_UNCHECKED);	
	}

	// 화면 색 대기 추가
	if ((_item.delay.flags&0xF1) == 0x20) {
		int color = _item.delay.delay & 0x00FFFFFF;
		int tol   = BOUND((_item.delay.delay >> 24)&0xFF, 0, 127);

		SetDlgItemInt (IDC_EDIT_R,   GetRValue (color), FALSE);
		SetDlgItemInt (IDC_EDIT_G,   GetGValue (color), FALSE);
		SetDlgItemInt (IDC_EDIT_B,   GetBValue (color), FALSE);
		SetDlgItemInt (IDC_EDIT_X,   (int)(short)LOWORD (_item.delay.xy), TRUE);
		SetDlgItemInt (IDC_EDIT_Y,   (int)(short)HIWORD (_item.delay.xy), TRUE);
		SetDlgItemInt (IDC_EDIT_TOL, tol, FALSE);
	}
	else {
		SetDlgItemInt (IDC_EDIT_R,   255, FALSE);
		SetDlgItemInt (IDC_EDIT_G,   255, FALSE);
		SetDlgItemInt (IDC_EDIT_B,   255, FALSE);
		SetDlgItemInt (IDC_EDIT_X,   0,   TRUE);
		SetDlgItemInt (IDC_EDIT_Y,   0,   TRUE);
		SetDlgItemInt (IDC_EDIT_TOL, 10,  FALSE);
	}

	char message[1024+1];
	_snprintf (message, 1024, "%s를 눌러 화면 색과 마우스 위치를 캡쳐합니다.", GetVkDescFromVK(g_ini.keyMacroColorStart));
	message[1024] = '\0';
	SetDlgItemText (IDC_STATIC_MESSAGE, message);

	/////////////////////////////////////////////////////////
	OnBnClickedCheckPush();

	OnBnClickedRadioDelay();
	OnBnClickedRadioTime();
	OnBnClickedRadioKey();
	OnBnClickedRadioColor();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogDelay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogCommon::OnPaint() for painting messages

	int color = RGB (
		BOUND(GetDlgItemInt (IDC_EDIT_R, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_G, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_B, 0, FALSE), 0, 255));

	CRect rect;
	GetDlgItem(IDC_STATIC_COLOR)->GetWindowRect (&rect);
	ScreenToClient(rect);

	CBrush brush_color (color);

	HGDIOBJ penOld = dc.SelectObject (::GetStockObject (NULL_PEN));
	HGDIOBJ brushOld = dc.SelectObject (brush_color);

	dc.Rectangle (rect.left+1, rect.top+1, rect.right-0, rect.bottom-0);

	dc.SelectObject (penOld);
	dc.SelectObject (brushOld);
}

void CDialogDelay::OnKeyboardEvent (BYTE vk_code, bool vk_pressed)
{
	if (IsDlgButtonChecked (IDC_RADIO_COLOR) == BST_CHECKED) {
		if (g_ini.keyMacroColorStart == vk_code && vk_pressed) {
			POINT p;
			if (GetCursorPos(&p)) {
				CDC *dc = CDC::FromHandle(::GetDC(NULL));   
				if (dc) {
					COLORREF color = dc->GetPixel (p.x, p.y);
					ReleaseDC (dc);

					SetDlgItemInt (IDC_EDIT_R, GetRValue (color), FALSE);
					SetDlgItemInt (IDC_EDIT_G, GetGValue (color), FALSE);
					SetDlgItemInt (IDC_EDIT_B, GetBValue (color), FALSE);
				}
				SetDlgItemInt (IDC_EDIT_X, p.x, TRUE);
				SetDlgItemInt (IDC_EDIT_Y, p.y, TRUE);

				InvalidateRect (NULL, FALSE);
			}
		}
	}
}

void CDialogDelay::OnBnClickedOk()
{
	// flags에 대한 설명:
	// - bit 0    : 0인 경우 delay 설정, 1인 경우 time 설정
	// - bit 1, 2 : 1인 경우 초만 설정, 2인경우 초와 분 설정, 3인 경우 시 분 초 설정

	if (IsDlgButtonChecked (IDC_RADIO_DELAY)) {
		_flags = 0x00;
		// delay는 밀리초 단위로 설정
		_delay = (int)(1000*GetDlgItemDouble (IDC_EDIT_DELAY));
		_delay = BOUND(_delay, 0L, 86400000L);

		_xy    = _comboRandom.GetCurSel ();
	}
	else if (IsDlgButtonChecked (IDC_RADIO_TIME)) {
		int hour = SendDlgItemMessage (IDC_COMBO_HOUR, CB_GETCURSEL) - 1;
		int min_ = SendDlgItemMessage (IDC_COMBO_MIN,  CB_GETCURSEL) - 1;
		int sec_ = SendDlgItemMessage (IDC_COMBO_SEC,  CB_GETCURSEL) - 1;

		// 시/분/초 로 표시되는 시각을 msec 단위의 시간으로 바꾼다.
		_delay = 0;
		int time_flags = 0;
		if (sec_ >= 0) {	
			_delay += sec_*1000;	 
			time_flags = 1; 
		
			if (min_ >= 0) {	
				_delay += min_*60*1000;	 
				time_flags = 2; 
			
				if (hour >= 0) {	
					_delay += hour*60*60*1000;	 
					time_flags = 3; 
				}
			}
		}
		_flags = 0x01 | (time_flags<<1);

		if (time_flags == 0) {
			AfxMessageBox ("지연 시각이 제대로 설정되지 않았습니다.");
			return;
		}
		_xy = 0;	// 사용안함
	}
	else if (IsDlgButtonChecked (IDC_RADIO_KEY)) {
		int index = SendDlgItemMessage (IDC_COMBO_KEY, CB_GETCURSEL, 0, 0);
		if (index != -1)										_delay = GetVkCode(index);
		if (IsDlgButtonChecked (IDC_CHECK_PUSH)		!= BST_CHECKED)	_delay |= VK_KEY_UP;
		if (IsDlgButtonChecked (IDC_CHECK_CTRL)		== BST_CHECKED)	_delay |= VK_KEY_CTRL;
		if (IsDlgButtonChecked (IDC_CHECK_ALT)		== BST_CHECKED)	_delay |= VK_KEY_ALT;
		if (IsDlgButtonChecked (IDC_CHECK_SHIFT)	== BST_CHECKED)	_delay |= VK_KEY_SHIFT;
		if (IsDlgButtonChecked (IDC_CHECK_MODIFIER)	== BST_CHECKED)	_delay |= VK_MODIFIER_KEY;
		
		_flags = 0x10;
		_xy    = 0;
	}
	else if (IsDlgButtonChecked (IDC_RADIO_COLOR)) {
		_delay = RGB (
			BOUND(GetDlgItemInt (IDC_EDIT_R, 0, FALSE), 0, 255),
			BOUND(GetDlgItemInt (IDC_EDIT_G, 0, FALSE), 0, 255),
			BOUND(GetDlgItemInt (IDC_EDIT_B, 0, FALSE), 0, 255));
		_delay |= BOUND (GetDlgItemInt (IDC_EDIT_TOL, 0, FALSE), 0, 255) << 24;
		
		_xy = MAKELONG (
			BOUND(GetDlgItemInt (IDC_EDIT_X, 0, TRUE), -32768, 32767),
			BOUND(GetDlgItemInt (IDC_EDIT_Y, 0, TRUE), -32768, 32767));

		_flags = 0x20;
	}
	else {
		assert (0);
	}

	_item.delay.delay = _delay;
	_item.delay.flags = _flags;
	_item.delay.xy    = _xy;

	OnOK();
}

void CDialogDelay::EnableDlgItem (int sel)
{
	GetDlgItem (IDC_EDIT_DELAY)  ->EnableWindow (sel == 0);
	GetDlgItem (IDC_COMBO_RANDOM)->EnableWindow (sel == 0);

	GetDlgItem (IDC_COMBO_HOUR)  ->EnableWindow (sel == 1);
	GetDlgItem (IDC_COMBO_MIN)   ->EnableWindow (sel == 1);
	GetDlgItem (IDC_COMBO_SEC)   ->EnableWindow (sel == 1);

	BOOL enable = (sel == 2) && (IsDlgButtonChecked (IDC_CHECK_MODIFIER) == BST_CHECKED);

	GetDlgItem (IDC_COMBO_KEY)   ->EnableWindow (sel == 2);
	GetDlgItem (IDC_CHECK_PUSH)  ->EnableWindow (sel == 2);
	GetDlgItem (IDC_CHECK_CTRL)  ->EnableWindow (enable);
	GetDlgItem (IDC_CHECK_ALT)   ->EnableWindow (enable);
	GetDlgItem (IDC_CHECK_SHIFT) ->EnableWindow (enable);
	GetDlgItem (IDC_CHECK_MODIFIER) ->EnableWindow (sel == 2);

	GetDlgItem (IDC_EDIT_R)      ->EnableWindow (sel == 3);
	GetDlgItem (IDC_EDIT_G)      ->EnableWindow (sel == 3);
	GetDlgItem (IDC_EDIT_B)      ->EnableWindow (sel == 3);
	GetDlgItem (IDC_EDIT_X)      ->EnableWindow (sel == 3);
	GetDlgItem (IDC_EDIT_Y)      ->EnableWindow (sel == 3);
	GetDlgItem (IDC_EDIT_TOL)    ->EnableWindow (sel == 3);
	GetDlgItem (IDC_BUTTON_COLOR_PICK) ->EnableWindow (sel == 3);
	GetDlgItem (IDC_STATIC_MESSAGE)    ->EnableWindow (sel == 3);
}

void CDialogDelay::OnBnClickedRadioDelay() { if (IsDlgButtonChecked (IDC_RADIO_DELAY)) { EnableDlgItem (0); } }
void CDialogDelay::OnBnClickedRadioTime()  { if (IsDlgButtonChecked (IDC_RADIO_TIME))  { EnableDlgItem (1); } }
void CDialogDelay::OnBnClickedRadioKey()   { if (IsDlgButtonChecked (IDC_RADIO_KEY))   { EnableDlgItem (2); } }
void CDialogDelay::OnBnClickedRadioColor() { if (IsDlgButtonChecked (IDC_RADIO_COLOR)) { EnableDlgItem (3); } }

void CDialogDelay::OnBnClickedCheckPush()
{
	if (IsDlgButtonChecked (IDC_CHECK_PUSH) == BST_CHECKED) {
		SetDlgItemText (IDC_CHECK_PUSH, "누를 때");
	}
	else {
		SetDlgItemText (IDC_CHECK_PUSH, "뗄 때");
	}
}

void CDialogDelay::OnBnClickedButtonColorPick()
{
	int color = RGB (
		BOUND(GetDlgItemInt (IDC_EDIT_R, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_G, 0, FALSE), 0, 255),
		BOUND(GetDlgItemInt (IDC_EDIT_B, 0, FALSE), 0, 255));

	CColorDialog dlg(color);
	if (dlg.DoModal() == IDOK) {
		color = dlg.GetColor();
	
		SetDlgItemInt (IDC_EDIT_R, GetRValue (color), FALSE);
		SetDlgItemInt (IDC_EDIT_G, GetGValue (color), FALSE);
		SetDlgItemInt (IDC_EDIT_B, GetBValue (color), FALSE);

		InvalidateRect (NULL, FALSE);
	}
}

void CDialogDelay::OnEnChangeEditR() { InvalidateRect (NULL, FALSE); }
void CDialogDelay::OnEnChangeEditG() { InvalidateRect (NULL, FALSE); }
void CDialogDelay::OnEnChangeEditB() { InvalidateRect (NULL, FALSE); }

void CDialogDelay::OnBnClickedCheckModifier()
{
	if (IsDlgButtonChecked (IDC_RADIO_KEY)) {
		BOOL enable = (IsDlgButtonChecked (IDC_CHECK_MODIFIER) == BST_CHECKED);

		GetDlgItem (IDC_CHECK_CTRL)  ->EnableWindow (enable);
		GetDlgItem (IDC_CHECK_ALT)   ->EnableWindow (enable);
		GetDlgItem (IDC_CHECK_SHIFT) ->EnableWindow (enable);
	}
}
