#include "stdafx.h"
#include "thread.h"

Thread::Thread()
{
	this->thread = new OSThread;
	this->runned = false;
	this->ThreadList.push_back(this);
}

Thread::~Thread()
{
	delete this->thread;
	this->activeThreadsCount--;
}

bool Thread::Start(void * threadFunc, void * threadFuncArgs)
{
	if (this->activeThreadsCount >= this->thread->GetMaxThreadCount())
	{
		this->CheckListCompleted();
		if (this->activeThreadsCount >= this->thread->GetMaxThreadCount())
		{
			return false;
		}
	}

	this->thread->Init(threadFunc, threadFuncArgs, 0, t_flags::RUNIMMEDIATLY, t_secattr::ENULL);
	this->thread->Start();
	this->runned = true;
	this->activeThreadsCount++;

	return true;
}

bool Thread::CheckCompleted()
{
	return (this->runned && !this->thread->CheckActive());
}

bool Thread::CheckActive()
{
	return (this->runned && this->thread->CheckActive());
}

void Thread::CheckListCompleted()
{
	for (size_t i = 0; i < Thread::ThreadList.size(); i++)
	{
		if (!Thread::ThreadList[i]->CheckCompleted())
		{
			delete Thread::ThreadList[i];
			Thread::ThreadList.erase(Thread::ThreadList.begin() + i);
		}
	}
}
