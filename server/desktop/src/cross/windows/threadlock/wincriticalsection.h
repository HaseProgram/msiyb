/*!
\file wincriticalsection.h "server\desktop\src\cross\windows\threadlock\wincriticalsection.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 09 July 2017
*/

#pragma once

#include "../../ilocker.h"
#include "windows.h"

/*!
\class WinCriticalSection wincriticalsection.h "server\desktop\src\cross\windows\threadlock\wincriticalsection.h"
\brief  Structure of critical section.
Provides access to critical section.
*/
class WinCriticalSection : public ILocker
{
public:
	/*!
	Constructor. Set SpinLock count.
	\param[in] spinLockCount Amount of spinlock iterations before system switches in kernel mode.
	*/
	WinCriticalSection(unsigned long spinLockCount = 0);

	/*!
	Constructor copy
	\param[in] other CriticalSection object to copy
	*/
	WinCriticalSection(const WinCriticalSection& other);

	/*!
	Constructor transfer
	\param[in] other CriticalSection object to transfer
	*/
	WinCriticalSection(WinCriticalSection&& other);

	/*!
	Copy.
	\param[in] other CriticalSection object to copy
	*/
	WinCriticalSection& operator=(const WinCriticalSection& other);

	/*!
	Deletes critical section.
	*/
	~WinCriticalSection();

	/*!
	Initialise critical section with specified attributes.
	\param[in] spinLockCount Iteration amount of spinlock before OS switch into kernel mode.
	*/
	void Init(unsigned long spinLockCount = 0);

	/*!
	Enter the critical section.
	If another thread has already locked the criticalsection,
	a call to lock will block execution until the lock is acquired.
	\return always TRUE.
	*/
	bool Lock() override;

	/*!
	Try to enter critical section.
	If another thread has already entered,
	function returns false immediatly.
	\return TRUE if thread has entered critical section and FALSE in other case.
	*/
	bool TryLock() override;

	/*!
	Leaves the critical section, if it's active.
	\return TRUE if succeed and FALSE if thread didn't enter in critical section.
	*/
	bool Unlock() override;

private:
	bool _ready;				///< Flag is true if critical section was init.
	int _inUse;					///< Flag is above 0 if critical section is in use.
	CRITICAL_SECTION _critical;	///< Critical section speciment.
	DWORD _spinLockCount;		///< Iteration amount of spinlock before OS switch into kernel mode.
};
