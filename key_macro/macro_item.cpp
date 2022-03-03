#include "stdafx.h"
#include "macro_item.h"
#include "macro_def.h"
#include "AppIni.h"
#include "Common.h"
#include "hangul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void sKeybd::Step (bool vk_pressed[256])
{
	bool hangul      = (flags & 0x10) ? true : false;
	bool extended    = (flags & 0x20) ? true : false;
	BYTE vk_code     = LOBYTE (scan_vk_code);
	BYTE scan_code   = HIBYTE (scan_vk_code);

	// Scan code가 설정되어 있지 않으면 Virtual key로부터 Scan code를 얻어온다.
	if (scan_code == 0) scan_code = MapVirtualKey (vk_code, MAPVK_VK_TO_VSC);	

	if (g_ini.macroOptions.CHECK_IME_HAN_ENG) {
		SetImeHangul (hangul);
	}

	if (flags & 0x01) {
		if      (vk_code == VK_LBUTTON)  mouse_event (MOUSEEVENTF_LEFTDOWN,   0, 0, 0, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_RBUTTON)  mouse_event (MOUSEEVENTF_RIGHTDOWN,  0, 0, 0, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_MBUTTON)  mouse_event (MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_XBUTTON1) mouse_event (MOUSEEVENTF_XDOWN,      0, 0, XBUTTON1, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_XBUTTON2) mouse_event (MOUSEEVENTF_XDOWN,      0, 0, XBUTTON2, EX_KEY_MACRO_ITSELF);
		else {
			int flags = extended ? KEYEVENTF_EXTENDEDKEY : 0;
			keybd_event (vk_code, scan_code, flags, EX_KEY_MACRO_ITSELF);
		}
		vk_pressed[(BYTE)vk_code] = true;
	}
	if (flags & 0x02) {
		if      (vk_code == VK_LBUTTON)  mouse_event (MOUSEEVENTF_LEFTUP,   0, 0, 0, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_RBUTTON)  mouse_event (MOUSEEVENTF_RIGHTUP,  0, 0, 0, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_MBUTTON)  mouse_event (MOUSEEVENTF_MIDDLEUP, 0, 0, 0, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_XBUTTON1) mouse_event (MOUSEEVENTF_XUP,      0, 0, XBUTTON1, EX_KEY_MACRO_ITSELF);
		else if (vk_code == VK_XBUTTON2) mouse_event (MOUSEEVENTF_XUP,      0, 0, XBUTTON2, EX_KEY_MACRO_ITSELF);
		else {
			int flags = KEYEVENTF_KEYUP;
			if (extended) flags |= KEYEVENTF_EXTENDEDKEY;
			keybd_event (vk_code, scan_code, flags, EX_KEY_MACRO_ITSELF);
		}
		vk_pressed[(BYTE)vk_code] = false;
	}
}

void sMouse::Step (bool vk_pressed[256])
{
	long mx = x;
	long my = y;

	// TRACE ("%d %d ---", mx, my);
	if (flags&MOUSEEVENTF_ABSOLUTE) {
		if (flags&MOUSEEVENTF_SCREEN_INDEPENDENT_POS);
		else ScreenIndependentMousePos (mx, my);
	}
	else {
		// 상대적인 움직임일 때는 마우스 커서의 현재 위치에 상대 위치를 더해준다.
		POINT cp;
		GetCursorPos(&cp);

		mx = cp.x + mx;
		my = cp.y + my;
		
		ScreenIndependentMousePos (mx, my);
	}

	// TRACE ("%d %d \n", mx, my);

	DWORD flags_bt_down = flags & (
		MOUSEEVENTF_LEFTDOWN   | MOUSEEVENTF_RIGHTDOWN | 
		MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_XDOWN     | 
		MOUSEEVENTF_MOVE       | MOUSEEVENTF_WHEEL     );
	DWORD flags_bt_up = flags & (
		MOUSEEVENTF_LEFTUP   | MOUSEEVENTF_RIGHTUP | 
		MOUSEEVENTF_MIDDLEUP | MOUSEEVENTF_XUP	   );

	if (flags_bt_down & MOUSEEVENTF_MOVE) {
		flags_bt_down |= MOUSEEVENTF_ABSOLUTE;
	}

	DWORD dwData = (short)HIWORD(flags); // Wheel delta 값은 음수가 될 수 있다.
	if (!(flags & (MOUSEEVENTF_WHEEL | MOUSEEVENTF_XDOWN | MOUSEEVENTF_XUP))) {
		dwData = 0;
	}

	if (flags_bt_down) {
		mouse_event (flags_bt_down, mx, my, dwData, EX_KEY_MACRO_ITSELF);
	}
	if (flags_bt_up) {
		mouse_event (flags_bt_up,   mx, my, dwData, EX_KEY_MACRO_ITSELF);
	}
	
	bool vk_pressed_ = false;
	BYTE vk_code = MouseButton2VkCode (flags, vk_pressed_);
		
	vk_pressed[vk_code] = vk_pressed_;
}

