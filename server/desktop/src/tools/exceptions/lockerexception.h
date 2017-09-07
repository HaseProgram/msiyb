#pragma once

#include "../exception.h"

#ifdef DEBUG
#define ThrowLockerException(msg)											\
{																			\
	throw LockerException(msg, __FILE__, __FUNCTION__, __LINE__);			\
}

#define ThrowLockerExceptionWithCode(msg, code)								\
{																			\
	throw LockerException(msg, __FILE__, __FUNCTION__, __LINE__, code);		\
}
#else
#define ThrowLockerException(msg)											\
{																			\
	throw LockerException(msg);												\
}																	
#define void ThrowLockerExceptionWithCode(msg, code)							\
{																			\
	throw LockerException(msg, code);										\
}
#endif

class LockerException : public Exception
{
public:
	LockerException(const char* message, const char *file, const char *function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n Error code: %\n", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	LockerException(const char* message, const char *file, const char *function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n", file, function, line, message);
		this->errCode = -1;
	}

	const char* what() const
	{
		return message;
	}

	void operator = (const LockerException &fE)
	{
		strcpy(message, fE.what());
		errCode = fE.GetErrorCode();
	}

	long GetErrorCode() const
	{
		return errCode;
	}

	void SetErrorCode(long errCode)
	{
		this->errCode = errCode;
	}
};
