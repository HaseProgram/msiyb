#pragma once

class IThread
{
public:
	int maxThreadsCount;
	
	virtual int GetMaxThreadCount() = 0;
	virtual void Start() = 0;
	virtual void Interrupt() = 0;
	virtual long GetThreadID() = 0;
};
