/*!
\file winsemaphore.h "server\desktop\src\cross\windows\threadlock\winsemaphore.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 05 July 2017
*/

#pragma once

#include <windows.h>
#include "../../ilocker.h"

namespace MSIYBCore
{
	/*!
	\class WinSemaphore winsemaphore.h "server\desktop\src\cross\windows\threadlock\winsemaphore.h"
	\brief  Windows depended structure of semaphore.
	Provides windows-specified access to semaphore.
	*/
	class WinSemaphore : public ILocker
	{
	public:
		/*!
		Constructor. Set timeout.
		\param[in] timeout Timeout after which OS stop trying access locker object.
		*/
		WinSemaphore(long initialCount = 0, long maxCount = 1, unsigned long timeout = INFINITE);

		/*!
		Constructor copy
		\param[in] other Semaphore object to copy
		*/
		WinSemaphore(const WinSemaphore& other);

		/*!
		Constructor transfer
		\param[in] other Semaphore object to transfer
		*/
		WinSemaphore(WinSemaphore&& other);

		/*!
		Creates semaphore with specified attributes.
		\param[in] semaphoreAttr Determines if semaphore handle can be inherited by child.
		\param[in] initialOwner If this value is TRUE and the caller created the semaphore, the calling thread obtains initial ownership of the semaphore object.
		\param[in] timeout Timeout after which OS stop trying access locker object.
		*/
		WinSemaphore(LPSECURITY_ATTRIBUTES semaphoreAttr, long initialCount, long maxCount, unsigned long timeout = INFINITE);

		/*!
		Creates semaphore with specified attributes.
		\param[in] semaphoreAttr Determines if semaphore handle can be inherited by child.
		\param[in] initialOwner If this value is TRUE and the caller created the semaphore, the calling thread obtains initial ownership of the semaphore object.
		\param[in] timeout Timeout after which OS stop trying access locker object.
		*/
		WinSemaphore(t_secattr semaphoreAttr, long initialCount, long maxCount,  unsigned long timeout = INFINITE);

		/*!
		Empty.
		*/
		~WinSemaphore();

		/*!
		Copy.
		\param[in] other Semaphore object to copy
		*/
		WinSemaphore& operator=(const WinSemaphore& other);

		/*!
		Locks the semaphore.
		If another thread has already locked the semaphore,
		a call to lock will block execution until the lock is acquired.
		\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
		*/
		bool Lock() override;

		/*!
		Unavailable for semaphore object.
		\return FALSE
		*/
		bool LockShared() override;

		/*!
		Try to lock the semaphore object.
		If another thread has already locked the semaphore,
		function returns false immediatly.
		\return TRUE if the state of specified object is signaled and FALSE if object already locked.
		*/
		bool TryLock() override;

		/*!
		Unavailable for semaphore object.
		\return FALSE
		*/
		bool TryLockShared() override;

		/*
		Unlocks the semaphore.
		\return TRUE if succeed (always in semaphore case / or Exception).
		*/
		bool Unlock() override;

	private:
		/*!
		Parse WaitForSingleObject response
		\param[in] event Rsponse from WaitForSingleObject function
		\return TRUE if the state of specified object is signaled and FALSE in other case (ex. timeout).
		*/
		bool WaitEventResponse(unsigned long event);

		/*!
		Initialise semaphore with specified attributes.
		\param[in] semaphoreAttr Determines if semaphore handle can be inherited by child.
		\param[in] initialOwner If this value is TRUE and the caller created the semaphore, the calling thread obtains initial ownership of the semaphore object.
		\param[in] name The name of the semaphore object.
		\param[in] timeout Timeout after which OS stop trying access locker object.
		*/
		void Init(t_secattr semaphoreAttr, long initialCount, long maxCount, unsigned long timeout = INFINITE);

		/*!
		Initialise semaphore with specified attributes.
		\param[in] semaphoreAttr Determines if semaphore handle can be inherited by child.
		\param[in] initialOwner If this value is TRUE and the caller created the semaphore, the calling thread obtains initial ownership of the semaphore object.
		\param[in] name The name of the semaphore object.
		\param[in] timeout Timeout after which OS stop trying access locker object.
		*/
		void Init(LPSECURITY_ATTRIBUTES semaphoreAttr, long initialCount, long maxCount, unsigned long timeout = INFINITE);

		/*!
		Create semaphore object;
		*/
		void Create();

		HANDLE _hSemaphore;						///< Handle of semaphore object.
		LPSECURITY_ATTRIBUTES _semaphoreAttr;	///< Security attributes
		long _initialCount;						///< The initial count for the semaphore object.
		long _maxCount;							///< The maximum count for the semaphore object. This value must be greater than zero.
		unsigned long _timeout;					///< Timeout after which OS stop trying access locker object.
	};
}
