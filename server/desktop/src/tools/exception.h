#pragma once
#include <exception>
#include "../stdafx.h"

#ifdef _WIN32 
#define LastError GetLastError()
#elif __unix__
#define LastError errno
#endif

#define ERROR_MSG_SIZE 1024
#define DEBUG


#ifdef DEBUG
inline void ThrowException(const char *msg)
{
	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__);
}

inline void ThrowExceptionWithCode(const char* msg, long code)
{
	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__, code);
}
#elif
inline void ThrowException(const char* msg)
{
	throw Exception(msg);
}
inline void ThrowExceptionWithCode(const char* msg, long code)
{
	throw Exception(msg, code);
}
#endif


#ifdef _WIN32
#include <Windows.h>
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
public:
	char message[ERROR_MSG_SIZE];
	long errCode;


	// TODO LOG Exceptions
	Exception(char* message, int errnum = -1, const char* = nullptr, int line = -1)
	{
		strcpy(this->message, message);
		//log this shit
	}


	Exception(const char* message, const  char* file, const char* function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message:\n %s\n Error code:", file, function, line, message, errCode);
		this->errCode = errCode;
	}


	Exception(const char* message, const char* file, const char* function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message:\n %s\n", file, function, line, message);
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

	Exception()
	{

	}

	virtual const char* what()
	{
		return this->message;
	}
};


