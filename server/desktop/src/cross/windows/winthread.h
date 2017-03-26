/*!
\file winthread.h "server\desktop\src\cross\windows\winthread.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 02 March 2017
*/

#pragma once

#include "../ithread.h"

using namespace std;

/// Link between thread's id and it's handle.
typedef struct
{
	int threadID;		///< Thread ID.
	HANDLE hThread;		///< Handle of thread.
	int status;			///< Thread status. Zero value if checking exit status fails, non-zero in other case.

	/* Thread creation parameters */
	LPSECURITY_ATTRIBUTES threadSecurityAttributes;	///< Determines if returned handle can be inherited by child.
	SIZE_T threadStackSize;							///< The initial size of the stack, in bytes.
	LPTHREAD_START_ROUTINE threadFunc;				///< Pointer to a function to be executed by the thread.
	LPVOID threadFuncArgs;							///< A pointer to a variable to be passed to the thread.
	DWORD threadFlags;								///< The flags that control the creation of the thread.

	DWORD threadSystemID;		///< Launched threadID
	LPDWORD exitCode;			///< Recive the thread termination status
} THREADHANDLE;

/*!
\class WinThread winthread.h "server\desktop\src\cross\windows\winthread.h"
\brief  Windows depended structure of thread.
Provides windows-specified access to thread creation (and handle it).
*/
class WinThread : public IThread
{
public:
	/*!
	Determine amound of logical proccesses in system.
	Depended on logical processes determine maximum amound of threads.
	*/
	WinThread();

	/*!
	Clears handle array.
	*/
	~WinThread();

	/*!
	Set up thread before launch. Only windows oriented call. 
	\param[in] threadID ID of thread to launch.
	\param[in] threadFunc Pointer to a function to be executed by the thread.
	\param[in] threadFuncArgs A pointer to a variable to be passed to the thread.
	\param[in] threadStackSize The initial size of the stack, in bytes.
	\param[in] threadFlags The flags that control the creation of the thread.
	\param[in] threadSecurityAttributes Determines if returned handle can be inherited by child.
	*/
	void Init(int threadID, LPTHREAD_START_ROUTINE threadFunc, LPVOID threadFuncArgs, SIZE_T threadStackSize, DWORD threadFlags, LPSECURITY_ATTRIBUTES threadSecurityAttributes);

	/*!
	Set up thread before launch. Cross platform call.
	\param[in] threadID ID of thread to launch.
	\param[in] threadFunc Pointer to a function to be executed by the thread.
	\param[in] threadFuncArgs A pointer to a variable to be passed to the thread.
	\param[in] threadStackSize The initial size of the stack, in bytes.
	\param[in] threadFlags The flags that control the creation of the thread.
	\param[in] threadSecurityAttributes Determines if returned handle can be inherited by child.
	*/
	virtual void Init(int threadID, void* threadFunc, void* threadFuncArgs, size_t threadStackSize, t_flags threadFlags, t_secattr threadSecurityAttributes);

	/*!
	Returns maximum amount of threads can be launched.
	\return Maximum amount of threads can be launched.
	*/
	virtual int GetMaxThreadCount() override;

	/*!
	Launch thread with setted parameters.
	\param[in] threadID ID of thread to launch.
	*/
	virtual void Start(int threadID) override;

	/*!
	Returns launched thread system ID.
	\param[in] Local thread ID.
	\return Launched thread system ID.
	*/
	virtual long GetThreadID(int threadID) override;

	/*!
	Check if thread comleted his work.
	\param[in] threadID ID of thread to check.
	\param[out] result Value returned from thread function.
	\return TRUE if still active and FALSE in other case
	*/
	virtual bool CheckActive(int threadID, void *result = nullptr) override;

	/*!
	Check if thread with incoming thread ID already exists.
	\param[in] threadID ID of thread to check.
	\return TRUE if exists, FALSE in other case
	*/
	bool ThreadIDExists(int threadID);

private:
	void freeHandles(int handleID = -1);
	vector<THREADHANDLE*> hThreads;	///< Handles to the threads.
};
