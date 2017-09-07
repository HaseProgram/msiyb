/*!
\file thread.h "server\desktop\src\common\thread.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 2.0
\date 02 March 2017
*/

#pragma once

#ifdef _WIN32
#include "../cross/windows/winthread.h"
typedef WinThread OSThread;
#elif _unix_
#include "../cross/unix/unixthread.h"
typedef UnixThread OSThread;
#endif

/*!
\class Thread thread.h "server\desktop\src\common\thread.h"
\brief  Thread interface.
Provide interface to single thread structure and it's action.
Factory for unix/bsd/windows structure of thread.
*/
class Thread
{
public:

	/*!
	Initialises OS depended thread structure.
	*/
	Thread();

	/*!
	Dealocates memory of OS depended object.
	*/
	~Thread();

	/*!
	Try to start new thread. If number of active threads is already
	equal to max thread count, looks for completed threads and tried one more time.
	\param[in] threadFunc Pointer to a function to be executed by the thread.
	\param[in] threadFuncArgs A pointer to a variable to be passed to the thread.
	\param[out] result Value returned from thread function.
	\return Thread ID if new thread started.
	*/
	int Start(void *threadFunc, void *threadFuncArgs, void *result = nullptr);

	void* GetResult();

	/*!
	Checks if thread completed it's work
	\param[in] threadID Id if thread to check.
	\return TRUE if completed, FALSE in other case
	*/
	bool IsCompleted();

	/*!
	Returns maximum amount of threads can be launched. Static.
	\return Maximum amount of threads can be launched.
	*/
	static int GetMaxThreadCount();

private:
	int _id;				///< Current thread ID.
	void *_result;			///< Value returned from thread function.
	OSThread *_thread;		///< OS depended thread structure.
};
