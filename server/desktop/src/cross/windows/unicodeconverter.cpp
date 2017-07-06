#include "unicodeconverter.h"

void ConvertCharToTCHAR(const char *cStr, TCHAR *tStr)
{
#ifdef _UNICODE
	mbstowcs((wchar_t*)tStr, cStr, strlen(cStr) + 1);
#else
	strcpy(tStr, cStr);
#endif
}

void ConvertTCHARToChar(TCHAR *tStr, char *cStr)
{
#ifdef _UNICODE
	wcstombs(cStr, (wchar_t*)tStr, wcslen((wchar_t*)tStr) + 1);
#else
	strcpy(cStr, tStr);
#endif
}