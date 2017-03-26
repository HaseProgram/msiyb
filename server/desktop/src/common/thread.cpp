#include "stdafx.h"
#include "thread.h"

Thread::Thread()
{
	
}

Thread::~Thread()
{
	this->CheckListCompleted();
}

int Thread::Start(void *threadFunc, void *threadFuncArgs, void *result)
{
	ThreadInfo* thrInfo = new ThreadInfo;
	if (!thrInfo)
	{
		ThrowThreadExceptionWithCode("Can't allocate memory for thread info structure!", GetLastError());
	}

	if (this->threadList.size() >= OSThread::GetMaxThreadCount())
	{
		this->CheckListCompleted();
		if (this->threadList.size() >= OSThread::GetMaxThreadCount())
		{
			ThrowThreadException("Can't start new thread. Amount of launched threads is max!");
		}
	}

	this->threadList.push_back(thrInfo);

	thrInfo->threadID = this->lastThreadID++;

	thrInfo->threadPoolPtr = new OSThread;
	if (!thrInfo->threadPoolPtr)
	{
		ThrowException("Can't allocate memory!");
	}
	thrInfo->result = result;
	thrInfo->thr = this;

	thrInfo->threadPoolPtr->Init(threadFunc, threadFuncArgs, 0, t_flags::RUNIMMEDIATLY, t_secattr::ENULL);
	thrInfo->threadPoolPtr->Start();

	return thrInfo->threadID;
}

bool Thread::IsCompleted(int threadID)
{
	for (size_t i = 0; i < this->threadList.size(); i++)
	{
		if (!this->threadList[i])
		{
			this->threadList.erase(this->threadList.begin() + i);
		}
		if ((this->threadList[i]->threadID = threadID))
		{
			return (!this->threadList[i]->threadPoolPtr->CheckActive(this->threadList[i]->result));
		}
	}
}

void Thread::CheckListCompleted()
{
	for (size_t i = 0; i < Thread::threadList.size(); i++)
	{
		if (!Thread::threadList[i])
		{
			Thread::threadList.erase(Thread::threadList.begin() + i);
		}
		if (Thread::threadList[i]->thr->IsCompleted(Thread::threadList[i]->threadID))
		{
			delete Thread::threadList[i]->threadPoolPtr;
			delete Thread::threadList[i]->thr;
			delete Thread::threadList[i];
			Thread::threadList.erase(Thread::threadList.begin() + i);
		}
	}
}

void Thread::CheckThreadCompleted(int threadID)
{
	for (size_t i = 0; i < Thread::threadList.size(); i++)
	{
		if (!Thread::threadList[i])
		{
			Thread::threadList.erase(Thread::threadList.begin() + i);
			return;
		}
		if (Thread::threadList[i]->threadID == threadID && Thread::threadList[i]->thr->IsCompleted(Thread::threadList[i]->threadID))
		{
			delete Thread::threadList[i]->threadPoolPtr;
			delete Thread::threadList[i]->thr;
			delete Thread::threadList[i];
			Thread::threadList.erase(Thread::threadList.begin() + i);
			return;
		}
	}
}
