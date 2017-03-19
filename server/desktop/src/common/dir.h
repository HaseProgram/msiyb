#pragma once

#ifdef _WIN32
#include "../cross/windows/windir.h"
typedef WinDir OSDir;
#elif _unix_
#include "../cross/unix/unixdir.h"
typedef UnixDir OSDir;
#endif

using namespace std;

class Dir
{
public:
	char workingDir[PATH_MAX];

	Dir();
	Dir(const char* path);
	~Dir();

	void SetWorkingDir(const char *path);

	size_lt GetDirSize();
	size_lt static GetDirSize(const char *path);

	size_lt GetNumberOfFiles();
	static size_lt GetNumberOfFiles(const char *path);

	File& SearchFileName(const char *fileName);
	static File& SearchFileName(const char *path, const char *fileName);

	vector<File&> SearchFileExt(const char* fileExtention);
	static vector<File&> SearchFileExt(const char *path, const char* fileExtention);
	
	vector<File&> SearchFileTime(time_t fileTime);
	static vector<File&> SearchFileTime(const char *path, time_t fileTime);

	void MakeDir();
	static void MakeDir(const char *path);

	bool Exists();
	static bool Exists(const char *path);

private:
	IDir* dir;
};