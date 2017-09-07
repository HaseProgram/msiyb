#include "stringmethods.h"

using namespace std;

void ResizeStringArray(char ***arrayStrings, size_lt *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char **oldPtr = *arrayStrings;
	char **newPtr = new char*[newSize];
	if (!newPtr) ThrowException("Can't resize string array!");
	memcpy(newPtr, oldPtr, *oldSize * sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*arrayStrings = newPtr;
}

void ResizeString(char **string, size_lt *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char *oldPtr = *string;
	char *newPtr = new char[newSize];
	if (!newPtr) ThrowException("Can't resize string!");
	memcpy(newPtr, oldPtr, *oldSize * sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

void Resize(char **string, size_lt *oldSize, size_lt newSize)
{
	char *oldPtr = *string;
	char *newPtr = new char[newSize];
	if (!newPtr) ThrowException("Can't resize string!");
	memcpy(newPtr, oldPtr, *oldSize);
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

size_lt ConvertCharStringArrayToByte(byte** byteArr, char **str, size_lt cnt)
{
	size_lt countBytes = 0;
	for (size_lt i = 0; i < cnt; i++)
	{
		countBytes += strlen(str[i]);
	}

	byte* buf = new byte[countBytes];
	if (!buf)
	{
		ThrowException("Can't allocate memory for buffer!");
	}

	size_lt pos = 0;
	for (size_lt i = 0; i < cnt; i++)
	{
		size_lt len = strlen(str[i]);
		memcpy(buf + pos, str[i], len);
		pos += len;
	}
	*byteArr = buf;
	return countBytes;
}

size_lt ConvertStringArrayToByte(byte** byteArr, string *str, size_lt cnt)
{
	size_lt countBytes = 0;
	for (size_lt i = 0; i < cnt; i++)
	{
		countBytes += str[i].length();
	}

	byte* buf = new byte[countBytes];
	if (!buf)
	{
		ThrowException("Can't allocate memory for buffer!");
	}

	size_lt pos = 0;
	for (size_lt i = 0; i < cnt; i++)
	{
		size_lt len = str[i].length();
		memcpy(buf + pos, str[i].c_str(), len);
		pos += len;
	}
	*byteArr = buf;
	return countBytes;
}
