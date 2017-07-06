#pragma once
#include <Windows.h>

/*!
Convert char string into tchar.
\param[in] cStr char string.
\param[out] tStr tchar string.
*/
void ConvertCharToTCHAR(const char *cStr, TCHAR *tStr);

/*!
Convert tchar string into char.
\param[in] tStr tchar string.
\param[out] cStr char string.
*/
void ConvertTCHARToChar(TCHAR *tStr, char *cStr);