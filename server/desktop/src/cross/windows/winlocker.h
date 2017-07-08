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
#include <memory.h>
#include <memory>

/*!
\class WinMutex winmutex.h "server\desktop\src\cross\windows\winmutex.h"
\brief  Windows depended structure of mutex.
Provides windows-specified access to mutex.
*/
class WinLocker
{
public:
	static ILocker* GetLocker(unsigned long timeout = INFINITE, bool oneProcess = false, bool useTry = false);
};
