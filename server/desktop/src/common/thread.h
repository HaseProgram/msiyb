#pragma once

#ifdef _WIN32
#include "../cross/windows/winthread.h"
typedef WinThread OSThread;
#elif _unix_
#include "../cross/unix/unixthread.h"
typedef UnixThread OSThread;
#endif

using namespace std;

int Thread::activeThreadsCount = 0;

class Thread
{
public:
	static int activeThreadsCount;
	
	Thread();

private:
	IThread* thread;
};
