#include "winfile.h"
#include "../../common/stringmethods.h"

WinFile::WinFile()
{
	this->fileName = nullptr;
	this->wFileName = nullptr;
}

WinFile::WinFile(const char *fileName)
{
	this->fileName = new char[MAX_PATH];
	if (!this->fileName)
	{
		ThrowException("Can't allocate memory on fileName");
	}

	this->wFileName = new wchar_t[MAX_PATH];
	if (!this->wFileName)
	{
		ThrowException("Can't allocate memory on wFileName");
	}

	strcpy(this->fileName, fileName);
	ctow(this->fileName, this->wFileName);
	this->opened = false;
}

WinFile::~WinFile()
{
	if (this->opened)
	{
		this->Close();
	}
	
	if (this->fileName)
	{
		delete this->fileName;
	}

	if (this->wFileName)
	{
		delete this->wFileName;
	}
}

void WinFile::Open(FileOpenMode mode)
{
	this->hFile = this->Open(this->fileName, mode);
	this->opened = true;
}

HANDLE WinFile::Open(const char *fileName, FileOpenMode mode)
{
	DWORD creationDisposition = OPEN_EXISTING;
	DWORD desiredAccess;
	switch (mode)
	{
	case FileOpenMode::READONLY:
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

	wchar_t wFileName[MAX_PATH];
	ctow(fileName, wFileName);

	HANDLE hfile = CreateFile(wFileName, desiredAccess, FILE_SHARE_READ, NULL,
		creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		ThrowFileExceptionWithCode("Error occured while opening file.", GetLastError());
	}
	return hfile;
}

void WinFile::Close()
{
	this->Close(this->hFile);
	this->opened = false;
}

void WinFile::Close(HANDLE hFile)
{
	if (!CloseHandle(hFile))
	{
		ThrowFileExceptionWithCode("Error occured while closing file!", GetLastError());
	}
}

void WinFile::Rename(const char *newFileName)
{
	this->Rename(this->fileName, newFileName);

	ctow(newFileName, this->wFileName);
	strcpy(this->fileName, newFileName);
}

void WinFile::Rename(const char *fileName, const char * newFileName)
{
	wchar_t wFileName[MAX_PATH];
	ctow(fileName, wFileName);

	wchar_t wNewFileName[MAX_PATH];
	ctow(newFileName, wNewFileName);

	if (!MoveFileEx(wFileName, wNewFileName, MOVEFILE_COPY_ALLOWED))
	{
		ThrowFileExceptionWithCode("Can't rename file", GetLastError());
	}
}

bool WinFile::Exist()
{
	return this->Exist(this->wFileName);
}

bool WinFile::Exist(const char *fileName)
{
	wchar_t wFileName[MAX_PATH];
	mbstowcs(wFileName, fileName, strlen(fileName) + 1);
	return WinFile::Exist(wFileName);
}

bool WinFile::Exist(const wchar_t *wFileName)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(wFileName, &FindFileData);
	bool found = (handle != INVALID_HANDLE_VALUE);
	if (found)
	{
		FindClose(handle);
	}
	return found;
}

void WinFile::Delete()
{
	this->Delete(this->wFileName);
}

void WinFile::Delete(const char *fileName)
{
	wchar_t wFileName[MAX_PATH];
	mbstowcs(wFileName, fileName, strlen(fileName) + 1);
	WinFile::Delete(wFileName);
}

void WinFile::Delete(const wchar_t *wFileName)
{
	if (!DeleteFile(wFileName))
	{
		ThrowFileExceptionWithCode("Can't delete file!", GetLastError());
	}
}

size_lt WinFile::FileSize()
{
	return this->FileSize(this->hFile);
}

size_lt WinFile::FileSize(HANDLE hFile)
{
	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(hFile, &fileSize))
	{
		ThrowFileExceptionWithCode("Can't get file size!", GetLastError());
	}
	return fileSize.QuadPart;
}

size_lt WinFile::FileSize(const char *fileName)
{
	HANDLE hFile = WinFile::Open(fileName, FileOpenMode::READONLY);
	return WinFile::FileSize(hFile);
}

size_lt WinFile::Seek(size_lt offset, SeekReference move)
{
	LARGE_INTEGER ps, FilePos;
	ps.QuadPart = (size_lt)offset;

	long res = SetFilePointerEx(this->hFile, ps, (PLARGE_INTEGER)&FilePos, move);
	if (res == INVALID_SET_FILE_POINTER)
	{
		ThrowFileExceptionWithCode("Can't set pointer in file on new position!", GetLastError());
	}

	return FilePos.QuadPart;
}

int WinFile::ReadByte()
{
	char b;
	int readedByte;
	bool res = ReadFile(this->hFile, (LPVOID)&b, 1, (LPDWORD)&readedByte, NULL);
	if (!res)
	{
		ThrowFileExceptionWithCode("Can't read data from file!", GetLastError());
	}

	if (readedByte == 0)
	{
		return -1;
	}
	return (int)b;
}

size_lt WinFile::ReadBlock(byte *block, size_lt blockSize)
{
	return this->ReadBlock(this->hFile, block, blockSize);
}

size_lt WinFile::ReadBlock(HANDLE hFile, byte *block, size_lt blockSize)
{
	size_lt readedBytes = 1;
	bool res = ReadFile(hFile, (LPVOID)block, blockSize, (LPDWORD)&readedBytes, NULL);
	if (!res)
	{
		ThrowFileExceptionWithCode("Can't read data from file!", GetLastError());
	}
	return readedBytes;
}

void WinFile::WriteByte(byte b)
{
	this->WriteBlock(&b, 1);
}

void WinFile::WriteBlock(byte *block, size_lt sizeBlock)
{
	this->WriteBlock(this->hFile, block, sizeBlock);
}

void WinFile::WriteBlock(HANDLE hFile, byte * block, size_lt sizeBlock)
{
	size_lt writed = 0;
	bool res = WriteFile(hFile, (LPVOID)block, sizeBlock, (LPDWORD)&writed, NULL);
	if (!res)
	{
		ThrowFileExceptionWithCode("Can't write data into file!", GetLastError());
	}
}

size_lt WinFile::ReadAllBytes(const char *fileName, byte **block)
{
	HANDLE hFile = Open(fileName, FileOpenMode::READONLY);
	size_lt fSize = FileSize(fileName);
	byte *blockT = new byte[fSize];
	if (!blockT)
	{
		ThrowException("Cant Aloc memory for block arr");
	}

	fSize = WinFile::ReadBlock(hFile, blockT, fSize);
	*block = blockT;
	return fSize;
}

void WinFile::WriteAllBytes(const char *fileName, byte * data, size_lt size, FileOpenMode mode)
{
	HANDLE hFile = Open(fileName, mode);
	WinFile::WriteBlock(hFile, data, size);
	Close(hFile);
}

FileMeta WinFile::LastModified(const char * fileName)
{
	FileMeta meta;
	return meta;
}
