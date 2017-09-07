/*!
\file allocator.h "server\desktop\src\common\allocator.h"
\authors Alexandr Barulev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 07 March 2017
*/

#pragma once
#include "../defines.h"
#include "../tools/exception.h"
#include <string>

using std::string;

/*!
Realocates memory for char string array
\param[out] arrayStrings Allocated block for strings array
\param[in] oldSize Current array size
\param[in] rCoeff New size will be result of multiplying rCoef and oldSize
*/
void ResizeStringArray(char ***arrayStrings, size_lt *oldSize, int rCoeff);

/*!
Realocates memory for char string 
\param[out] string Allocated block for string
\param[in] oldSize Current string size
\param[in] rCoeff New size will be result of multiplying rCoef and oldSize
*/
void ResizeString(char **string, size_lt *oldSize, int rCoeff);

/*!
Realocates memory for char string with constatnt size
\param[out] arrayStrings Allocated block for string
\param[in] oldSize Current string size
\param[in] newSize New string size
*/
void Resize(char **string, size_lt *oldSize, size_lt newSize);

/*!
Convert array of char strings into byte array
\param[out] byteArr Array of byte
\param[in] str Array of char string to convert
\param[in] cnt Amount of strings in array
\return Amount of bytes in byte array
*/
size_lt ConvertCharStringArrayToByte(byte** byteArr, char **str, size_lt cnt);

/*!
Convert array of strings into byte array
\param[out] byteArr Array of byte
\param[in] str Array of string to convert
\param[in] cnt Amount of strings in array
\return Amount of bytes in byte array
*/
size_lt ConvertStringArrayToByte(byte** byteArr, string *str, size_lt cnt);