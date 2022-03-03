#include "stdafx.h"
#include "key_macro.h"
#include "DialogMacroEdit.h"
#include "DialogString.h"
#include "DialogMacro.h"
#include "DialogDelay.h"
#include "DialogKeyboard.h"
#include "DialogMouse.h"
#include "DialogCS.h"
#include "DialogLoop.h"
#include "DialogMacroStartStop.h"
#include "virtual_key.h"
#include "key_hook.h"
#include "AppIni.h"
#include "MmTimer.h"
#include "Common.h"
#include "ScopedLock.h"


CDialogMacroEdit             *g_editDlg     = NULL;
static CDialogMouse          *_mouseDlg     = NULL;

extern const char *GetItemDescription (const sMacroItem &item);


// Multimedia timer로 Macro 실행에 실시간성 확보
static class CMmtMacro: public CMmTimer {
public:
	virtual void OnTimer (UINT timerId, UINT msg) 
	{ 
		if (g_editDlg) {
			g_editDlg->PostMessage (WM_MM_TIMER, 0, 0);
		}
	}
} _mmtm;


IMPLEMENT_DYNAMIC(CDialogMacroEdit, CDialog)

CDialogMacroEdit::CDialogMacroEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMacroEdit::IDD, pParent)
{
	g_editDlg = this;
	_macro = NULL;
	_macro_index = -1;
}

CDialogMacroEdit::~CDialogMacroEdit()
{
	g_editDlg = NULL;
}

void CDialogMacroEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEMS, _listItems);
}


BEGIN_MESSAGE_MAP(CDialogMacroEdit, CDialog)
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST_ITEMS, &CDialogMacroEdit::OnLbnDblclkListItems)
	ON_BN_CLICKED(IDC_CHECK_HIDE_MACRO_TEST, &CDialogMacroEdit::OnBnClickedCheckHideMacroTest)
	ON_BN_CLICKED(IDOK, &CDialogMacroEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogMacroEdit::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD_KEY, &CDialogMacroEdit::OnBnClickedButtonAddKey)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MOUSE, &CDialogMacroEdit::OnBnClickedButtonAddMouse)
	ON_BN_CLICKED(IDC_BUTTON_ADD_DELAY, &CDialogMacroEdit::OnBnClickedButtonAddDelay)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MACRO, &CDialogMacroEdit::OnBnClickedButtonAddMacro)
	ON_BN_CLICKED(IDC_BUTTON_ADD_STRING, &CDialogMacroEdit::OnBnClickedButtonAddString)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CS, &CDialogMacroEdit::OnBnClickedButtonAddCs)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LOOP, &CDialogMacroEdit::OnBnClickedButtonAddLoop)
	ON_BN_CLICKED(IDC_BUTTON_MACRO_START_STOP, &CDialogMacroEdit::OnBnClickedButtonMacroStartStop)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDialogMacroEdit::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDialogMacroEdit::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDialogMacroEdit::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDialogMacroEdit::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_MOUSE, &CDialogMacroEdit::OnBnClickedButtonDisplayMouse)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_MOUSE, &CDialogMacroEdit::OnBnClickedButtonEraseMouse)
	ON_BN_CLICKED(IDC_BUTTON_TEST_RUN, &CDialogMacroEdit::OnBnClickedButtonTestRun)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDialogMacroEdit::OnBnClickedButtonStop)
END_MESSAGE_MAP()


