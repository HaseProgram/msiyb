#include "stdafx.h"
#include "../../stdafx.h"
#include "winthread.h"

WinThread::WinThread()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	this->maxThreadsCount = sysinfo.dwNumberOfProcessors;
}

WinThread::~WinThread()
{
	this->freeHandles();
}

void WinThread::Init(int threadID, LPTHREAD_START_ROUTINE threadFunc, LPVOID threadFuncArgs, SIZE_T threadStackSize, DWORD threadFlags, LPSECURITY_ATTRIBUTES threadSecurityAttributes)
{
	if (this->ThreadIDExists(threadID))
	{
		ThrowThreadException("Can not init thread: thread with threadID already exists!");
	}
	THREADHANDLE *tHandle = new THREADHANDLE;
	tHandle->threadID = threadID;
	tHandle->threadSecurityAttributes = threadSecurityAttributes;
	tHandle->threadStackSize = threadStackSize;
	tHandle->threadFunc = threadFunc;
	tHandle->threadFuncArgs = threadFuncArgs;
	tHandle->threadFlags = threadFlags;
	this->hThreads.push_back(tHandle);
}

void WinThread::Init(int threadID, void *threadFunc, void *threadFuncArgs, size_t threadStackSize, t_flags threadFlags, t_secattr threadSecurityAttributes)
{
	if (this->ThreadIDExists(threadID))
	{
		ThrowThreadException("Can not init thread: thread with threadID already exists!");
	}
	THREADHANDLE *tHandle = new THREADHANDLE;
	this->GetThreadID = threadID;
	tHandle->threadFunc = (LPTHREAD_START_ROUTINE)threadFunc;
	tHandle->threadFuncArgs = threadFuncArgs;
	tHandle->threadStackSize = threadStackSize;
	switch (threadFlags)
	{
	case RUNIMMEDIATLY:
		tHandle->threadFlags = 0;
		break;
	case CREATESUSPENDED:
		tHandle->threadFlags = CREATE_SUSPENDED;
		break;
	case RESERVESTACK:
		tHandle->threadFlags = STACK_SIZE_PARAM_IS_A_RESERVATION;
		break;
	}
	switch (threadSecurityAttributes)
	{
	case ENULL:
		tHandle->threadSecurityAttributes = NULL;
		break;
	}
	this->hThreads.push_back(tHandle);
}

int WinThread::GetMaxThreadCount()
{
	return this->maxThreadsCount;
}

void WinThread::Start(int threadID)
{
	for (int i = 0; i < this->hThreads.size(); i++)
	{
		if (this->hThreads[i]->threadID == threadID)
		{
			this->hThreads[i]->hThread = CreateThread(this->hThreads[i]->threadSecurityAttributes,
				this->hThreads[i]->threadStackSize,
				this->hThreads[i]->threadFunc,
				this->hThreads[i]->threadFuncArgs,
				this->hThreads[i]->threadFlags,
				&this->hThreads[i]->threadSystemID);

			if (!this->hThreads[i]->hThread)
			{
				this->freeHandles(threadID);
				ThrowThreadExceptionWithCode("Could not create new thread!",GetLastError());
			}
			return;
		}
	}
	ThrowThreadException("Can not start thread: threadID not found!");
}

long WinThread::GetThreadID(int threadID)
{
	for (int i = 0; i < this->hThreads.size(); i++)
	{
		if (this->hThreads[i]->threadID == threadID)
		{
			return this->hThreads[i]->threadSystemID;
		}
	}
	ThrowThreadException("Can not get thread system ID: threadID not found!");
}

bool WinThread::CheckActive(int threadID, void *result)
{
	for (int i = 0; i < this->hThreads.size(); i++)
	{
		if (this->hThreads[i]->threadID == threadID)
		{
			this->hThreads[i]->status = GetExitCodeThread(this->hThreads[i]->hThread, this->hThreads[i]->exitCode);
			result = this->hThreads[i]->exitCode;
			if (this->hThreads[i]->status == 0)
			{
				ThrowThreadExceptionWithCode("Can not check thread status!", GetLastError());
			}
			return (this->hThreads[i]->status == STILL_ACTIVE);
		}
	}
	ThrowThreadException("Can not check thread status: threadID not found!");
}

bool WinThread::ThreadIDExists(int threadID)
{
	for (int i = 0; i < this->hThreads.size(); i++)
	{
		if (this->hThreads[i]->threadID == threadID)
		{
			return true;
		}
	}
	return false;
}

void WinThread::freeHandles(int handleID)
{
	for (int i = 0; i < this->hThreads.size(); i++)
	{
		if (handleID < 0 || this->hThreads[i]->threadID == handleID)
		{
			delete this->hThreads[i];
		}
	}
}
