#include "winlocker.h"

ILocker* WinLocker::GetLocker(LockerAttr attributes)
{
	if (!attributes.oneProcess)
	{
		return (ILocker*) new WinMutex(attributes.timeout);
	}

	if (attributes.al == additionalLock::ESHARED)
	{
		return (ILocker*) new WinSRWLock();
	}

	if (attributes.al == additionalLock::EREC)
	{
		return (ILocker*) new WinCriticalSection(attributes.SLC);
	}

	return (ILocker*) new WinSRWLock();

}
