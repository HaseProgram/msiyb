/*!
\file winsrwlock.h "server\desktop\src\cross\windows\threadlock\winsrwlock.h"
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
\class WinSRWLock winsrwlock.h "server\desktop\src\cross\windows\threadlock\winsrwlock.h"
\brief  Structure of SRW lock.
*/
class WinSRWLock : public ILocker
{
public:
	/*!
	Constructor. Set timeout.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	*/
	WinSRWLock();

	/*!
	Constructor copy
	\param[in] other SRWLock object to copy
	*/
	WinSRWLock(const WinSRWLock& other);

	/*!
	Constructor transfer
	\param[in] other SRWLock object to transfer
	*/
	WinSRWLock(WinSRWLock&& other);

	/*!
	Copy.
	\param[in] other SRWLock object to copy
	*/
	WinSRWLock& operator=(const WinSRWLock& other);

	/*!
	Deletes critical section.
	*/
	~WinSRWLock();

	/*!
	Initialise critical section with specified attributes.
	\param[in] spinLockCount Iteration amount of spinlock before OS switch into kernel mode.
	*/
	void Init(unsigned long spinLockCount = 0);

	/*!
	Enter the critical section.
	If another thread has already locked the srwlock,
	a call to lock will block execution until the lock is acquired.
	\return always TRUE.
	*/
	bool Lock() override;

	/*!
	Leaves the critical section, if it's active.
	\return TRUE if succeed and FALSE if thread didn't enter in critical section.
	*/
	bool Unlock() override;

private:
	/*!
	SRW lock can not try to lock, so it's private
	Empty.
	\return false.
	*/
	bool TryLock() override;

	bool _ready;				///< Flag is true if critical section was init.
	CRITICAL_SECTION _critical;	///< Critical section speciment.
	DWORD _spinLockCount;		///< Iteration amount of spinlock before OS switch into kernel mode.
};
