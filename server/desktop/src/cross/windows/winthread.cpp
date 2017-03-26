#include "stdafx.h"
#include "../../stdafx.h"
#include "winthread.h"

WinThread::WinThread()
{

}

WinThread::~WinThread()
{
	
}

void WinThread::Init(LPTHREAD_START_ROUTINE threadFunc, LPVOID threadFuncArgs, SIZE_T threadStackSize, DWORD threadFlags, LPSECURITY_ATTRIBUTES threadSecurityAttributes)
{
	this->threadSecurityAttributes = threadSecurityAttributes;
	this->threadStackSize = threadStackSize;
	this->threadFunc = threadFunc;
	this->threadFuncArgs = threadFuncArgs;
	this->threadFlags = threadFlags;
}

void WinThread::Init(void *threadFunc, void *threadFuncArgs, size_t threadStackSize, t_flags threadFlags, t_secattr threadSecurityAttributes)
{
	this->threadFunc = (LPTHREAD_START_ROUTINE)threadFunc;
	this->threadFuncArgs = threadFuncArgs;
	this->threadStackSize = threadStackSize;
	switch (threadFlags)
	{
	case RUNIMMEDIATLY:
		this->threadFlags = 0;
		break;
	case CREATESUSPENDED:
		this->threadFlags = CREATE_SUSPENDED;
		break;
	case RESERVESTACK:
		this->threadFlags = STACK_SIZE_PARAM_IS_A_RESERVATION;
		break;
	}
	switch (threadSecurityAttributes)
	{
	case ENULL:
		this->threadSecurityAttributes = NULL;
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
	this->hThread = CreateThread(this->threadSecurityAttributes,
		this->threadStackSize,
		this->threadFunc,
		this->threadFuncArgs,
		this->threadFlags,
		&this->threadSystemID);

	if (!this->hThread)
	{
		ThrowThreadExceptionWithCode("Could not create new thread!", GetLastError());
	}
}

long WinThread::GetThreadID()
{
	return this->threadSystemID;
}

bool WinThread::CheckActive(void *result)
{
	this->status = GetExitCodeThread(this->hThread, this->exitCode);
	result = this->exitCode;
	if (this->status == 0)
	{
		ThrowThreadExceptionWithCode("Can not check thread status!", GetLastError());
	}
	return (this->status == STILL_ACTIVE);
}
