/*!
\file locker.h "server\desktop\src\common\locker.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 08 July 2017
*/

#pragma once

#ifdef _WIN32
#include "../cross/windows/winlocker.h"
#elif _unix_
#include "../cross/unix/unixlocker.h"
#endif

namespace MSIYBCore {

	typedef enum
	{
		ELOCKDEFAULT,
		ELOCKTRY,
		ELOCKSHARED,
		ELOCKTRYSHARED
	} LockMethod;

	/*!
	\class Locker locker.h "server\desktop\src\common\locker.h"
	\brief  RAII for mutex, spinlock, SRWlock, critical etc
	*/
	class Locker
	{
	public:
		/*!
		Lock object.
		\param[in] locker Locker object.
		*/
		Locker(ILocker& locker, LockMethod lockMethod = LockMethod::ELOCKDEFAULT);

		/*!
		Unlock object.
		*/
		~Locker();

		/*!
		Check success of lock.
		\return TRUE if the state of specified object is signaled and FALSE in other case
		*/
		bool WasLocked();

	private:
		ILocker& _locker;	///< Locker object
		bool _state;		/// true if lock succeed false in other case
	};
}
