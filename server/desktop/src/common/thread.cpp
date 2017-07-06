#include "thread.h"

Thread::Thread()
{
	_thread = new OSThread();
	if (!_thread)
	{
		ThrowThreadExceptionWithCode("Can't allocate memory for OS thread!", GetLastError());
	}
}

Thread::~Thread()
{
	delete _thread;
}

int Thread::Start(void *threadFunc, void *threadFuncArgs, void *result)
{
	_thread->Init(threadFunc, threadFuncArgs, 0, t_flags::RUNIMMEDIATLY, t_secattr::ENULL);
	_thread->Start();
	_id = _thread->GetThreadID();
	return _id;
}

void* Thread::GetResult()
{
	return _result;
}

bool Thread::IsCompleted()
{
	return !_thread->CheckActive(_result);
}

int Thread::GetMaxThreadCount()
{
	return OSThread::GetMaxThreadCount();
}
