#include "winsrwlock.h"
using MSIYBCore::WinSRWLock;

WinSRWLock::WinSRWLock()
{
	_shared = -1;
	_ready = false;
	TCHAR* moduleName;
	ConvertCharToTCHAR("kernel32", moduleName);
	HMODULE module = GetModuleHandle(moduleName);
	_tryExPtr = (TrySrwLock)GetProcAddress(module, "TryAcquireSRWLockExclusive");
	_tryShPtr = (TrySrwLock)GetProcAddress(module, "TryAcquireSRWLockShared");
}

WinSRWLock::WinSRWLock(const WinSRWLock &other)
{
	_shared = other._shared;
	_srw = other._srw;
	_ready = other._ready;
	_tryExPtr = other._tryExPtr;
	_tryShPtr = other._tryShPtr;
}

WinSRWLock::WinSRWLock(WinSRWLock &&other)
{
	_shared = other._shared;
	_srw = other._srw;
	_ready = other._ready;
	_tryExPtr = other._tryExPtr;
	_tryShPtr = other._tryShPtr;
}

WinSRWLock & WinSRWLock::operator=(const WinSRWLock &other)
{
	_shared = other._shared;
	_srw = other._srw;
	_ready = other._ready;
	_tryExPtr = other._tryExPtr;
	_tryShPtr = other._tryShPtr;
	return *this;
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
	_shared = 0;
	return true;
}

bool WinSRWLock::LockShared()
{
	Init();
	AcquireSRWLockShared(&_srw);
	_shared = 1;
	return true;
}

bool WinSRWLock::TryLock()
{
	if (_tryExPtr != NULL)
	{
		bool res = _tryExPtr(&_srw);
		if (res)
		{
			_shared = 0;
		}
		return res;
	}
	else
	{
		return false; // Maybe try use Interlocked functions...
	}
}

bool WinSRWLock::TryLockShared()
{
	if (_tryShPtr != NULL)
	{
		bool res = _tryShPtr(&_srw);
		if (res)
		{
			_shared = 1;
		}
		return res;
	}
	else
	{
		return false; // Maybe try use Interlocked functions...
	}
}

bool WinSRWLock::Unlock()
{
	if (_shared == -1)
	{
		return false;
	}
	if (_shared == 0)
	{
		ReleaseSRWLockExclusive(&_srw);
	}
	if (_shared == 1)
	{
		ReleaseSRWLockShared(&_srw);
	}
	return true;
}
