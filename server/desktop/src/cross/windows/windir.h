#pragma once

#include "../idir.h"

class WinDir : public IDir
{
public:
	char workingDir[PATH_MAX];
	wchar_t wWorkingDir[PATH_MAX];

	WinDir();
	WinDir(const char *path);
	~WinDir();

	virtual size_lt GetDirSize() override;
	static size_lt GetDirSize(const char *path);
	static size_lt GetDirSize(const wchar_t *wPath);

	virtual size_lt GetNumberOfFiles() override;
	static size_lt GetNumberOfFiles(const char *path);
	static size_lt GetNumberOfFiles(const wchar_t *wPath); 

	virtual File& SearchFileName(const char *fileName) override;
	static File& SearchFileName(const char *path, const char *fileName);

	virtual vector<File&> SearchFileExt(const char* fileExtention) override;
	static vector<File&> SearchFileExt(const char *path, const char* fileExtention);

	virtual vector<File&> SearchFileTime(time_t fileTime) override;
	static vector<File&> SearchFileTime(const char *path, time_t fileTime);

	virtual void MakeDir() override;
	static void MakeDir(const char *path);

	virtual bool Exists() override;
	static bool Exists(const char *path);

};
