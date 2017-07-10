#include "winlocker.h"

ILocker* WinLocker::GetLocker(LockerAttr attributes)
{
	if (!attributes.oneProcess)
	{
		return (ILocker*) new WinMutex(attributes.timeout);
	}

	if (!attributes.useTry)
	{
		return (ILocker*) new WinSRWLock();
	}

	return (ILocker*) new WinCriticalSection(attributes.SLC);
}
