/*!
\file winthread.h "server\desktop\src\cross\windows\winthread.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright � MSiYB 2017
\license GPL license
\version 0.1
\date 02 March 2017
*/

#pragma once

#include "../ithread.h"

using namespace std;

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
	Set up thread before launch. Only windows oriented call. 
	\param[in] threadFunc Pointer to a function to be executed by the thread.
	\param[in] threadFuncArgs A pointer to a variable to be passed to the thread.
	\param[in] threadStackSize The initial size of the stack, in bytes.
	\param[in] threadFlags The flags that control the creation of the thread.
	\param[in] threadSecurityAttributes Determines if returned handle can be inherited by child.
	*/
	void Init(LPTHREAD_START_ROUTINE threadFunc, LPVOID threadFuncArgs, SIZE_T threadStackSize, DWORD threadFlags, LPSECURITY_ATTRIBUTES threadSecurityAttributes);

	/*!
	Set up thread before launch. Cross platform call.
	\param[in] threadFunc Pointer to a function to be executed by the thread.
	\param[in] threadFuncArgs A pointer to a variable to be passed to the thread.
	\param[in] threadStackSize The initial size of the stack, in bytes.
	\param[in] threadFlags The flags that control the creation of the thread.
	\param[in] threadSecurityAttributes Determines if returned handle can be inherited by child.
	*/
	virtual void Init(void* threadFunc, void* threadFuncArgs, size_t threadStackSize, t_flags threadFlags, t_secattr threadSecurityAttributes);

	/*!
	Returns maximum amount of threads can be launched.
	\return Maximum amount of threads can be launched.
	*/
	virtual int GetMaxThreadCount() override;

	/*!
	Launch thread with setted parameters.
	*/
	virtual void Start() override;

	/*!
	Returns launched thread ID.
	\return Launched thread ID.
	*/
	virtual long GetThreadID() override;

	/*!
	Check if thread comleted his work.
	\return TRUE if still active and FALSE in other case
	*/
	virtual bool CheckActive() override;

private:
	HANDLE hThread;	///< Handle to the new thread.

	/* Thread creation parameters */
	LPSECURITY_ATTRIBUTES threadSecurityAttributes;	///< Determines if returned handle can be inherited by child.
	SIZE_T threadStackSize;							///< The initial size of the stack, in bytes.
	LPTHREAD_START_ROUTINE threadFunc;				///< Pointer to a function to be executed by the thread.
	LPVOID threadFuncArgs;							///< A pointer to a variable to be passed to the thread.
	DWORD threadFlags;								///< The flags that control the creation of the thread.

	DWORD threadID;		///< Launched threadID
	LPDWORD exitCode;	///< Recive the thread termination status
};