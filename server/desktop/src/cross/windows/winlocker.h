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

namespace MSIYBCore
{
	typedef WinCriticalSection DefaultLock;
	typedef WinMutex Mutex;
	typedef WinSRWLock RWLock;
}
