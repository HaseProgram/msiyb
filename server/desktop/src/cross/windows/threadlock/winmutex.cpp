#include "winmutex.h"

using MSIYBCore::WinMutex;

WinMutex::WinMutex(unsigned long timeout)
{
	_timeout = timeout;
	Init(t_secattr::ENULL, FALSE, timeout);
	Create();
}

WinMutex::WinMutex(const WinMutex& other)
{
	_hMutex = other._hMutex;
	_mutexAttr = other._mutexAttr;
	_initialOwner = other._initialOwner;
	_timeout = other._timeout;
}

WinMutex::WinMutex(WinMutex&& other)
{
	_hMutex = other._hMutex;
	_mutexAttr = other._mutexAttr;
	_initialOwner = other._initialOwner;
	_timeout = other._timeout;
}

WinMutex::WinMutex(t_secattr mutexAttr, bool initialOwner, unsigned long timeout)
{
	Init(mutexAttr, initialOwner, timeout);
	Create();
}

WinMutex::WinMutex(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, unsigned long timeout)
{
	Init(mutexAttr, initialOwner, timeout);
	Create();
}

WinMutex::~WinMutex()
{
}

WinMutex & WinMutex::operator=(const WinMutex & other)
{
	_hMutex = other._hMutex;
	_mutexAttr = other._mutexAttr;
	_initialOwner = other._initialOwner;
	_timeout = other._timeout;
	return *this;
}

void WinMutex::Init(t_secattr mutexAttr, bool initialOwner, unsigned long timeout)
{
	_timeout = timeout;
	LPSECURITY_ATTRIBUTES securityAttr = NULL;
	switch (mutexAttr)
	{
	case ENULL:
		securityAttr = NULL;
		break;
	}
	Init(securityAttr, initialOwner);
}

void WinMutex::Init(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, unsigned long timeout)
{
	_timeout = timeout;
	_mutexAttr = mutexAttr;
	_initialOwner = initialOwner;
}

void WinMutex::Create()
{
	_hMutex = CreateMutex(
		_mutexAttr,
		_initialOwner,
		NULL
	);
}

bool WinMutex::Lock()
{
	return WaitEventResponse(WaitForSingleObject(_hMutex, _timeout));
}

bool WinMutex::LockShared()
{
	return false;
}

bool WinMutex::TryLock()
{
	return WaitEventResponse(WaitForSingleObject(_hMutex, 0));
}

bool WinMutex::TryLockShared()
{
	return false;
}

bool WinMutex::Unlock()
{
	if (ReleaseMutex(_hMutex) != 0)
	{
		ThrowLockerExceptionWithCode("Can't unlock mutex.", GetLastError());
	}
	return true;
}

bool WinMutex::WaitEventResponse(unsigned long event)
{
	switch (event)
	{
	case WAIT_FAILED:
		ThrowLockerExceptionWithCode("Can't lock mutex.", GetLastError());
		break;
	case WAIT_OBJECT_0:
		return true;
		break;
	default:
		return false;
		break;
	}
}
