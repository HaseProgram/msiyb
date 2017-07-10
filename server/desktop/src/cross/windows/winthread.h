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
#include "windows.h"

/*!
\class WinThread winthread.h "server\desktop\src\cross\windows\winthread.h"
\brief  Windows depended structure of thread.
Provides windows-specified access to thread creation (and handle it).
*/
class WinThread : public IThread
{
public:
	int status;			///< Thread status. Zero value if checking exit status fails, non-zero in other case.
	
	/*!
	Empty.
	*/
	WinThread();

	/*!
	Clears handle.
	*/
	~WinThread();

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
	virtual void Init(void* threadFunc, void* threadFuncArgs, size_t threadStackSize, t_flags threadFlags, t_secattr threadSecurityAttributes) override;

	/*!
	Returns maximum amount of threads can be launched. Static.
	\return Maximum amount of threads can be launched.
	*/
	static int GetMaxThreadCount();

	/*!
	Launch thread with setted parameters.
	*/
	virtual void Start() override;

	/*!
	Returns launched thread system ID.
	\return Launched thread system ID.
	*/
	virtual long GetThreadID() override;

	/*!
	Check if thread comleted his work.
	\param[in] threadID ID of thread to check.
	\param[out] result Value returned from thread function.
	\return TRUE if still active and FALSE in other case
	*/
	virtual bool CheckActive(void *result = nullptr) override;

private:	
	HANDLE _hThread;		///< Handle of thread.
	
	/* Thread creation parameters */
	LPSECURITY_ATTRIBUTES _threadSecurityAttributes;	///< Determines if returned handle can be inherited by child.
	SIZE_T _threadStackSize;							///< The initial size of the stack, in bytes.
	LPTHREAD_START_ROUTINE _threadFunc;					///< Pointer to a function to be executed by the thread.
	LPVOID _threadFuncArgs;								///< A pointer to a variable to be passed to the thread.
	DWORD _threadFlags;									///< The flags that control the creation of the thread.

	DWORD _threadSystemID;		///< Launched threadID
	LPDWORD _exitCode;			///< Recive the thread termination status

};
