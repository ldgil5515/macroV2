#pragma once
#include <vector>
#include "macro_item.h"
#include "log.h"
#include "AppIni.h"

using namespace std;

extern int g_critical_section_count;
extern int g_sequence_count;

// Macro Start/Stop 옵션
#define	MACRO_AUTO_SS			0x0002
#define MACRO_KEY_START			0x0004
#define MACRO_TIMER_SS			0x0008
#define MACRO_COLOR_START		0x0010
#define MACRO_KEY_STOP			0x0020
#define MACRO_COLOR_STOP		0x0040
#define MACRO_COLOR_ONCE		0x0080


class sMacro {
public:
	// 매크로에 대한 프라퍼티 설정
	char name[256];				// 매크로 이름
	long start_key;				// 매크로 시작 단축 가상키, VK_KEY_UP, VK_KEY_CTRL, VK_KEY_ALT, VK_KEY_SHIFT, VK_MODIFIER_KEY 코드 포함
	long stop_key;				// 매크로 중단 단축 가상키, VK_KEY_UP, VK_KEY_CTRL, VK_KEY_ALT, VK_KEY_SHIFT, VK_MODIFIER_KEY 코드 포함
	long start_time;			// 매크로 시작 시각 설정
	long stop_time;				// 매크로 중단 시각 설정
	long start_color;			// 매크로 시작 색 설정
	long stop_color;			// 매크로 중단 색 설정
	long tolerance;				// 시작과 중단 색에 대한 허용 편차
	long start_xy;				// 매크로 시작 색을 가져오는 화면상의 위치
	long stop_xy;				// 매크로 중단 색을 가져오는 화면상의 위치
	long options;				// 매크로 시작, 중단 등 관련 옵션들

	vector<sMacroItem> _item;	// 매크로 항목(키보드, 마우스 이벤트)들

public:
	// 매크로 실행시 필요한 변수들
	long index;					// 매크로 항목 목록에서 실행되고 있는 항목에 대한 index
	long run_count;
	bool start_color_prev;
	bool stop_color_prev;
	long time_remain;
	long wait_key;
	long wait_color;
	long wait_xy;

private:
	long str_index;				// 문자열에서 출력되고 있는 문자에 대한 색인
	bool vk_pressed[256];		// 이 매크로에 의해 눌린 Virtucal Key code 기록
	bool cs_enter;				// 이 매크로가 Critical Section에 들어감
	vector<int> loop_count;		// sLoop 항목에서 사용하는 카운트 값

	//CDeviceLock lock;
	void make_loop ();
	void init_runtime_value ();

public:
	long ExecTime (int &depth);
	void MacroStep (int dt);

	void start (int index_ = 1); 
	void stop (); 

	bool is_running() 
	{
		return 0 <= index && index < (int)_item.size();
	}

	sMacro () 
		: start_key(0), stop_key(0), 
		start_time(0), stop_time(0), 
		start_color(0), stop_color(0), 
		tolerance (10),
		start_xy(0), stop_xy(0), 
		options(0) 
	{
		init_runtime_value();
		name[0] = '\0';
	}
	
	sMacro (const sMacro &macro) 
	{
		strncpy (name, macro.name, 255);
		name[255] = '\0';
		start_key   = macro.start_key;
		stop_key    = macro.stop_key;
		start_time  = macro.start_time;
		stop_time   = macro.stop_time;
		start_color = macro.start_color;
		stop_color  = macro.stop_color;
		tolerance   = macro.tolerance;
		start_xy    = macro.start_xy;
		stop_xy     = macro.stop_xy;
		options     = macro.options;
		_item       = macro._item;

		init_runtime_value ();
	}

	sMacro &operator = (const sMacro &macro) 
	{
		if (this != &macro) {
			strncpy (name, macro.name, 255);
			name[255] = '\0';
			start_key   = macro.start_key;
			stop_key    = macro.stop_key;
			start_time  = macro.start_time;
			stop_time   = macro.stop_time;
			start_color = macro.start_color;
			stop_color  = macro.stop_color;
			tolerance   = macro.tolerance;
			start_xy    = macro.start_xy;
			stop_xy     = macro.stop_xy;
			options     = macro.options;
			_item       = macro._item;

			init_runtime_value ();
		}
		return *this;
	}
};


extern vector<sMacro> g_macros;

extern void MacroReset ();
extern void MacroInit  ();
extern void MacroTerm  ();

extern void MacroStartStopShortkey (int vk_input);
extern void MacroStartStopColor (CWnd *wnd);
extern void MacroStartStopTimer ();
extern void MacroStart     (int id, int index = 1);
extern void MacroStop      (int id);
extern bool IsMacroRunning ();
extern int  GetMacroID     (const char *name, int except = -1);

extern void AllMacroStep (int dt);
