#include "stdafx.h"
#include "../stdafx.h"
#include "dir.h"

Dir::Dir()
{
	this->dir = new OSDir();
	strcpy(this->workingDir, "/");
}

Dir::Dir(const char* path)
{
	this->dir = new OSDir();
	strcpy(this->workingDir, path);
}

Dir::~Dir()
{
	delete this->dir;
	delete[] this->workingDir;
}

void Dir::SetWorkingDir(const char* path)
{
	strcpy(this->workingDir, path);
}

long long Dir::GetDirSize()
{
	return 123414134124;
}

long long Dir::GetNumberOfFIles()
{
	return 3141241241234;
}

File& Dir::SearchFileName(const char* filename)
{
	return File();
}

//vector<File&> Dir::SearchFileExtention(const char * filextention)
//{
//	return vector<File&>();
//}
//
//vector<File&> Dir::SearchFileDate(time_t filedate)
//{
//	return vector<File&>();
//}

void Dir::MakeDir(const char * path)
{
}

bool Dir::DirExists(const char * path)
{
	return false;
}

bool Dir::IsDir(const char * path)
{
	return false;
}

