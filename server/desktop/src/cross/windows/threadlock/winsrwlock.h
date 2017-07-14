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
	Constructor. Empty.
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
	Initialise SRW lock.
	*/
	void Init();

	/*!
	Enter the SRW lock. Exclusive lock. (For writers).
	If another thread has already locked the srwlock,
	a call to lock will block execution until the lock is acquired.
	\return always TRUE.
	*/
	bool Lock() override;

	/*!
	Locks the locker object in shared mode
	so other reader thread can use it as well.
	\return always TRUE.
	*/
	bool LockShared() override;

	/*!
	Try to enter critical section.
	If another thread has already entered,
	function returns false immediatly.
	\return TRUE if thread has been locked and FALSE in other case.
	*/
	bool TryLock() override;

	/*!
	Try to lock the locker object in shared mode.
	If the call is successful, the calling thread takes ownership of the lock.
	function returns false immediatly.
	\return TRUE if the state of specified object is signaled and FALSE if object already locked.
	*/
	bool TryLockShared() override;

	/*!
	Leaves the critical section, if it's active.
	\return TRUE if succeed and FALSE if thread didn't enter in critical section.
	*/
	bool Unlock() override;

private:
	volatile long _inUse;
	volatile long _shared;		///< Flag of shared or exclusive. -1 - unlocked; 0 - shared; 1 - exclusive.
	bool _ready;				///< Flag is true if srw lock was init.
	SRWLOCK _srw;				///< SRW speciment.
};
