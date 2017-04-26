#pragma once
#include <exception>
#include <stdio.h>

#ifdef _WIN32 
#define LastError GetLastError()
#elif __unix__
#define LastError errno
#endif

#define ERROR_MSG_SIZE 1024
#define DEBUG

#ifdef _WIN32
#include "windows.h"

#ifdef DEBUG
#define ThrowException(msg)											\
{																	\
	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);			\
}

#define ThrowExceptionWithCode(msg, code)							\
{																	\
	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__, code);	\
}
#else
#define ThrowException(msg)											\
{																	\
	throw Exception(msg);											\
}																	
#define void ThrowExceptionWithCode(msg, code)						\
{																	\
	throw Exception(msg, code);										\
}
#endif

inline const char *ParseException(int errCode)
{
	char *errMessage = new char[1024];
	LPSTR messageBuffer = NULL;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	//wcstombs(errMessage, messageBuffer, size);
	strcpy(errMessage, messageBuffer);
	//errMessage[size] = '\0';
	LocalFree(messageBuffer);
	return errMessage;
}
#elif __unix__
inline const char *ParseException(int errCode)
{
	char *errMessage = new char[1024];
	strcpy(errMessage, strerror(errCode));
	return errMessage;
}
#endif

class Exception : public std::exception
{	
protected:
	char message[ERROR_MSG_SIZE];
	long errCode;
public:


	Exception() {}

	// TODO LOG Exceptions
	Exception(char* message, int errnum = -1, const char* = nullptr, int line = -1)
	{
		strcpy(this->message, message);
		//log this shit
	}


	Exception(const char* message, const  char* file, const char* function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n Error code: %d\n", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	Exception(const Exception &e)
	{
		strcpy(message, e.message);
	}

	Exception(const char* message, const char* file, const char* function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n", file, function, line, message);
		this->errCode = errCode;
	}

	Exception(const char* message)
	{
		strcpy(this->message, message);
	}

	Exception(const char* message, long errCode)
	{
		strcpy(this->message, message);
		this->errCode = errCode;
	}

	//Exception()
	//{
	//	memset(message, 0, sizeof(message));
	//}

	virtual const char* what()
	{
		return message;
	}
};
