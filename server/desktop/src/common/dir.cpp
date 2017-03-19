#include "stdafx.h"
#include "../stdafx.h"
#include "dir.h"

Dir::Dir()
{
	this->dir = new OSDir();
	if(!this->dir)
	{
		ThrowException("Can't allocate memory!");
	}
	strcpy(this->workingDir, "/");
}

Dir::Dir(const char *path)
{
	this->dir = new OSDir(path);
	if(!this->dir)
	{
		ThrowException("Can't allocate memory!");
	}
	strcpy(this->workingDir, path);
}

Dir::~Dir()
{
	delete this->dir;
	delete[] &this->workingDir;
}

void Dir::SetWorkingDir(const char *path)
{
	strcpy(this->workingDir, path);
}

size_lt Dir::GetDirSize()
{
	return this->GetDirSize(this->workingDir);
}

size_lt Dir::GetDirSize(const char *path)
{
	return OSDir::GetDirSize(path);
}

size_lt Dir::GetNumberOfFiles()
{
	return this->GetNumberOfFiles(this->workingDir);
}

size_lt Dir::GetNumberOfFiles(const char *path)
{
	return OSDir::GetNumberOfFiles(path);
}

File& Dir::SearchFileName(const char *fileName)
{
	return this->SearchFileName(this->workingDir, fileName);
}

File& Dir::SearchFileName(const char *path, const char *fileName)
{
	return OSDir::SearchFileName(path, fileName);
}

vector<File&> Dir::SearchFileExt(const char * filextention)
{
	return this->SearchFileExt(this->workingDir, filextention);
}

vector<File&> Dir::SearchFileExt(const char *path, const char *fileExtention)
{
	return OSDir::SearchFileExt(path, fileExtention);
}

vector<File&> Dir::SearchFileTime(time_t fileTime)
{
	return this->SearchFileTime(this->workingDir, fileTime);
}

vector<File&> Dir::SearchFileTime(const char * path, time_t fileTime)
{
	return OSDir::SearchFileTime(path, fileTime);
}

void Dir::MakeDir()
{
	this->MakeDir(this->workingDir);
}

void Dir::MakeDir(const char *path)
{
	OSDir::MakeDir(path);
}

bool Dir::Exists()
{
	this->Exists(this->workingDir);
}

bool Dir::Exists(const char *path)
{
	return OSDir::Exists(path);
}
