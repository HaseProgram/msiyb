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
#include "threadlock\winsrwlock.h"

/*!
\class WinLocker winlocker.h "server\desktop\src\cross\windows\winlocker.h"
\brief  Factory of locker objects.
*/
class WinLocker
{
public:
	/*!
	Determine which type of locker object is the best depending on incoming parameters.
	\param[in] attributes Structure that defines locker parameters.
	\return New locker object.
	*/
	static ILocker* GetLocker(LockerAttr attributes);
};
