/*!
\file ilocker.h "server\desktop\src\cross\ilocker.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 2.0
\date 05 July 2017
*/

#pragma once
#include "../defines.h"
#include "threadsecurity.h"
#include "../tools/exceptions/lockerexception.h"

/*!
\class ILocker ilocker.h "server\desktop\src\cross\ilocker.h"
\brief  Class-interface for OS depended classes
Defined methods for locker should be realized in OS depended classes
*/
class ILocker
{
public:

	/*!
	Create locker object;
	*/
	virtual void Create() = 0;

	/*
	Locks the locker.
	If another thread has already locked the locker,
	a call to lock will block execution until the lock is acquired.
	\param[in] timeout Set timeout interval.
	\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
	*/
	virtual bool Lock() = 0;

	/*
	Unlocks the locker.
	\return TRUE if succeed.
	*/
	virtual bool Unlock() = 0;

};
