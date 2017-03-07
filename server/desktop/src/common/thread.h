/*!
\file thread.h "server\desktop\src\common\thread.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
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

using namespace std;

/*!
\class Thread thread.h "server\desktop\src\common\thread.h"
\brief  Thread interface.
Provide interface to common thread structure and it's action.
Factory for unix/bsd/windows structure of thread.
*/
class Thread
{
public:
	static int activeThreadsCount;		///< Amount of current active threads
	static vector<Thread*> ThreadList;	///< List of current thread objects

	bool runned;	///< TRUE if we started thread; false if not yet

	/*!
	Initialises OS depended thread structure.
	Add self in ThreadList
	*/
	Thread();

	/*!
	Dealocates memory of OS depended object.
	Decrease active threads count
	*/
	~Thread();

	/*!
	Try to start new thread. If number of active threads is already
	equal to max thread count, looks for completed threads and tried one more time.
	\param[in] threadFunc Pointer to a function to be executed by the thread.
	\param[in] threadFuncArgs A pointer to a variable to be passed to the thread.
	\return TRUE if new thread started, NO in other case.
	*/
	bool Start(void* threadFunc, void* threadFuncArgs);

	/*!
	Checks if current thread started and completed it's work
	\return TRUE if completed, FALSE in other case
	*/
	bool CheckCompleted();

	/*!
	Checks if current thread started and not completed it's work yet
	\return TRUE if still active, FALSE in other case
	*/
	bool CheckActive();

	/*!
	Looks for completed threads in threads list and free them.
	*/
	static void CheckListCompleted();

private:
	IThread* thread;	///< OS depended thread structure
};

int Thread::activeThreadsCount = 0;
vector<Thread*> Thread::ThreadList;
