#pragma once
#include "file.h"

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

	void SetWorkingDir(const char* path);
	long long GetDirSize();
	long long GetNumberOfFIles();
	File& SearchFileName(const char* filename);
	//vector<File&> SearchFileExtention(const char* filextention);
	//vector<File&> SearchFileDate(time_t filedate);
	static void MakeDir(const char* path);
	static bool DirExists(const char* path);
	static bool IsDir(const char* path);

private:
	IDir* dir;
};