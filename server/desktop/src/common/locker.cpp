#include "locker.h"

Locker::Locker(ILocker& locker) : _locker(locker)
{
	_locker.Lock();
}

Locker::~Locker()
{ 
	_locker.Unlock(); 
}

ILocker* Locker::GetLocker(LockerAttr attributes)
{
	return OSLocker::GetLocker(attributes);
}
