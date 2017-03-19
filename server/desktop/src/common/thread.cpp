#include "stdafx.h"
#include "thread.h"

Thread::Thread()
{
	this->thread = new OSThread;
	if (!this->thread)
	{
		ThrowException("Can't allocate memory!");
	}
	this->runned = false;
	this->ThreadList.push_back(this);
}

Thread::~Thread()
{
	delete this->thread;
	this->activeThreadsCount--;
}

int Thread::Start(void *threadFunc, void *threadFuncArgs, void *result)
{
	if (this->runned)
	{
		return -1;
	}

	if (this->threadLID < MAX_INT)
	{
		this->threadLID++;
	}
	else
	{
		this->threadLID = 0;
	}
	this->threadID = this->threadLID;
	if (this->activeThreadsCount >= this->thread->GetMaxThreadCount())
	{
		this->CheckListCompleted();
		if (this->activeThreadsCount >= this->thread->GetMaxThreadCount())
		{
			return -1;
		}
	}

	this->thread->Init(threadFunc, threadFuncArgs, 0, t_flags::RUNIMMEDIATLY, t_secattr::ENULL);
	this->result = result;
	this->thread->Start();
	this->runned = true;
	this->activeThreadsCount++;

	return this->threadLID;
}

bool Thread::CheckCompleted()
{
	return (this->runned && !this->thread->CheckActive(this->result));
}

bool Thread::CheckActive()
{
	return (this->runned && this->thread->CheckActive(this->result));
}

void Thread::CheckListCompleted()
{
	for (size_t i = 0; i < Thread::ThreadList.size(); i++)
	{
		if (!Thread::ThreadList[i])
		{
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
		}
		if (Thread::ThreadList[i]->CheckCompleted())
		{
			delete Thread::ThreadList[i];
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
		}
	}
}

void Thread::CheckThreadCompleted(int ID)
{
	for (size_t i = 0; i < Thread::ThreadList.size(); i++)
	{
		if (!Thread::ThreadList[i])
		{
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
			return;
		}
		if (Thread::ThreadList[i]->threadID == ID && Thread::ThreadList[i]->CheckCompleted())
		{
			delete Thread::ThreadList[i];
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
			return;
		}
	}
}
