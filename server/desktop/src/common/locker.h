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
typedef WinLocker OSLocker;
#elif _unix_
#include "../cross/unix/unixlocker.h"
typedef UnixLocker OSLocker;
#endif

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
	Locker(ILocker& locker);

	/*!
	Unlock object.
	*/
	~Locker();

	/*!
	Determine which type of locker object is the best depending on incoming parameters.
	\param[in] useTry Define if user need TryLock() method.
	\param[in] oneProcess Define if locker will be used in one or many processes.
	\param[in] security Determines if handle can be inherited by child.
	\param[in] initialOwner If this value is TRUE and the caller created the locker, the calling thread obtains initial ownership of the locker object.
	\param[in] name The name of the locker object.
	\param[in] timeout Timeout after which OS stop trying access locker object.
	\return New locker object.
	*/
	static ILocker* GetLocker(
		bool useTry = false,
		bool oneProcess = true,
		unsigned long timeout = 1000,
		t_secattr security = t_secattr::ENULL,
		bool initialOwner = FALSE,
		char* name = NULL
	);

private:
	ILocker& _locker;	///< Locker object
};
