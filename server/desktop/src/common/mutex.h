/*!
\file mutex.h "server\desktop\src\common\mutex.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 05 July 2017
*/

#pragma once

#ifdef _WIN32
#include "../cross/windows/winmutex.h"
typedef WinMutex OSMutex;
#elif _unix_
#include "../cross/unix/unixmutex.h"
typedef UnixMutex OSMutex;
#endif

/*!
\class Mutex mutex.h "server\desktop\src\common\mutex.h"
\brief  Mutex interface.
Provide interface to mutex structure and it's action.
Factory for unix/bsd/windows structure of mutex.
*/
class Mutex
{
public:

	/*!
	Initialises OS depended mutex structure.
	*/
	Mutex();

	/*!
	Dealocates memory of OS depended object.
	*/
	~Mutex();

private:
	/*!
	Locks the mutex. Blocks if mutex is not available.
	*/
	void Lock();

	/*!
	Unlocks the mutex.
	*/
	void Unlock();
	
	OSMutex *_mutex;		///< OS depended mutex structure.
};