int sDelay::GetTimeDelay (int flag, int delay)
{
	time_t current_time = time(NULL);
	struct tm *ts = localtime (&current_time);

	if (flag == 3) {
		int tt = ts->tm_hour*60*60*1000 + ts->tm_min*60*1000 + ts->tm_sec*1000;
		return (delay > tt) ? delay - tt : 24*60*60*1000 + (delay - tt);
	}
	else if (flag == 2) {
		int tt = ts->tm_min*60*1000 + ts->tm_sec*1000;
		return (delay > tt) ? delay - tt : 60*60*1000 + (delay - tt);
	}
	else if (flag == 1) {
		int tt = ts->tm_sec*1000;
		return (delay > tt) ? delay - tt : 60*1000 + (delay - tt);
	}
	else {
		return 1000;
	}
}

int sDelay::GetRandomDelay (int delay, int random)
{
	if (random > 0) {
		double random_delay = (rand()%(2*random+1) - random)/1000.;
		return delay + (int)(delay*random_delay);
	}
	else {
		return delay;
	}
}

bool sDelay::StepDelay (long &time_remain, int dt)
{
	if (time_remain == 0) {
		int random_delay = BOUND (xy, 0, 10);
		time_remain = GetRandomDelay (delay, random_delay*100); // random_delay: 0 ~ 100 %
	}
	else {
		time_remain -= dt;
	}
	if (time_remain <= 0) { 
		time_remain = 0;
		return true;
	}
	return false;
}

bool sDelay::StepTime (long &time_remain, int dt)
{
	if (time_remain == 0) {
		time_remain = GetTimeDelay ((flags>>1)&0x03, delay);
	}
	else {
		time_remain -= dt;
	}
	if (time_remain <= 0) { 
		time_remain = 0;
		return true;
	}
	return false;
}

bool sDelay::StepKey (long &wait_key)
{
	if (wait_key == 0) {
		wait_key = delay; 
	}
	else if (wait_key == -1) {
		wait_key = 0;
		return true;
	}
	return false;
}

bool sDelay::StepColor (long &wait_color, long &wait_xy)
{
	if (wait_color == 0) {
		wait_color = delay; 
		wait_xy    = xy;
	}
	else if (wait_color == -1) {
		wait_color = 0;
		wait_xy = 0;
		return true;
	}
	return false;
}

