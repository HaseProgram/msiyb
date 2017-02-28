#include "stdafx.h"
#include "../../stdafx.h"
#include "file.h"

File::File()
{
	this->meta = new FileMeta;
	this->inuse = false;
}

File::File(char* filename)
{
	this->meta = new FileMeta;
	this->meta->filename = filename;
	inuse = false;
}

File::~File()
{
	delete this->meta;
}

bool File::Exists()
{
	if (!this->meta->filename)
	{
		throw Exception();
	}
	return this->Exists(this->meta->filename);
}

bool File::Exists(const char* filename)
{
	wchar_t w_filename[_MAX_PATH];
	mbstowcs(w_filename, filename, strlen(filename) + 1);
	return File::Exists(w_filename);
}

bool File::Exists(const wchar_t* w_filename)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(w_filename, &FindFileData);
	bool found = (handle != INVALID_HANDLE_VALUE);
	if (found)
	{
		FindClose(handle);
	}
	return found;
}

void File::Open(FileOpenMode mode)
{
	if (!this->meta->filename)
	{
		throw Exception("Trying to open file while no file name specified!", GetLastError());
	}
	hfile = File::Open(this->meta->filename, mode);
	this->inuse = true;
}

HANDLE File::Open(const char* filename, FileOpenMode mode)
{
	wchar_t w_filename[_MAX_PATH];
	mbstowcs(w_filename, filename, strlen(filename) + 1);

	return File::Open(w_filename, mode);
}

HANDLE File::Open(const wchar_t* w_filename, FileOpenMode mode)
{
	DWORD creationDisposition = OPEN_EXISTING;
	DWORD desiredAccess;
	switch (mode)
	{
	case FileOpenMode::READ:
		desiredAccess = GENERIC_READ;
		break;
	case FileOpenMode::WRITE:
		desiredAccess = GENERIC_WRITE;
		break;
	case FileOpenMode::WRITEONEXISTS:
		desiredAccess = GENERIC_WRITE | GENERIC_READ;
		creationDisposition = CREATE_ALWAYS;
		break;
	}

	HANDLE hfile = CreateFile(w_filename, desiredAccess, FILE_SHARE_READ, NULL,
		creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		throw Exception("Error occured while opening file.", GetLastError());
	}
	return hfile;
}

void File::Close()
{
	File::Close(this->hfile);
}

void File::Close(HANDLE hfile)
{
	if (!CloseHandle(hfile))
	{
		throw Exception("Error occured while closing file.", GetLastError());
	}
}
