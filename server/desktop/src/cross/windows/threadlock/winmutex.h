/*!
\file winmutex.h "server\desktop\src\cross\windows\threadlock\winmutex.h"
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
\class WinMutex winmutex.h "server\desktop\src\cross\windows\threadlock\winmutex.h"
\brief  Windows depended structure of mutex.
Provides windows-specified access to mutex.
*/
class WinMutex : public ILocker
{
public:
	/*!
	Constructor. Set timeout.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	*/
	WinMutex(unsigned long timeout = INFINITE);

	/*!
	Constructor copy
	\param[in] other Mutex object to copy
	*/
	WinMutex(const WinMutex& other);

	/*!
	Constructor transfer
	\param[in] other Mutex object to transfer
	*/
	WinMutex(WinMutex&& other);

	/*!
	Creates mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	\param[in] name The name of the mutex object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	*/
	WinMutex(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, LPCTSTR name, unsigned long timeout = INFINITE);

	/*!
	Creates mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] name The name of the mutex object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	*/
	WinMutex(t_secattr mutexAttr, bool initialOwner, char* name, unsigned long timeout = INFINITE);

	/*!
	Empty.
	*/
	~WinMutex();

	/*!
	Copy.
	\param[in] other Mutex object to copy
	*/
	WinMutex& operator=(const WinMutex& other);

	/*!
	Initialise mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] name The name of the mutex object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	*/
	void Init(t_secattr mutexAttr, bool initialOwner, char* name, unsigned long timeout = INFINITE);

	/*!
	Initialise mutex with specified attributes.
	\param[in] mutexAttr Determines if mutex handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.
	\param[in] name The name of the mutex object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	*/
	void Init(LPSECURITY_ATTRIBUTES mutexAttr, BOOL initialOwner, LPCTSTR name, unsigned long timeout = INFINITE);

	/*!
	Create mutex object;
	*/
	void Create();

	/*!
	Locks the mutex. 
	If another thread has already locked the mutex, 
	a call to lock will block execution until the lock is acquired.
	\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
	*/
	bool Lock() override;

	/*!
	Try to lock the mutex object.
	If another thread has already locked the mutex,
	function returns false immediatly.
	\return TRUE if the state of specified object is signaled and FALSE if object already locked.
	*/
	bool TryLock() override;

	/*
	Unlocks the mutex.
	\return TRUE if succeed (always in mutex case / or Exception).
	*/
	bool Unlock() override;

private:
	/*!
	Parse WaitForSingleObject response
	\param[in] event Rsponse from WaitForSingleObject function
	\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
	*/
	bool WaitEventResponse(unsigned long event);

	HANDLE _hMutex;						///< Handle of mutex object.
	LPSECURITY_ATTRIBUTES _mutexAttr;	///< Security attributes
	BOOL _initialOwner;					///< The calling thread obtains initial ownership of the mutex object.
	LPCTSTR _name;						///< Mutex name.
};
