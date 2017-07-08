#include "winmutex.h"

WinMutex::WinMutex()
{

}

WinMutex::WinMutex(t_secattr mutexAttr, bool initialOwner, char* name)
{
	Init(mutexAttr, initialOwner, name);
	Create();
}

WinMutex::WinMutex(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, LPCTSTR name)
{
	Init(mutexAttr, initialOwner, name);
	Create();
}

WinMutex::~WinMutex()
{
}

void WinMutex::Init(t_secattr mutexAttr, bool initialOwner, char* name, unsigned long timeout)
{
	_timeout = timeout;
	LPSECURITY_ATTRIBUTES securityAttr;
	switch (mutexAttr)
	{
	case ENULL:
		securityAttr = NULL;
		break;
	}
	TCHAR* tName;
	ConvertCharToTCHAR(name, tName);
	Init(securityAttr, initialOwner, tName);
}

void WinMutex::Init(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, LPCTSTR name, unsigned long timeout)
{
	_timeout = timeout;
	_mutexAttr = mutexAttr;
	_initialOwner = initialOwner;
	_name = name;
}

void WinMutex::Create()
{
	_hMutex = CreateMutex(
		_mutexAttr,
		_initialOwner,
		_name
	);
}

bool WinMutex::Lock()
{
	unsigned long event = WaitForSingleObject(_hMutex, _timeout);
	switch(event)
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

bool WinMutex::Unlock()
{
	if (ReleaseMutex(_hMutex) != 0)
	{
		ThrowLockerExceptionWithCode("Can't unlock mutex.", GetLastError());
	}
	return true;
}
