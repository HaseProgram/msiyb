#pragma once

#include "../exception.h"

#ifdef DEBUG
#define ThrowDirException(msg)											\
{																		\
	throw DirException(msg, __FILE__, __FUNCTION__, __LINE__);			\
}

#define ThrowDirExceptionWithCode(msg, code)							\
{																		\
	throw DirException(msg, __FILE__, __FUNCTION__, __LINE__, code);	\
}
#else
#define ThrowDirException(msg)											\
{																		\
	throw DirException(msg);											\
}																	
#define void ThrowDirExceptionWithCode(msg, code)						\
{																		\
	throw DirException(msg, code);										\
}
#endif

class DirException : public Exception
{
	char message[1024];
	long errCode;
public:
	DirException(const char* message, const char *file, const char *function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n Error code: %d\n", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	DirException(const char* message, const char *file, const char *function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n", file, function, line, message);
		this->errCode = -1;
	}

	const char* what() const
	{
		return message;
	}

	void operator = (const DirException &fE)
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
