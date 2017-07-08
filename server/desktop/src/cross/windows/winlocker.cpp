#include "winlocker.h"

ILocker* WinLocker::GetLocker(unsigned long timeout, bool oneProcess, bool useTry)
{
	if (oneProcess)
	{
		return (ILocker*)new WinMutex();
	}
}
