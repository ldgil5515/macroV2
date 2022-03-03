#pragma once

class CDeviceLock  
{
public:
	CDeviceLock()	{ InitializeCriticalSection(&_cs);	}
	~CDeviceLock()	{ DeleteCriticalSection(&_cs);		}

	void Lock()		{ EnterCriticalSection (&_cs);		}
	void Unlock()	{ LeaveCriticalSection (&_cs);		}

private:
	CRITICAL_SECTION _cs;
};


class CScopedLock {
public:
	CScopedLock::CScopedLock (CDeviceLock *device): _device(device) { _device->Lock (); }
	CScopedLock::~CScopedLock () { _device->Unlock (); }

private: 
	CDeviceLock *_device;
};

#define LOCK_BLOCK(x)	CScopedLock _lock_block_(x)