BOOL CDialogMacroEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if (!_macro->name[0]) {
		SetWindowText ("매크로 추가");

		int name_id = 0;
		do {
			_snprintf (_macro->name, 255, "Macro %d", ++name_id);
			_macro->name[255] = '\0';
		} while (GetMacroID(_macro->name) >= 0);

		_macro->start_key =  VK_F2;
		_macro->stop_key  =  VK_F3;
		_macro->options   =  MAKELONG(MACRO_KEY_START | MACRO_KEY_STOP, 1);

		// 제일 첫줄에 아무것도 아닌 항목을 삽입해 둔다.
		// 이유는, 선택한 항목 뒤에다가 새로운 항목을 추가할 수 있기때문에,
		// 이 MI_NONE이 제일 처음에 없으면 첫줄에 새로운 항목을 추가할 수 없다.
		_macro->_item.resize(1);
		_macro->_item[0].type = MI_NONE;
	}
	else {
		SetWindowText ("매크로 편집");
	}

	// 매크로 테스트 숨기기/보이기 선택 옵션에 따라 대화상자 크기를 조절하기위해
	// 대화상자의 가로, 세로 크기를 저장해 둔다.
	RECT rect;
	GetWindowRect (&rect);
	_dlg_cx = rect.right - rect.left;
	_dlg_cy = rect.bottom - rect.top;

	CheckDlgButton (IDC_CHECK_HIDE_MACRO_TEST, g_ini.macroOptions.HIDE_MACRO_TEST ? BST_CHECKED : BST_UNCHECKED);
	OnBnClickedCheckHideMacroTest ();

	char message[1024+1];
	_snprintf (message, 1024, "%s를 눌러 현재 마우스 위치를 매크로 항목에 추가합니다. "
		"Insert key를 누르면 매크로 이름과 모든 내용을 클립보드에 텍스트로 복사합니다. ", 
		GetVkDescFromVK (g_ini.keyMousePosCapture));
	message[1024] = '\0';
	SetDlgItemText (IDC_STATIC_MESSAGE1, message);

	SetDlgItemText (IDC_EDIT_MACRO_NAME, _macro->name);

	CheckDlgButton (IDC_CHECK_START_STOP_KEY, g_ini.editOptions.START_STOP_KEY ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHECK_SELECTED_ITEM,  g_ini.editOptions.SELECTED_ITEM  ? BST_CHECKED : BST_UNCHECKED);

	SetDlgItemInt (IDC_EDIT_MACRO_REPEAT_COUNT, (int)(ushort)HIWORD(_macro->options));

	GetDlgItem (IDC_BUTTON_STOP)->EnableWindow (FALSE);

	_listItems.SetMacroItems (&_macro->_item);
	for (uint i=0; i<_macro->_item.size(); ++i) {
	   _listItems.InsertString (i, GetItemDescription (_macro->_item[i]));
	}
	_listItems.SetSel (0, TRUE);

	SetTextRecCount ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogMacroEdit::SetTextRecCount()
{
	int depth = 0;
	long exec_time = _macro->ExecTime (depth);

	char text[256+1];
	if (exec_time >= 100000000) {
		_snprintf (text, 256, "%d개 항목이 기록됨 (순환호출!)", 
			_macro->_item.size()-1);
	}
	else {
		_snprintf (text, 256, "%d개 항목이 기록됨 (%g 초)", 
			_macro->_item.size()-1, exec_time/1000.);
	}
	text[256] = '\0';

	SetDlgItemText (IDC_STATIC_REC_COUNT, text);
}

extern bool CheckSameKey (int macro_vk, int vk_input);

void CDialogMacroEdit::OnKeyMouseEvent (BYTE vk_code, bool vk_pressed, bool key)
{
	if (IsDlgButtonChecked (IDC_CHECK_START_STOP_KEY) == BST_CHECKED) {
		// ReadStartStopVk();
		// 여기 수정 필요....

		int vk_input = vk_code;
		vk_input |= GetCtrlAltShiftKeyState ();
		if (!vk_pressed) vk_input |= VK_KEY_UP;

		if (CheckSameKey (_macro->start_key, vk_input)) {
			if (GetDlgItem(IDC_BUTTON_TEST_RUN)->IsWindowEnabled ()) {
				OnBnClickedButtonTestRun();
			}
		}
		else if (CheckSameKey (_macro->stop_key, vk_input)) {
			if (GetDlgItem(IDC_BUTTON_STOP)->IsWindowEnabled ()) {
				OnBnClickedButtonStop ();
			}
		}
	}
	
	if (g_ini.keyMousePosCapture == vk_code && vk_pressed) {
		if (_mouseDlg) {
			_mouseDlg->OnkeyMouseEvent ();
		}
		else {
			int sel = _listItems.GetLastSel ();
			if (0 <= sel) sel++; // 만일 선택된 경우, 선택된 항목 뒤에다 추가하려고 1 증가
			else sel = _listItems.GetCount ();

			if (0 <= sel) {
				POINT cp;
				GetCursorPos(&cp);

				sMacroItem item;
				item.type        = MI_MOUSE;
				item.mouse.flags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
				item.mouse.x     = cp.x;
				item.mouse.y     = cp.y;
				// if (GetAsyncKeyState(VK_LBUTTON)) item.mouse.flags |= MOUSEEVENTF_LEFTDOWN;

				AddItem (sel, item);
			}
		}
	}

	if (vk_code == VK_INSERT && !vk_pressed) {
		CString text = "매크로 이름: ";
		text += _macro->name;
		text += "\r\n\r\n";

		for (uint i=0; i<_macro->_item.size(); ++i) {
		   text += GetItemDescription (_macro->_item[i]);
			text += "\r\n";
		}

		const size_t len = text.GetLength() + 1;

		HGLOBAL hGlobal = GlobalAlloc (GMEM_MOVEABLE, len);
		PSTR pGlobal = (PSTR)GlobalLock (hGlobal);
		CopyMemory (pGlobal, LPCSTR(text), len);
		pGlobal[len-1] = '\0';
		GlobalUnlock (hGlobal);
		
		OpenClipboard ();
		EmptyClipboard ();
		SetClipboardData (CF_TEXT, hGlobal);
		CloseClipboard();	

		//GlobalFree (hMem);
	}
}

