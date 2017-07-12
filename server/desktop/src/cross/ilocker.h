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
\struct LockerAttributes ilocker.h "server\desktop\src\cross\ilocker.h"
\brief Locker attributes.
*/
typedef struct LockerAttributes
{
	bool useTry;			///< Define if user need TryLock() method.
	bool oneProcess;		///< Define if locker will be used in one or many processes.
	unsigned long timeout;	///< Timeout after which OS stop trying access locker object.
	unsigned long SLC;		///< Amount of spinlock iterations before system switches in kernel mode.
	t_secattr security;		///< Determines if handle can be inherited by child.
	bool initialOwner;		///< Obtains initial ownership of the locker object.
	char* name = NULL;		///< The name of the locker object.

	/*!
	Set initial values.
	*/
	LockerAttributes() :
		useTry(false),
		oneProcess(true),
		timeout(INFINITE),
		SLC(0),
		security(t_secattr::ENULL),
		initialOwner(FALSE),
		name(NULL) {}
} LockerAttr;

/*!
\class ILocker ilocker.h "server\desktop\src\cross\ilocker.h"
\brief  Class-interface for OS depended classes
Defined methods for locker should be realized in OS depended classes
*/
class ILocker
{
public:

	/*!
	Locks the locker object.
	If another thread has already locked the locker,
	a call to lock will block execution until the lock is acquired.
	\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
	*/
	virtual bool Lock() = 0;

	/*!
	Try to lock the locker object.
	If another thread has already locked the locker,
	function returns false immediatly.
	\return TRUE if the state of specified object is signaled and FALSE if object already locked.
	*/
	virtual bool TryLock() = 0;

	/*!
	Unlocks the locker.
	\return TRUE if succeed.
	*/
	virtual bool Unlock() = 0;

	/*!
	Set timeout to try lock the locker object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	*/
	void SetTimeout(unsigned long timeout) { _timeout = timeout; }

protected:
	unsigned long _timeout;		///< Timeout after which OS stop trying access locker object.

};
