#include "winlocker.h"

ILocker* WinLocker::GetLocker(bool useTry, bool oneProcess, unsigned long timeout, t_secattr security, bool initialOwner, char* name)
{
	if (!oneProcess)
	{
		return (ILocker*) new WinMutex(timeout);
	}

	if (!useTry)
	{
		// return (ILocker*) new WinSRWLock();
	}

	return (ILocker*) new WinCriticalSection(timeout);
}
