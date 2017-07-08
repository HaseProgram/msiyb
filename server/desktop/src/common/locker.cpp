#include "locker.h"

Locker::Locker(ILocker& locker) : _locker(locker)
{
	_locker.Lock();
}

Locker::~Locker()
{ 
	_locker.Unlock(); 
}

ILocker* Locker::GetLocker(unsigned long timeout, bool oneProcess , bool useTry)
{
	return OSLocker::GetLocker(timeout, oneProcess, useTry);
}
