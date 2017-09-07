#include "winsemaphore.h"

using MSIYBCore::WinSemaphore;

WinSemaphore::WinSemaphore(long initialCount, long maxCount, unsigned long timeout)
{
	_timeout = timeout;
	Init(t_secattr::ENULL, initialCount, maxCount, timeout);
	Create();
}

WinSemaphore::WinSemaphore(const WinSemaphore &other)
{
	_hSemaphore = other._hSemaphore;
	_semaphoreAttr = other._semaphoreAttr;
	_initialCount = other._initialCount;
	_maxCount = other._maxCount;
	_timeout = other._timeout;
}

WinSemaphore::WinSemaphore(WinSemaphore && other)
{
	_hSemaphore = other._hSemaphore;
	_semaphoreAttr = other._semaphoreAttr;
	_initialCount = other._initialCount;
	_maxCount = other._maxCount;
	_timeout = other._timeout;
}

WinSemaphore::WinSemaphore(LPSECURITY_ATTRIBUTES semaphoreAttr, long initialCount, long maxCount, unsigned long timeout)
{
	Init(semaphoreAttr, initialCount, maxCount, timeout);
	Create();
}

WinSemaphore::WinSemaphore(t_secattr semaphoreAttr, long initialCount, long maxCount, unsigned long timeout)
{
	Init(semaphoreAttr, initialCount, maxCount, timeout);
	Create();
}

WinSemaphore::~WinSemaphore()
{
}

WinSemaphore& WinSemaphore::operator=(const WinSemaphore & other)
{
	_hSemaphore = other._hSemaphore;
	_semaphoreAttr = other._semaphoreAttr;
	_initialCount = other._initialCount;
	_maxCount = other._maxCount;
	_timeout = other._timeout;
	return *this;
}

bool WinSemaphore::Lock()
{
	return WaitEventResponse(WaitForSingleObject(_hSemaphore, _timeout));
}

bool WinSemaphore::LockShared()
{
	return false;
}

bool WinSemaphore::TryLock()
{
	return WaitEventResponse(WaitForSingleObject(_hSemaphore, 0));
}

bool WinSemaphore::TryLockShared()
{
	return false;
}

bool WinSemaphore::Unlock()
{
	if (ReleaseSemaphore(_hSemaphore, 1, NULL) != 0)
	{
		ThrowLockerExceptionWithCode("Can't unlock semaphore.", GetLastError());
	}
	return true;
}

bool WinSemaphore::WaitEventResponse(unsigned long event)
{
	switch (event)
	{
	case WAIT_FAILED:
		ThrowLockerExceptionWithCode("Can't lock semaphore.", GetLastError());
		break;
	case WAIT_OBJECT_0:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void WinSemaphore::Init(t_secattr semaphoreAttr, long initialCount, long maxCount, unsigned long timeout)
{
	_timeout = timeout;
	LPSECURITY_ATTRIBUTES securityAttr = NULL;
	switch (semaphoreAttr)
	{
	case ENULL:
		securityAttr = NULL;
		break;
	}
	Init(semaphoreAttr, initialCount, maxCount);
}

void WinSemaphore::Init(LPSECURITY_ATTRIBUTES semaphoreAttr, long initialCount, long maxCount, unsigned long timeout)
{
	_timeout = timeout;
	_semaphoreAttr = semaphoreAttr;
	_initialCount = initialCount;
	if (maxCount == 0 || maxCount < initialCount)
	{
		ThrowLockerException("Wrong semaphore initialisation parameters. MaxCount must be greeder then zero or initialCount");
	}
	_maxCount = maxCount;
}

void WinSemaphore::Create()
{
	_hSemaphore = CreateSemaphore(
		_semaphoreAttr,
		_initialCount,
		_maxCount,
		NULL
	);
}

