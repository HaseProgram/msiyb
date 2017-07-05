#pragma once

#include "../exception.h"

#ifdef DEBUG
#define ThrowMutexException(msg)											\
{																			\
	throw MutexException(msg, __FILE__, __FUNCTION__, __LINE__);			\
}

#define ThrowMutexExceptionWithCode(msg, code)								\
{																			\
	throw MutexException(msg, __FILE__, __FUNCTION__, __LINE__, code);		\
}
#else
#define ThrowMutexException(msg)											\
{																			\
	throw MutexException(msg);												\
}																	
#define void ThrowMutexExceptionWithCode(msg, code)							\
{																			\
	throw MutexException(msg, code);										\
}
#endif

class MutexException : public Exception
{
public:
	MutexException(const char* message, const char *file, const char *function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n Error code: %\n", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	MutexException(const char* message, const char *file, const char *function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n", file, function, line, message);
		this->errCode = -1;
	}

	const char* what() const
	{
		return message;
	}

	void operator = (const MutexException &fE)
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
