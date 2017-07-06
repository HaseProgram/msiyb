/*!
\file imutex.h "server\desktop\src\cross\imutex.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 05 July 2017
*/

#pragma once
#include "../defines.h"
#include "threadsecurity.h"
#include "../tools/exceptions/mutexexception.h"

/*!
\class IMutex imutex.h "server\desktop\src\cross\imutex.h"
\brief  Class-interface for OS depended classes
Defined methods for mutex should be realized in OS depended classes
*/
class IMutex
{
public:
	/*!
	Initialise mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] name The name of the mutex object.
	*/
	virtual void Init(t_secattr mutexAttr, bool initialOwner, char* name) = 0;
	
	/*!
	Create mutex object;
	*/
	virtual void Create() = 0;

	/*
	Locks the mutex.
	If another thread has already locked the mutex,
	a call to lock will block execution until the lock is acquired.
	\param[in] timeout Set timeout interval.
	\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
	*/
	virtual bool Lock(unsigned long timeout) = 0;

	/*
	Unlocks the mutex.
	\return TRUE if succeed.
	*/
	virtual bool Unlock() = 0;
	
};
