#pragma once
#include <vector>
#include "common.h"

using namespace std;


#define MOUSEEVENTF_SCREEN_INDEPENDENT_POS         0x0200


struct sKeybd {
	// Hi  byte - Scan code
	// Low byte - Virtual key code
	long scan_vk_code;
	// 0x01 - 누르기
	// 0x02 - 떼기
	// 0x03 - 누르고 떼기
	// 0x10 - 한글 입력 상태
	// 0x20 - Extended key가 눌림
	ulong flags;

	void Step (bool vk_pressed[256]);
};

struct sMouse {
	// 마우스 위치(절대위치 or 상대위치)
	long x, y;
	// Low WORD:
	//	 MOUSEEVENTF_MOVE        0x0001 // mouse move 
	//	 MOUSEEVENTF_LEFTDOWN    0x0002 // left button down 
	//	 MOUSEEVENTF_LEFTUP      0x0004 // left button up 
	//	 MOUSEEVENTF_RIGHTDOWN   0x0008 // right button down 
	//	 MOUSEEVENTF_RIGHTUP     0x0010 // right button up 
	//	 MOUSEEVENTF_MIDDLEDOWN  0x0020 // middle button down 
	//	 MOUSEEVENTF_MIDDLEUP    0x0040 // middle button up 
	//	 MOUSEEVENTF_XDOWN       0x0080 // x button down (XBUTTON 1,2는 High byte에서 구분)
	//	 MOUSEEVENTF_XUP         0x0100 // x button down 
	//	 MOUSEEVENTF_WHEEL       0x0800 // wheel button rolled 
	//   MOUSEEVENTF_ABSOLUTE    0x8000 // absolute move 
	//	 MOUSEEVENTF_SCREEN_INDEPENDENT_POS 0x0200
	// High WORD:
	//	 XBUTTON1, XBUTTON2, 휠의 굴린 양
	ulong flags;

	void Step (bool vk_pressed[256]);
};

struct sDelay {
	// 시간 지연 값 (단위: msec)
	long delay;		// 지연시간, 시각, 키값, RGB색
	// 0x00 - 시간지연(delay) 설정
	// 0x01 - 지연시각(time) 설정
	// 0x02 - 지연시각을 단위로 설정
	// 0x04 - 지연시각을 초/분 단위로 설정
	// 0x06 - 지연시각을 초/분/시간 단위로 설정
	// 0x10 - 키입력 대기
	// 0x20 - 화면 색 변환 대기
	ulong flags;	// 
	ulong xy;		// 랜덤 지연 시간(0~10), 화면 색인식에서 화면 위치

	bool StepDelay (long &time_remain, int dt);
	bool StepTime  (long &time_remain, int dt);
	bool StepKey   (long &wait_key);
	bool StepColor (long &wait_color, long &wait_xy);

private:
	int GetTimeDelay (int flag, int delay);
	int GetRandomDelay (int delay, int random);
};

struct sString {
	long length;
	// 0x01 - 랜덤 문자 생성 사용
	// 0x02 - 연속 숫자 생성 사용
	// 0x10 - 문자열을 주석(comment)으로 사용
	ulong flags;
	// 주석 or 문자열에 대한 포인터
	char *string;

	bool Step (long &str_index);
	void KeybdEventNum (int count);

private:
	inline BYTE GetUpperRand () { return 'A' + rand()%26; }
	inline BYTE GetLowerRand () { return 'a' + rand()%26; }
	inline BYTE GetNumberRand() { return '0' + rand()%10; }
	
	void KeybdEvent (BYTE ch);
	void KeybdEventHangul (WORD ch);
	BYTE GetVK (uchar ch, bool *shift_key);
};

struct sMCall {
	char *name;
	// 0x00 - 호출되는 매크로 순차적 실행
	// 0x01 - 호출되는 매크로 병렬 실행
	// 0x02 - 실행 중인 매크로 중단
	ulong flags;

	bool Step (long &str_index);
};

struct sLock {
	// 0x00 - 독점 실행구간에서 나옴
	// 0x01 - 독점 실행구간으로 들어감
	ulong flags;

	bool Step (bool &cs_enter);
};

struct sLoop {
	// 0x00 - 반복구간으로 들어감
	// 0x01 - 반복 숫자를 키보드 이벤트로 출력
	// 0x02 - 반복 숫자를 마우스 x 위치로 설정
	// 0x03 - 반복 숫자를 마우스 y 위치로 설정
	// 0x07 - 반복구간에서 나옴
	ushort flags;
	short  step;	// 증분
	long   start;	// 반복구간 시작 숫자
	long   stop;	// 반복구간 종료 숫자

	bool Step (long &index, vector<int> &stack);
};

enum eMacroItem { MI_NONE = 0, MI_KEYBD, MI_MOUSE, MI_DELAY, MI_STRING, MI_MCALL, MI_LOCK, MI_LOOP };

struct sMacroItem {
	eMacroItem type;

	union {
		sKeybd keybd;
		sMouse mouse;
		sDelay delay;
		sString string;
		sMCall mcall;
		sLock  lock;
		sLoop  loop;
		long   data[3];
	};

	sMacroItem () : type(MI_NONE) 
	{ 
		data[0] = data[1] = data[2] = 0;
	}
	
	~sMacroItem () 
	{
		Delete ();
	}

	sMacroItem (const sMacroItem &mi) 
	{
		Copy (mi);
	}
	
	sMacroItem &operator = (const sMacroItem &mi) 
	{
		if (this != &mi) {
			Delete ();
			Copy (mi);
		}
		return *this;
	}

private:
	void Delete (); 
	void Copy (const sMacroItem &mi); 
};
