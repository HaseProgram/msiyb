#include "stdafx.h"
#include "../../stdafx.h"
#include "winthread.h"

WinThread::WinThread()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	this->maxThreadsCount = sysinfo.dwNumberOfProcessors;
}

int WinThread::GetMaxThreadCount()
{
	return this->maxThreadsCount;
}

void WinThread::Init(LPSECURITY_ATTRIBUTES threadSecurityAttributes, SIZE_T threadStackSize,
	LPTHREAD_START_ROUTINE threadFunc, LPVOID threadFuncArgs, DWORD threadFlags)
{
	this->threadSecurityAttributes = threadSecurityAttributes;
	this->threadStackSize = threadStackSize;
	this->threadFunc = threadFunc;
	this->threadFuncArgs = threadFuncArgs;
	this->threadFlags = threadFlags;
}

void WinThread::Start()
{
	if (CreateThread(this->threadSecurityAttributes, 
		this->threadStackSize, 
		this->threadFunc,
		this->threadFuncArgs,
		this->threadFlags,
		&this->threadID) == NULL)
	{
		throw Exception("Can not create thread"); // Temporary, will change after Sasha's exception push
	}
}

void WinThread::Interrupt()
{

}

long WinThread::GetThreadID()
{
	return this->threadID;
}
