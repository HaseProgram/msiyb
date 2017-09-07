#include "winthread.h"

WinThread::WinThread()
{

}

WinThread::~WinThread()
{

}


void WinThread::Init(LPTHREAD_START_ROUTINE threadFunc, LPVOID threadFuncArgs, SIZE_T threadStackSize, DWORD threadFlags, LPSECURITY_ATTRIBUTES threadSecurityAttributes)
{
	_threadSecurityAttributes = threadSecurityAttributes;
	_threadStackSize = threadStackSize;
	_threadFunc = threadFunc;
	_threadFuncArgs = threadFuncArgs;
	_threadFlags = threadFlags;
}

void WinThread::Init(void *threadFunc, void *threadFuncArgs, size_t threadStackSize, t_flags threadFlags, t_secattr threadSecurityAttributes)
{
	_threadFunc = (LPTHREAD_START_ROUTINE)threadFunc;
	_threadFuncArgs = threadFuncArgs;
	_threadStackSize = threadStackSize;
	switch (threadFlags)
	{
	case RUNIMMEDIATLY:
		_threadFlags = 0;
		break;
	case CREATESUSPENDED:
		_threadFlags = CREATE_SUSPENDED;
		break;
	case RESERVESTACK:
		_threadFlags = STACK_SIZE_PARAM_IS_A_RESERVATION;
		break;
	}
	switch (threadSecurityAttributes)
	{
	case ENULL:
		_threadSecurityAttributes = NULL;
		break;
	}
}

int WinThread::GetMaxThreadCount()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}

void WinThread::Start()
{
	_hThread = CreateThread(_threadSecurityAttributes,
		_threadStackSize,
		_threadFunc,
		_threadFuncArgs,
		_threadFlags,
		&_threadSystemID);

	if (!_hThread)
	{
		ThrowThreadExceptionWithCode("Could not create new thread!", GetLastError());
	}
}

long WinThread::GetThreadID()
{
	return _threadSystemID;
}

bool WinThread::CheckActive(void *result)
{
	this->status = GetExitCodeThread(_hThread, _exitCode);
	result = _exitCode;
	if (this->status == 0)
	{
		ThrowThreadExceptionWithCode("Can not check thread status!", GetLastError());
	}
	return (this->status == STILL_ACTIVE);
}
