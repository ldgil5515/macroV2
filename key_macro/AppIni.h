#pragma once
#include "Ini.h"


struct sMacorBitOptions {
	DWORD CHECK_IME_HAN_ENG		: 1;	// 0x01 - IME의 한/영 전환기능 감지
	DWORD MACRO_RUN				: 1;     
	DWORD ALWAYS_ON_TOP			: 1;     
	DWORD HIDE_MACRO_TEST		: 1;	// 0x08
	DWORD TRAY_AT_STARTUP		: 1;
	DWORD KEY_REENTRANCE		: 1;
	DWORD MACRO_LOG				: 1;
	DWORD MACRO_NOTIFY			: 1;	// 0x80
	DWORD START_STOP_MARK		: 1;
	DWORD RUN_SEL_MACRO			: 1;
	DWORD MACRO_DEBUG_LOG		: 1;
	DWORD RELEASE_ALL_KEYS		: 1;	//        매크로 종료시 모든 눌린 키/버튼 해제
};

struct sRecBitOptions {
	DWORD REC_KEYBOARD			: 1;	// 0x01 - Keyboard 입력 기록
	DWORD REC_MOUSE_BUTTON		: 1;	// 0x02 - Mouse Button 입력 기록
	DWORD REC_MOUSE_MOVE		: 1;	// 0x04 - Mouse 위치 변화 기록
	DWORD REC_MOUSE_WHEEL		: 1;	// 0x08 - Mouse 휠 변화 기록
	DWORD REC_TIME_DELAY		: 1;	// 0x10 - Keyboard나 Mouse 이벤트간 시간 간격 기록
	DWORD REC_MERGE_UP_DOWN		: 1;	// 0x20 - Button Up/Down 이벤트 합치기 허용
	DWORD SCREEN_INDEP_MOUSE	: 1;	// 0x40 - Mouse 위치 변화를 화면 크기에 독립적으로 기록(0 ~ 65535)
	DWORD MOUSE_POS_ABSOLUTE	: 1;	// 0x80	- Check일때는 Mouse의 절대 위치를 기록, Uncheck일때는 상대적 위치를 기록
	DWORD REC_MOUSE_BUTTON_POS	: 1;	// 0x100 -Mouse Button 입력 기록시 커서 위치도 같이 기록
};

struct sEditBitOptions {
	DWORD START_STOP_KEY		: 1;
	DWORD SELECTED_ITEM			: 1;
};

class CAppIni: public CIni
{
	// 프로그램이 종료되기 전에 ini 파일에 보관되는 변수들.
	// 프로그램 내에서 전역 변수와 같이 사용된다.
public: 
	sMacorBitOptions macroOptions;
	sRecBitOptions   recOptions; 
	sEditBitOptions  editOptions; 

	int  hangulKeyboard;

	int  windowSX;
	int  windowSY;  
	int  debugSX;
	int  debugSY;
	int  statusSX;
	int  statusSY;  

	char macroFileName[MAX_PATH+1];
	char title[MAX_PATH+1];

	int  eventDelay;
	int  keyMacroRun;
	int  keyMacroRecord;
	int  keyMousePosCapture;
	int  keyMacroColorStart;
	int  keyMacroColorStop;

	int  recMouseDistance;
	int  recEventTimeInterval;

	// int runSelMacro;
	int sequenceStart;
	int macroSel;

public:
	CAppIni(const char *ini_path);
	virtual ~CAppIni();

	void LoadData();
	void SaveData();

private:
	void TransferAllData (bool bSave);
};

extern CAppIni g_ini;
