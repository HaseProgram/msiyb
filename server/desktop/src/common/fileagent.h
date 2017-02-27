#pragma once

struct FileInfo
{
	char* filename;
	char* filepath;
	char* filetype;
	time_t creationdate;
};

#ifdef _WIN32
#include "../cross/windows/file.h"
#elif _unix_
#include "../cross/unix/file.h"
#endif

using namespace std;

class FileAgent
{
public:
	FileInfo* info;

	FileAgent();
	FileAgent(char* filename);
	~FileAgent();

private:
	IFile* file;

};