BYTE sString::GetVK (uchar ch, bool *shift_key)
{
	if ('a' <= ch && ch <= 'z') {
		*shift_key = false;
		return ch + 'A' - 'a';
	}
	if ('A' <= ch && ch <= 'Z') {
		*shift_key = true;
		return ch;
	}
	switch (ch) {
	case '`':	*shift_key = false; return 0xC0;
	case '~':	*shift_key = true;  return 0xC0;
	case '!':	*shift_key = true;  return 0x31;
	case '@':	*shift_key = true;  return 0x32;
	case '#':	*shift_key = true;  return 0x33;
	case '$':	*shift_key = true;  return 0x34;
	case '%':	*shift_key = true;  return 0x35;
	case '^':	*shift_key = true;  return 0x36;
	case '&':	*shift_key = true;  return 0x37;
	case '*':	*shift_key = true;  return 0x38;
	case '(':	*shift_key = true;  return 0x39;
	case ')':	*shift_key = true;  return 0x30;
	case '-':	*shift_key = false; return 0xBD;
	case '_':	*shift_key = true;  return 0xBD;
	case '=':	*shift_key = false; return 0xBB;
	case '+':	*shift_key = true;  return 0xBB;
	case '\\':	*shift_key = false; return 0xDC;
	case '|':	*shift_key = true;  return 0xDC;
	case '[':	*shift_key = false; return 0xDB;
	case '{':	*shift_key = true;  return 0xDB;
	case ']':	*shift_key = false; return 0xDD;
	case '}':	*shift_key = true;  return 0xDD;
	case ';':	*shift_key = false; return 0xBA;
	case ':':	*shift_key = true;  return 0xBA;
	case '\'':	*shift_key = false; return 0xDE;
	case '"':	*shift_key = true;  return 0xDE;
	case ',':	*shift_key = false; return 0xBC;
	case '<':	*shift_key = true;  return 0xBC;
	case '.':	*shift_key = false; return 0xBE;
	case '>':	*shift_key = true;  return 0xBE;
	case '/':	*shift_key = false; return 0xBF;
	case '?':	*shift_key = true;  return 0xBF;
	}

	*shift_key = false; 
	return ch;
}

void sString::KeybdEventHangul (WORD ch)
{
	SetImeHangul (true);

	ch = KS2KSSM (ch);
	int cho  = (ch >> 10) & 0x1F;
	int jung = (ch >> 5 ) & 0x1F;
	int jong = (ch >> 0 ) & 0x1F;

	PrintKeyChosung  (cho);
	PrintKeyJungsung (jung);
	PrintKeyJongsung (jong);
}

void sString::KeybdEvent (BYTE ch)
{
	SetImeHangul (false);

	bool shift_key = false;
	BYTE vk_code = GetVK (ch, &shift_key);
	UINT sc_vk = MapVirtualKey (vk_code, MAPVK_VK_TO_VSC);
	UINT sc_lshift = MapVirtualKey (VK_LSHIFT, MAPVK_VK_TO_VSC);
	
	if (shift_key) keybd_event (VK_LSHIFT, sc_lshift, KEYEVENTF_EXTENDEDKEY, EX_KEY_MACRO_ITSELF);
	keybd_event (vk_code, sc_vk, KEYEVENTF_EXTENDEDKEY, EX_KEY_MACRO_ITSELF);
	keybd_event (vk_code, sc_vk, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, EX_KEY_MACRO_ITSELF); 
	if (shift_key) keybd_event (VK_LSHIFT, sc_lshift, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, EX_KEY_MACRO_ITSELF);
}

void sString::KeybdEventNum (int count)
{
	char buff[32+1];
	
	_snprintf (buff, 32, "%d", count);
	buff[32] = '\0';

	for (int i=0, n=strlen(buff); i<n; i++) {
		KeybdEvent (buff[i]);
	}
}

bool sString::Step (long &str_index)
{
	// 주석인 경우 패스
	if (flags & 0x10) return true;

	if (str_index < length) {
		BYTE ch = string[str_index];

		if (0x07 <= ch && ch <= 0x7F) {		// 영어 and 제어문자
			if      ((flags & 0x01) && (ch == '#')) KeybdEvent (GetUpperRand ());
			else if ((flags & 0x01) && (ch == '*')) KeybdEvent (GetLowerRand ());
			else if ((flags & 0x01) && (ch == '?')) KeybdEvent (GetNumberRand ());
			else if ((flags & 0x02) && (ch == '$')) KeybdEventNum (g_sequence_count++);
			else									KeybdEvent (ch);
		}
		else if (ch & 0x80) {				// 한글 (2-byte 완성형)
			WORD ch2 = (WORD)ch<<0 | ((WORD)string[++str_index] << 8);
			KeybdEventHangul (ch2);
		}

		++str_index;
		return false;
	}
	else {
		str_index = 0;
		return true;
	}
}

