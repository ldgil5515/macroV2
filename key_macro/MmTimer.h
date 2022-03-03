#pragma once 
#pragma comment (lib,"winmm.lib")

class CMmTimer {
public:
	CMmTimer ();
	virtual ~CMmTimer ();

	virtual void OnTimer (UINT timerId, UINT msg) { }

	void Start (UINT executionPeriod);
	void Stop ();
	bool IsRunning () { return _timerId != -1; }

	static DWORD GetTime ();
	static void Sleep (DWORD ms);

public:
	UINT _period;	// ms

private:
	static int _mmtCount;
	UINT       _timerRes;
	DWORD      _timerId;
};
