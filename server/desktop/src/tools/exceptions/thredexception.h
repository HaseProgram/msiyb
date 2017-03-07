#pragma once

#include "../exception.h"

class ThreadException : public Exception
{
	char message[1024];
	long errCode;
public:
	ThreadException(const char* message, const char *file, const char *function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message:\n %s\n Error code: %d", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	ThreadException(const char* message, const char *file, const char *function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message:\n %s", file, function, line, message);
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

#ifdef DEBUG
inline void ThrowThreadException(const char *msg)
{
	throw ThreadException(msg, __FILE__, __FUNCTION__, __LINE__);
}

inline void ThrowThreadExceptionWithCode(const char* msg, long code)
{
	throw Exception(msg, __FILE__, __FUNCTION__, __LINE__, code);
}
#elif
inline void ThrowThreadException(const char* msg)
{
	throw ThreadException(msg);
}
inline void ThrowThreadExceptionWithCode(const char* msg, long code)
{
	throw ThreadException(msg, code);
}
#endif
