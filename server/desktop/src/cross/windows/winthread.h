#pragma once
#include "../ithread.h"

class WinThread : public IThread
{
public:

	WinThread();

	void WinThread::Init(LPSECURITY_ATTRIBUTES threadSecurityAttributes, SIZE_T threadStackSize,
		LPTHREAD_START_ROUTINE threadFunc, LPVOID threadFuncArgs, DWORD threadFlags);

	virtual int GetMaxThreadCount() override;
	virtual void Start() override;
	virtual void Interrupt() override;
	virtual long GetThreadID() override;

private:
	LPSECURITY_ATTRIBUTES threadSecurityAttributes;
	SIZE_T threadStackSize;
	LPTHREAD_START_ROUTINE threadFunc;
	LPVOID threadFuncArgs;
	DWORD threadFlags;
	DWORD threadID;

};
