#pragma once

#include "../tools/exceptions/direxception.h"
#include "../common/file.h"

class IDir
{
public:
	~IDir() {}
	virtual size_lt GetDirSize() = 0;
	virtual size_lt GetNumberOfFiles() = 0;
	virtual File& SearchFileName(const char *fileName) = 0;
	virtual vector<File&> SearchFileExt(const char *extention) = 0;
	virtual vector<File&> SearchFileTime(time_t modTime) = 0;
	virtual void MakeDir() = 0;
	virtual bool Exists() = 0;
};