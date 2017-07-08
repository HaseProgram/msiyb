/*!
\file winmutex.h "server\desktop\src\cross\windows\winmutex.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 05 July 2017
*/

#pragma once

#include "../../ilocker.h"
#include "windows.h"
#include "../unicodeconverter.h"

/*!
\class WinMutex winmutex.h "server\desktop\src\cross\windows\winmutex.h"
\brief  Windows depended structure of mutex.
Provides windows-specified access to mutex.
*/
class WinMutex : public ILocker
{
public:
	/*!
	Empty constructor.
	*/
	WinMutex();

	/*!
	Creates mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] name The name of the mutex object.
	*/
	WinMutex(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, LPCTSTR name);

	/*!
	Creates mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] name The name of the mutex object.
	*/
	WinMutex(t_secattr mutexAttr, bool initialOwner, char* name);

	/*!
	Clears handle.
	*/
	~WinMutex();

	/*!
	Initialise mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] timeout Set timeout interval.
	\param[in] name The name of the mutex object.
	*/
	void Init(t_secattr mutexAttr, bool initialOwner, char* name, unsigned long timeout = INFINITE);

	/*!
	Initialise mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] timeout Set timeout interval.
	\param[in] name The name of the mutex object.
	*/
	void Init(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, LPCTSTR name, unsigned long timeout = INFINITE);

	/*!
	Create mutex object;
	*/
	void Create() override;

	/*
	Locks the mutex. 
	If another thread has already locked the mutex, 
	a call to lock will block execution until the lock is acquired.
	\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
	*/
	bool Lock() override;

	/*
	Unlocks the mutex.
	\return TRUE if succeed.
	*/
	bool Unlock() override;

private:
	HANDLE _hMutex; ///< Handle of mutex object.
	LPSECURITY_ATTRIBUTES _mutexAttr;
	BOOL _initialOwner;
	LPCTSTR _name;
	unsigned long _timeout;
};
