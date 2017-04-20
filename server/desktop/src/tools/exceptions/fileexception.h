#pragma once

#include "../exception.h"
#ifdef DEBUG
#define ThrowFileException(msg)											\
{																		\
	throw FileException(msg, __FILE__, __FUNCTION__, __LINE__);			\
}

#define ThrowFileExceptionWithCode(msg, code)							\
{																		\
	throw FileException(msg, __FILE__, __FUNCTION__, __LINE__, code);	\
}
#else
#define ThrowFileException(msg)											\
{																		\
	throw FileException(msg);											\
}																	
#define void ThrowFileExceptionWithCode(msg, code)						\
{																		\
	throw FileException(msg, code);										\
}
#endif

class FileException : public Exception
{
public:
	FileException() {}
	FileException(const char* message, const char *file, const char *function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n Error code: %d\n", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	FileException(const char* message, const char *file, const char *function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n", file, function, line, message);
		this->errCode = -1;
	}

	const char* what() const
	{
		return message;
	}

	void operator = (const FileException &fE)
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
