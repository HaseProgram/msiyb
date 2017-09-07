/*!
\file wincv.h "server\desktop\src\cross\windows\threadlock\wincv.h"
\authors Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 1.0
\date 08 August 2017
*/

#pragma once

#include <Windows.h>
#include "wincriticalsection.h"
#include "winsrwlock.h"
#include "../unicodeconverter.h"

namespace MSIYBCore {

	// We need detect if we can use win32 CONDITION_VARIABLES, so we search for theese functions.
	typedef void(__cdecl *InitCV)(PCONDITION_VARIABLE);
	typedef bool(__cdecl *SleepCV)(PCONDITION_VARIABLE, PCRITICAL_SECTION, DWORD);
	typedef bool(__cdecl *WakeCV)(PCONDITION_VARIABLE);
	typedef bool(__cdecl *WakeAllCV)(PCONDITION_VARIABLE);

	InitCV initCVfn;
	SleepCV sleepCVfn;
	WakeCV wakeCVfn;
	WakeAllCV wakeAllCVfn;

	/*!
	Determine if our OS has CONDITION_VARIABLES.
	\return TRUE if we can use CONDITION_VARIABLES and FALSE in other case.
	*/
	bool ConditionVariableFunctions();

	class CV
	{
	public:
		virtual ~CV() = 0;
		virtual void Wait() = 0;
		virtual void Signal() = 0;
		virtual void Broadcast() = 0;
	};


	/*!
	\class ConditionVariable wincv.h "server\desktop\src\cross\windows\threadlock\wincv.h"
	\brief  Impementation of condition variable
	*/
	class ConditionVariable : public CV
	{
	public:
		/*!
		
		*/
		ConditionVariable();

		/*!
		
		*/
		~ConditionVariable() override;

		

	private:
		CONDITION_VARIABLE _cv;
	};
}