bool sMCall::Step (long &str_index)
{
	if (str_index == 0) {
		int id = GetMacroID (name);
		if (0 <= id) {
			// 병렬 실행, 순차적 실행이 선택되어 있고 선택된 매크로가
			// 실행 중이 아니라면 선택된 매크로를 실행한다.
			if ((flags == 0 || flags == 1) && !g_macros[id].is_running ()) {
				g_macros[id].start();
			}
			// 실행 중단이 선택되어 있고 선택된 매크로가 실행 중이라면
			// 선택된 매크로를 중단한다.
			else if (flags == 2 && g_macros[id].is_running ()) {
				g_macros[id].stop();
			}
		}
		else {
			str_index = 0;
			return true;
		}

		if (flags == 1 || flags == 2) {	// 병렬 실행, 실행 중단
			// 서브 매크로 실행을 시작/중단 하고, 다음 매크로 항목을 실행한다.
			str_index = 0;
			return true;
		}
		else {	 				// 순차적 실행
			// 서브 매크로 실행을 시작하고, 끝날 때까지 대기한다.
			str_index = 1;
		}
	}
	else {
		int id = GetMacroID (name);
		if (0 <= id) {
			if (!g_macros[id].is_running ()) {
				str_index = 0;
				return true;
			}
		}
		else {
			str_index = 0;
			return true;
		}
	}
	return false;
}

bool sLock::Step (bool &cs_enter)
{
	if(flags & 0x01) {
		// Macro의 Critical Section으로 진입
		if (g_critical_section_count == 0) { 
			g_critical_section_count++;
			cs_enter = true;
			return true;
		}
		else if (cs_enter) {
			// 이미 Critical Section으로 들어와 있는 경우는 무시하고 지나간다.
			return true;
		}
	}
	else {
		// Macro의 Critical Section을 탈출
		if (g_critical_section_count > 0 && cs_enter) {
			cs_enter = false;
			g_critical_section_count--;
		}
		return true;
	}
	return false;
}


bool sLoop::Step (long &index, vector<int> &loop_count)
{
	int loop_flag = flags & 0x07;

	if (loop_flag == 0) {		// 반복구간으로 들어감
		// Macro의 Loop로 진입
		//if (flags & 0x08) { // 사용자에게 루프 조건 질문
		//}
		loop_count.push_back (start);
		return true;
	}
	else if (loop_flag == 1){	// 반복 숫자를 키보드 이벤트로 출력
		int &count = loop_count.back();

		sString str;
		str.KeybdEventNum (count);
		return true;
	}
	else if (loop_flag == 2){	// 반복 숫자를 마우스 X 위치로 설정
		POINT p;

		if (GetCursorPos(&p)) {
			int &count = loop_count.back();

			p.x = count;
			ScreenIndependentMousePos (p.x, p.y);
			mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, p.x, p.y, (short)HIWORD(flags), EX_KEY_MACRO_ITSELF);
		}
		return true;
	}
	else if (loop_flag == 3){	// 반복 숫자를 마우스 Y 위치로 설정
		POINT p;

		if (GetCursorPos(&p)) {
			int &count = loop_count.back();

			p.y = count;
			ScreenIndependentMousePos (p.x, p.y);
			mouse_event (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, p.x, p.y, (short)HIWORD(flags), EX_KEY_MACRO_ITSELF);
		}
		return true;
	}
	else if (loop_flag == 7){	// 반복 구간에서 나옴
		int &count = loop_count.back();

		count += step;
		if (count > stop) {
			loop_count.pop_back ();
			return true;
		}
		else {
			index = start;
			return false;
		}
	}
	return true;
}

void sMacroItem::Delete () 
{
	if      (type == MI_STRING) delete [] string.string;
	else if (type == MI_MCALL)  delete [] mcall.name;
}

void sMacroItem::Copy (const sMacroItem &mi) 
{
	type = mi.type;
	memcpy (&data[0], &mi.data[0], 3*sizeof(long));

	if (type == MI_STRING) {
		string.string = new char [string.length + 1];
		memcpy (string.string, mi.string.string, string.length + 1);
	}
	else if (type == MI_MCALL) {
		mcall.name = new char [256];
		memcpy (mcall.name, mi.mcall.name, 256);
	}
}
