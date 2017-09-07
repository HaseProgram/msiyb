#include "locker.h"

using MSIYBCore::ILocker;
using MSIYBCore::Locker;
using MSIYBCore::LockMethod;

Locker::Locker(ILocker& locker, LockMethod lockMethod) : _locker(locker), _state(false)
{
	switch (lockMethod)
	{
	case LockMethod::ELOCKDEFAULT:
		_state = _locker.Lock();
		break;
	case LockMethod::ELOCKTRY:
		_state = _locker.TryLock();
		break;
	case LockMethod::ELOCKSHARED:
		_state = _locker.LockShared();
		break;
	case LockMethod::ELOCKTRYSHARED:
		_state = _locker.TryLockShared();
		break;
	}
}

Locker::~Locker()
{ 
	_locker.Unlock(); 
}

bool Locker::WasLocked()
{
	return _state;
}
