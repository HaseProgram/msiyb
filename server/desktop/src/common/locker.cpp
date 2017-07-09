#include "locker.h"

Locker::Locker(ILocker& locker) : _locker(locker)
{
	_locker.Lock();
}

Locker::~Locker()
{ 
	_locker.Unlock(); 
}

ILocker* Locker::GetLocker(bool useTry, bool oneProcess, unsigned long timeout, t_secattr security, bool initialOwner, char* name)
{
	return OSLocker::GetLocker(useTry, oneProcess, timeout, security, initialOwner, name);
}
