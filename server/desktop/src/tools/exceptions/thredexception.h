#pragma once

#include "../exception.h"

#ifdef DEBUG
#define ThrowThreadException(msg)											\
{																			\
	throw ThreadException(msg, __FILE__, __FUNCTION__, __LINE__);			\
}

#define ThrowThreadExceptionWithCode(msg, code)								\
{																			\
	throw ThreadException(msg, __FILE__, __FUNCTION__, __LINE__, code);		\
}
#else
#define ThrowThreadException(msg)											\
{																			\
	throw ThreadException(msg);												\
}																	
#define void ThrowThreadExceptionWithCode(msg, code)						\
{																			\
	throw ThreadException(msg, code);										\
}
#endif

class ThreadException : public Exception
{
public:
	ThreadException(const char* message, const char *file, const char *function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n Error code: %\n", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	ThreadException(const char* message, const char *file, const char *function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n", file, function, line, message);
		this->errCode = -1;
	}

	const char* what() const
	{
		return message;
	}

	void operator = (const ThreadException &fE)
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
