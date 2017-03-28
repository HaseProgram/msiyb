#include "stdafx.h"
#include "thread.h"

Thread::Thread()
{
	
}

Thread::~Thread()
{
	this->WaitToComplete();
	this->CheckListCompleted();
}

int Thread::Start(void *threadFunc, void *threadFuncArgs, void *result)
{
	ThreadData* thrData = new ThreadData;
	if (!thrData)
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

	this->threadList.push_back(thrData);
	this->threadPool.push_back(thrData);

	thrData->threadID = this->lastThreadID++;

	thrData->threadOSPtr = new OSThread;
	if (!thrData->threadOSPtr)
	{
		ThrowException("Can't allocate memory!");
	}
	thrData->result = result;
	thrData->thr = this;

	thrData->threadOSPtr->Init(threadFunc, threadFuncArgs, 0, t_flags::RUNIMMEDIATLY, t_secattr::ENULL);
	thrData->threadOSPtr->Start();

	return thrData->threadID;
}

void Thread::WaitToComplete()
{
	bool existsRunning = true;
	while (existsRunning)
	{
		existsRunning = false;
		for (int i = 0; i < this->threadPool.size(); i++)
		{
			existsRunning = this->threadPool[i]->threadOSPtr->CheckActive(this->threadPool[i]->result);
			if (existsRunning)
			{
				break;
			}
			delete this->threadPool[i]->threadOSPtr;
			delete this->threadPool[i]->thr;
			delete this->threadPool[i];
			this->threadPool.erase(this->threadPool.begin() + i);
		}
	}
	if (this->threadPool.size() > 0)
	{
		ThrowThreadException("There left threads after function WaitToComplete!");
	}
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
			return (!this->threadList[i]->threadOSPtr->CheckActive(this->threadList[i]->result));
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
			delete Thread::threadList[i]->threadOSPtr;
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
			delete Thread::threadList[i]->threadOSPtr;
			delete Thread::threadList[i]->thr;
			delete Thread::threadList[i];
			Thread::threadList.erase(Thread::threadList.begin() + i);
			return;
		}
	}
}
