#include "stdafx.h"
#include "../../stdafx.h"
#include "fileimpl.h"

FileImpl::FileImpl()
{
	this->meta = new FileMeta;
	this->inuse = false;
}

FileImpl::FileImpl(char* filename)
{
	this->meta = new FileMeta;
	this->meta->filename = filename;
	inuse = false;
}

FileImpl::~FileImpl()
{
	delete this->meta;
}

bool FileImpl::Exists()
{
	if (!this->meta->filename)
	{
		throw Exception();
	}
	return this->Exists(this->meta->filename);
}

bool FileImpl::Exists(const char* filename)
{
	wchar_t w_filename[_MAX_PATH];
	mbstowcs(w_filename, filename, strlen(filename) + 1);
	return FileImpl::Exists(w_filename);
}

bool FileImpl::Exists(const wchar_t* w_filename)
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

void FileImpl::Open(FileOpenMode mode)
{
	if (!this->meta->filename)
	{
		throw Exception("Trying to open file while no file name specified!", GetLastError());
	}
	hfile = FileImpl::Open(this->meta->filename, mode);
	this->inuse = true;
}

HANDLE FileImpl::Open(const char* filename, FileOpenMode mode)
{
	wchar_t w_filename[_MAX_PATH];
	mbstowcs(w_filename, filename, strlen(filename) + 1);

	return FileImpl::Open(w_filename, mode);
}

HANDLE FileImpl::Open(const wchar_t* w_filename, FileOpenMode mode)
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

void FileImpl::Close()
{
	FileImpl::Close(this->hfile);
}

void FileImpl::Close(HANDLE hfile)
{
	if (!CloseHandle(hfile))
	{
		throw Exception("Error occured while closing file.", GetLastError());
	}
}
