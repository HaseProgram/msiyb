#include "winsrwlock.h"

WinSRWLock::WinSRWLock()
{
	_shared = -1;
	_ready = false;
	_inUse = FALSE;
}

WinSRWLock::WinSRWLock(const WinSRWLock &other)
{
	_shared = other._shared;
	_srw = other._srw;
	_ready = other._ready;
	_inUse = other._inUse;
}

WinSRWLock::WinSRWLock(WinSRWLock &&other)
{
	_shared = other._shared;
	_srw = other._srw;
	_ready = other._ready;
	_inUse = other._inUse;
}

WinSRWLock & WinSRWLock::operator=(const WinSRWLock &other)
{
	_shared = other._shared;
	_srw = other._srw;
	_ready = other._ready;
	_inUse = other._inUse;
}

WinSRWLock::~WinSRWLock()
{
	_ready = false;
}

void WinSRWLock::Init()
{
	if (!_ready)
	{
		InitializeSRWLock(&_srw);
		_ready = true;
	}
}

bool WinSRWLock::Lock()
{
	Init();
	AcquireSRWLockExclusive(&_srw);
	return true;
}

bool WinSRWLock::TryLock()
{
	return false;
}
