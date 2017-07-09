/*!
\file winmutex.h "server\desktop\src\cross\windows\winmutex.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 05 July 2017
*/

#pragma once

#include "threadlock\winmutex.h"
#include "threadlock\wincriticalsection.h"

/*!
\class WinMutex winmutex.h "server\desktop\src\cross\windows\winmutex.h"
\brief  Factory of locker objects.
*/
class WinLocker
{
public:
	/*!
	Determine which type of locker object is the best depending on incoming parameters.
	\param[in] useTry Define if user need TryLock() method.
	\param[in] oneProcess Define if locker will be used in one or many processes.
	\param[in] security Determines if handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the locker, the calling thread obtains initial ownership of the locker object.
	\param[in] name The name of the locker object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	\return New locker object.
	*/
	static ILocker* GetLocker( 
		bool useTry = false,
		bool oneProcess = true,
		unsigned long timeout = 1000,
		t_secattr security = t_secattr::ENULL, 
		bool initialOwner = FALSE, 
		char* name = NULL
	);
};