LRESULT CDialogMacroEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case WM_MM_TIMER: {
		if (_macro && _macro->is_running ()) {
			DWORD current_time = CMmTimer::GetTime ();
			_macro->MacroStep (current_time - _macro_exec_time);
			_macro_exec_time = current_time;
		}
		break;
	}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CDialogMacroEdit::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1000) {
		int index = _macro->index;
		int size = _macro->_item.size();

		if (0 <= index && index < size) {
			_listItems.SelItemRange (FALSE, 0, size);
		   _listItems.SetSel (index, TRUE);
		}
		else {
			OnBnClickedButtonStop();
			_listItems.SelItemRange (FALSE, 0, size);
		   _listItems.SetSel (0, TRUE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDialogMacroEdit::AddItem (int sel, sMacroItem &_item)
{
	_macro->_item.insert (_macro->_item.begin() + sel, _item);

	_listItems.InsertString (sel, GetItemDescription (_item));
	_listItems.SelItemRange (FALSE, 0, _listItems.GetCount ());
	_listItems.SetSel(sel, TRUE);
}

void CDialogMacroEdit::EditItem (int sel, sMacroItem &_item)
{
	_macro->_item[sel] = _item;

	_listItems.DeleteString (sel);
	_listItems.InsertString (sel, GetItemDescription (_item));
	_listItems.SelItemRange (FALSE, 0, _listItems.GetCount ());
	_listItems.SetSel(sel, TRUE);
}

int CDialogMacroEdit::LbGetAddIndex ()
{
	int sel = _listItems.GetLastSel ();
	if (0 <= sel) {
		return sel+1; // 만일 선택된 경우, 선택된 항목 뒤에다 추가하려고 1 증가
	}
	else {
		return _listItems.GetCount ();
	}
}

void CDialogMacroEdit::OnBnClickedButtonAddString()
{
	int sel = LbGetAddIndex ();
	
	CDialogString dlg;
	if (dlg.DoModal () == IDOK) AddItem (sel, dlg._item);
	
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonAddKey()
{
	int sel = LbGetAddIndex ();
	
	CDialogKeyboard dlg;
	if (dlg.DoModal () == IDOK) AddItem (sel, dlg._item);
	
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonAddMouse()
{
	int sel = LbGetAddIndex ();
	
	CDialogMouse dlg;
	_mouseDlg = &dlg;
	if (dlg.DoModal () == IDOK) AddItem (sel, dlg._item);
	_mouseDlg = NULL;
	
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonAddDelay()
{
	int sel = LbGetAddIndex ();
	
	CDialogDelay dlg;
	if (dlg.DoModal () == IDOK) AddItem (sel, dlg._item);
	
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonAddMacro()
{
	int sel = LbGetAddIndex ();
	
	CDialogMacro dlg;
	dlg._caller_index = _macro_index;	// 현재 편집 중인 매크로의 index값 넘겨줌
	if (dlg.DoModal () == IDOK) AddItem (sel, dlg._item);
	
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonAddCs()
{
	int sel = LbGetAddIndex ();
	
	CDialogCS dlg;
	if (dlg.DoModal () == IDOK) AddItem (sel, dlg._item);
	
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonAddLoop()
{
	int sel = LbGetAddIndex ();
	
	CDialogLoop dlg;
	if (dlg.DoModal () == IDOK) AddItem (sel, dlg._item);
	
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonMacroStartStop()
{
	CDialogMacroStartStop dlg;
	
	dlg._macro = _macro;
	dlg.DoModal ();
}

void CDialogMacroEdit::OnBnClickedButtonEdit()
{
	int sel = _listItems.GetFirstSel ();
	if (1 <= sel) { // 처음 아이템은 편집하지 못하도록!
		switch (_macro->_item[sel].type) {
		case MI_KEYBD: {
			CDialogKeyboard dlg;
			dlg._item = _macro->_item[sel];

			if (dlg.DoModal () == IDOK) EditItem (sel, dlg._item);
			break; }
		case MI_MOUSE: {
			CDialogMouse dlg;
			_mouseDlg = &dlg;
			dlg._item = _macro->_item[sel];

			if (dlg.DoModal () == IDOK) EditItem (sel, dlg._item);
			_mouseDlg = NULL;
			break; }
		case MI_DELAY: {
			CDialogDelay dlg;
			dlg._item = _macro->_item[sel];

			if (dlg.DoModal () == IDOK) EditItem (sel, dlg._item);
			break; }
		case MI_STRING: {
			CDialogString dlg;
			dlg._item = _macro->_item[sel];

			if (dlg.DoModal () == IDOK) EditItem (sel, dlg._item);
			break; }
		case MI_MCALL: {
			CDialogMacro dlg;
			dlg._caller_index = _macro_index;	// 현재 편집 중인 매크로의 index값 넘겨줌
			dlg._item = _macro->_item[sel];

			if (dlg.DoModal () == IDOK) EditItem (sel, dlg._item);
			break; }
		case MI_LOCK: {
			CDialogCS dlg;
			dlg._item = _macro->_item[sel];

			if (dlg.DoModal () == IDOK) EditItem (sel, dlg._item);
			break; }
		case MI_LOOP: {
			CDialogLoop dlg;
			dlg._item = _macro->_item[sel];

			if (dlg.DoModal () == IDOK) EditItem (sel, dlg._item);
			break; }
		}
	}
	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonDelete()
{
	int count = _listItems.GetSelCount();
	CArray<int,int> arr;

	arr.SetSize(count);
	_listItems.GetSelItems(count, arr.GetData()); 

	// 지울 때는 마지막 선택 항목부터 거꾸로 지운다.
	for (int i=arr.GetSize()-1; i>=0; i--) {
		int sel = arr.GetAt(i);
		if (1 <= sel) { // 처음 아이템은 지우지 못하도록!
			_listItems.DeleteString (sel);

			_macro->_item.erase (_macro->_item.begin() + sel);
		}
	}

	SetTextRecCount();
	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonUp()
{
	int count = _listItems.GetSelCount();
	CArray<int,int> arr;

	arr.SetSize(count);
	_listItems.GetSelItems(count, arr.GetData()); 

	// 세 번째 아이템부터 위로 올릴 수 있다.
	if (arr.GetSize() == 0) return;
	if (arr.GetAt(0) < 2) return;

	for (int i=0; i<arr.GetSize(); i++) {
		int sel = arr.GetAt(i);

		// sel-1과 sel item을 스왑 한다.
		int ss = _listItems.GetSel (sel-1);
		_listItems.DeleteString (sel);
		_listItems.InsertString (sel-1, GetItemDescription (_macro->_item[sel]));
		_listItems.SetSel (sel-1, TRUE);
		if (ss) _listItems.SetSel (sel, TRUE);

		std::swap (_macro->_item[sel], _macro->_item[sel-1]);
	}

	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonDown()
{
	int count = _listItems.GetSelCount();
	CArray<int,int> arr;

	arr.SetSize(count);
	_listItems.GetSelItems(count, arr.GetData()); 

	// 처음 아이템과 마지막 아이템은 아래로 내릴 수 없다.
	if (arr.GetSize() == 0) return;
	if (arr.GetAt(0) == 0) return;
	if (arr.GetAt(arr.GetSize()-1) >= _listItems.GetCount ()-1) return;

	for (int i=arr.GetSize()-1; i>=0; i--) {
		int sel = arr.GetAt(i);

		// sel+1과 sel item을 스왑 한다.
		int ss = _listItems.GetSel (sel+1);
	   _listItems.DeleteString (sel);
	   _listItems.InsertString (sel+1, GetItemDescription (_macro->_item[sel]));
	   _listItems.SetSel (sel+1, TRUE);
		if (ss) _listItems.SetSel (sel, TRUE);

	   std::swap (_macro->_item[sel], _macro->_item[sel+1]);
	}

	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnLbnDblclkListItems()
{
	OnBnClickedButtonEdit();
}

bool CDialogMacroEdit::CheckMacroLoop ()
{
	int cs_count = 0;
	vector<int> stack;

	for (uint i=0; i<_macro->_item.size(); ++i) {
		sMacroItem &mi = _macro->_item[i];

		if (mi.type == MI_LOCK) {
			if (mi.lock.flags & 0x01) {			// <<< 독점실행구간으로 들어감
				if (cs_count++ != 0) return false;

				stack.push_back (0);
			}
			else {								// >>> 독점실행구간으로부터 나옴
				if (--cs_count != 0) return false;

				if (!stack.empty()) {
					if (stack.back() != 0) return false;
					stack.pop_back();
				}
				else return false;
			}
		}
		if (mi.type == MI_LOOP) {
			ushort loop_flag = (mi.loop.flags & 0x07);

			if (loop_flag == 0) {				// <<< 반복구간으로 들어감
				stack.push_back (1);
			}
			else if ((loop_flag == 1) ||		// --- 반복 숫자를 키보드 이벤트로 출력
				(loop_flag == 2) ||				// --- 반복 숫자를 마우스 X 위치로 출력
				(loop_flag == 3) ){				// --- 반복 숫자를 마우스 Y 위치로 출력
				
				bool find = false;
				for (uint j=0; j<stack.size(); j++) {
					if (stack[j] == 1) { find = true; break; }
				}
				if (!find) return false;
			}
			else if (loop_flag == 7) {			// >>> 반복구간에서 나옴
				if (!stack.empty()) {
					if (stack.back() != 1) return false;
					stack.pop_back();
				}
				else return false;
			}
		}
	}
	
	if (!stack.empty()) return false;

	return true;
}

void CDialogMacroEdit::OnBnClickedOk()
{
	g_ini.editOptions.START_STOP_KEY = (IsDlgButtonChecked (IDC_CHECK_START_STOP_KEY) == BST_CHECKED) ? 1 : 0;
	g_ini.editOptions.SELECTED_ITEM  = (IsDlgButtonChecked (IDC_CHECK_SELECTED_ITEM)  == BST_CHECKED) ? 1 : 0;
	
	GetDlgItemText (IDC_EDIT_MACRO_NAME, _macro->name, 255);
	_macro->name[255] = 0;

	int repeat_count = GetDlgItemInt (IDC_EDIT_MACRO_REPEAT_COUNT);
	repeat_count = BOUND(repeat_count, 0, 65535);

	// _macro->options = MAKELONG(0, repeat_count);
	_macro->options = MAKELONG(LOWORD(_macro->options), repeat_count);

	if (!_macro->name[0]) {
		AfxMessageBox ("매크로 이름이 지정되지 않았습니다. 이름을 입력하시기 바랍니다.");
	}
	else if (GetMacroID (_macro->name, _macro_index) >= 0) {
		AfxMessageBox ("같은 이름의 매크로가 존재합니다. 다른 이름을 입력하시기 바랍니다.");
	}
	else if (!CheckMacroLoop ()) {
		AfxMessageBox ("매크로 내에서 반복구간이나 독점실행구간이 올바르게 열리고 닫히지 않았습니다.");
	}
	else {
		OnOK();
	}
}

void CDialogMacroEdit::OnBnClickedCancel()
{
	g_ini.editOptions.START_STOP_KEY = (IsDlgButtonChecked (IDC_CHECK_START_STOP_KEY) == BST_CHECKED) ? 1 : 0;
	g_ini.editOptions.SELECTED_ITEM  = (IsDlgButtonChecked (IDC_CHECK_SELECTED_ITEM)  == BST_CHECKED) ? 1 : 0;

	OnCancel();
}

void CDialogMacroEdit::EnableWindowItem(BOOL enable)
{
	GetDlgItem(IDC_EDIT_MACRO_NAME)			->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_MACRO_START_STOP) ->EnableWindow (enable);

	GetDlgItem(IDC_BUTTON_ADD_KEY)    ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_ADD_MOUSE)  ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_ADD_DELAY)  ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_ADD_STRING) ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_ADD_MACRO)  ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_ADD_CS)     ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_ADD_LOOP)   ->EnableWindow (enable);

	GetDlgItem(IDC_BUTTON_EDIT)       ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_DELETE)     ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_UP)         ->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_DOWN)       ->EnableWindow (enable);

	GetDlgItem(IDC_EDIT_MACRO_REPEAT_COUNT)->EnableWindow (enable);

	GetDlgItem(IDOK)                    ->EnableWindow (enable);
	GetDlgItem(IDCANCEL)                ->EnableWindow (enable);

	GetDlgItem(IDC_BUTTON_TEST_RUN)		->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_STOP)			->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_DISPLAY_MOUSE)->EnableWindow (enable);
	GetDlgItem(IDC_BUTTON_ERASE_MOUSE)  ->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_START_STOP_KEY)->EnableWindow (enable);
	GetDlgItem(IDC_CHECK_SELECTED_ITEM) ->EnableWindow (enable);
}

void CDialogMacroEdit::MessagePump()
{
	MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {   
        TranslateMessage(&msg);   
        DispatchMessage(&msg);   
		Sleep (0);
    }   
}

void CDialogMacroEdit::OnBnClickedButtonDisplayMouse()
{
	CDC *dc = CDC::FromHandle(::GetDC(NULL));   
	if (!dc) return;

	EnableWindowItem (FALSE);

	CPen pen_blue (PS_SOLID, 2, RGB(0,0,255));
	CPen pen_red (PS_SOLID, 2, RGB(255,0,0));
	CBrush brush_red (RGB(128,0,0));

	HGDIOBJ penOld   = dc->SelectObject (&pen_red);
	HGDIOBJ brushOld = dc->SelectObject (&brush_red);

	int count = _listItems.GetSelCount();
	CArray<int,int> arrayListSel;

	arrayListSel.SetSize(count);
	_listItems.GetSelItems(count, arrayListSel.GetData()); 

	int mouse_count = 0;
	for (uint i=0; i<_macro->_item.size(); ++i) {
		sMacroItem &mi = _macro->_item[i];

		if (mi.type == MI_MOUSE) {
			long mx = mi.mouse.x;
			long my = mi.mouse.y;
			
			if (mi.mouse.flags&MOUSEEVENTF_SCREEN_INDEPENDENT_POS){
				ScreenDependentMousePos (mx, my);
			}

			if (!mouse_count) dc->MoveTo (mx, my);
			else              dc->LineTo (mx, my);
			mouse_count++;

			const int MOUSE_BUTTON_DOWN = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_XDOWN;
			if (mi.mouse.flags & MOUSE_BUTTON_DOWN) {
				const int r = 4;
				dc->Rectangle (mx-r, my-r, mx+r, my+r);
			}

			if (IsDlgButtonChecked (IDC_CHECK_SELECTED_ITEM) == BST_CHECKED) {
				for (int j=0; j<arrayListSel.GetSize(); j++) {
					if (arrayListSel.GetAt(j) == i) {
						dc->SelectObject (&pen_blue);
						dc->SelectObject (GetStockObject(NULL_BRUSH));

						const int r1 = 8, r2 = 12;
						dc->Rectangle (mx-r1, my-r1, mx+r1, my+r1);
						dc->Rectangle (mx-r2, my-r2, mx+r2, my+r2);

						dc->SelectObject (&pen_red);
						dc->SelectObject (&brush_red);
					}
				}
			}
		}
		MessagePump ();
	}

	dc->SelectObject (penOld);
	dc->SelectObject (brushOld);
	ReleaseDC (dc);

	EnableWindowItem (TRUE);

	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonEraseMouse()
{
	::InvalidateRect (NULL, NULL, FALSE);

	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonTestRun()
{
	EnableWindowItem (FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow (TRUE);

	SetTimer (1000, 33, NULL);
	_mmtm.Start (g_ini.eventDelay);

	int sel = _listItems.GetLastSel ();
	_macro->start ((sel < 1) ? 1 : sel);

	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedButtonStop()
{
	if (_macro->is_running()) _macro->stop ();
	
	KillTimer (1000);
	_mmtm.Stop ();

	EnableWindowItem (TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow (FALSE);

	_listItems.SetFocus ();
}

void CDialogMacroEdit::OnBnClickedCheckHideMacroTest()
{
	g_ini.macroOptions.HIDE_MACRO_TEST = IsDlgButtonChecked (IDC_CHECK_HIDE_MACRO_TEST);
	int dy = (g_ini.macroOptions.HIDE_MACRO_TEST) ? (int)(-_dlg_cy*0.185) : 0;

	SetWindowPos (NULL, 0, 0, _dlg_cx, _dlg_cy + dy, SWP_NOMOVE | SWP_NOZORDER);
}

