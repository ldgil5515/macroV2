#include "stdafx.h"
#include <Mmsystem.h>
#include <assert.h>
#include "MmTimer.h"

int CMmTimer::_mmtCount = 0;

CMmTimer::CMmTimer () :
	_timerRes (0), _timerId (-1)
{
	TIMECAPS tc;
	const int TARGET_RESOLUTION	 = 1;	// 1-millisecond target resolution

	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	_timerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);

	if (_mmtCount==0) {
		timeBeginPeriod(_timerRes); 
	}
	_mmtCount++;
}

CMmTimer::~CMmTimer ()
{
	_mmtCount--;
	Stop ();

	if (_mmtCount==0 && 0<_timerRes) {
		timeEndPeriod (_timerRes);
	}
}

static void CALLBACK OneShotTimer(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{
	CMmTimer *pThis = (CMmTimer *)dwUser;
	pThis->OnTimer (_timerId, msg);
} 

void CMmTimer::Start (UINT executionPeriod)
{
	assert (_timerId == (DWORD)-1);
	_period = executionPeriod;
	_timerId = timeSetEvent (executionPeriod, _timerRes, OneShotTimer, (DWORD)this, TIME_PERIODIC);
}

void CMmTimer::Stop ()
{
	if (_timerId != (DWORD)-1) {
		timeKillEvent (_timerId);  // cancel the event
		_timerId = -1;
	}
}

DWORD CMmTimer::GetTime ()
{
	return timeGetTime(); 
}

void CMmTimer::Sleep (DWORD ms)
{
	::Sleep (ms);
}

