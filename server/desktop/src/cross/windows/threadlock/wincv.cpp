#include "wincv.h"

bool MSIYBCore::ConditionVariableFunctions()
{
	TCHAR* moduleName;
	ConvertCharToTCHAR("kernel32", moduleName);
	HMODULE module = GetModuleHandle(moduleName);

	initCVfn = (InitCV)GetProcAddress(module, "InitializeConditionVariable");
	if (!initCVfn)
	{
		return false;
	}

	sleepCVfn = (SleepCV)GetProcAddress(module, "SleepConditionVariableCS");
	if (!sleepCVfn)
	{
		return false;
	}

	wakeCVfn = (WakeCV)GetProcAddress(module, "WakeConditionVariable");
	if (!wakeCVfn)
	{
		return false;
	}

	wakeAllCVfn = (WakeAllCV)GetProcAddress(module, "WakeAllConditionVariable");
	if (!wakeAllCVfn)
	{
		return false;
	}

	return true;
}
