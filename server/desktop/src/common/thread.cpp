#include "stdafx.h"
#include "thread.h"

Thread::Thread()
{
	this->thread = new OSThread;
	if (!this->thread)
	{
		ThrowException("Can't allocate memory!");
	}
	this->threadCount = 0;
	if (this->threadLastGroupID < MAX_INT)
	{
		this->threadLastGroupID++;
	}
	else
	{
		this->threadLastGroupID = 0;
	}
	this->threadGroupID = this->threadLastGroupID;
}

Thread::~Thread()
{
	this->CheckListCompleted();
}

int Thread::Start(void *threadFunc, void *threadFuncArgs, void *result)
{
	if (this->ThreadList.size() >= this->thread->GetMaxThreadCount())
	{
		this->CheckListCompleted();
		if (this->ThreadList.size() >= this->thread->GetMaxThreadCount())
		{
			ThrowThreadException("Can't start new thread. Amount of launched threads is max!");
		}
	}

	ThreadInfo* thrInfo = new ThreadInfo;
	if (!thrInfo)
	{
		ThrowThreadExceptionWithCode("Can't allocate memory for thread info structure!", GetLastError());
	}

	thrInfo->threadID = this->threadCount;
	if (this->threadCount < MAX_INT)
	{
		this->threadCount++;
	}
	else
	{
		this->threadCount = 0;
	}
	thrInfo->result = result;
	thrInfo->thr = this;
	this->ThreadList.push_back(thrInfo);

	this->thread->Init(thrInfo->threadID, threadFunc, threadFuncArgs, 0, t_flags::RUNIMMEDIATLY, t_secattr::ENULL);
	this->thread->Start(thrInfo->threadID);

	return thrInfo->threadID;
}

bool Thread::IsCompleted(int threadID)
{
	for (size_t i = 0; i < this->ThreadList.size(); i++)
	{
		if (!this->ThreadList[i])
		{
			this->ThreadList.erase(this->ThreadList.begin() + i);
		}
		if ((this->ThreadList[i]->thr->threadGroupID == this->threadGroupID) && (this->ThreadList[i]->threadID = threadID))
		{
			return (!this->thread->CheckActive(threadID, this->ThreadList[i]->result));
		}
	}
}

void Thread::CheckListCompleted()
{
	for (size_t i = 0; i < Thread::ThreadList.size(); i++)
	{
		if (!Thread::ThreadList[i])
		{
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
		}
		if (Thread::ThreadList[i]->thr->IsCompleted(Thread::ThreadList[i]->threadID))
		{
			delete Thread::ThreadList[i];
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
		}
	}
}

void Thread::CheckThreadCompleted(int groupID, int threadID)
{
	for (size_t i = 0; i < Thread::ThreadList.size(); i++)
	{
		if (!Thread::ThreadList[i])
		{
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
			return;
		}
		if (Thread::ThreadList[i]->threadID == threadID && Thread::ThreadList[i]->thr->IsCompleted(Thread::ThreadList[i]->threadID))
		{
			delete Thread::ThreadList[i];
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
			return;
		}
	}
}
