/*!
\file ithread.h "server\desktop\src\cross\ithread.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 02 March 2017
*/

#pragma once
#include "../tools/exceptions/thredexception.h"

/// Security attributes
typedef enum
{
	ENULL	///< The object is assigned the default security descriptor
} t_secattr;

/// The flags that control the creation of the thread.
typedef enum
{
	RUNIMMEDIATLY,		///< The thread runs immediately after creation.
	CREATESUSPENDED,	///< The thread is created in a suspended state
	RESERVESTACK		///< Specifies the initial reserve size of the stack.
} t_flags;

/*!
\class IThread ithread.h "server\desktop\src\cross\ithread.h"
\brief  Class-interface for OS depended classes
Defined methods for threads should be realized in OS depended classes
*/
class IThread
{
public:
	int maxThreadsCount;	///< Amount of threads can be launched (logical proc)
	
	/*!
	Returns maximum amount of threads can be launched
	\return Maximum amount of threads can be launched
	*/
	virtual int GetMaxThreadCount() = 0;

	/*!
	Set up thread before launch
	\param[in] threadFunc Pointer to a function to be executed by the thread.
	\param[in] threadFuncArgs A pointer to a variable to be passed to the thread.
	\param[in] threadStackSize The initial size of the stack, in bytes.
	\param[in] threadFlags The flags that control the creation of the thread
	\param[in] threadSecurityAttributes Determines if returned handle can be inherited by child.
	*/
	virtual void Init(void* threadFunc, void* threadFuncArgs, size_t threadStackSize, t_flags threadFlags, t_secattr threadSecurityAttributes) = 0;
	
	/*!
	Launch thread with setted parameters.
	*/
	virtual void Start() = 0;

	/*!
	Returns launched thread ID
	\return Launched thread ID
	*/
	virtual long GetThreadID() = 0;

	/*!
	Check if thread comleted his work.
	\param[out] result Value returned from thread function.
	\return TRUE if still active and FASLE in other case
	*/
	virtual bool CheckActive(void *result = nullptr) = 0;
};
