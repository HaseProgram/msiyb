#include "winfile.h"
#include "../../common/stringmethods.h"

WinFile::WinFile()
{
	_fileName = new char[MAX_PATH];
	_tFileName = new TCHAR[MAX_PATH];
	_opened = false;
}

WinFile::WinFile(const char *fileName)
{
	_fileName = new char[MAX_PATH];
	if (!_fileName)
	{
		ThrowException("Can't allocate memory on fileName");
	}

	_tFileName = new TCHAR[MAX_PATH];
	if (!_tFileName)
	{
		ThrowException("Can't allocate memory on tFileName");
	}

	strcpy(_fileName, fileName);
	ConvertCharToTCHAR(fileName, _tFileName);
	_opened = false;
}

WinFile::~WinFile()
{
	if (_opened)
	{
		Close();
	}

	delete[] _fileName;
	delete[] _tFileName;
}

void WinFile::Open(FileOpenMode mode)
{
	ConvertCharToTCHAR(_fileName, _tFileName);
	_hFile = tOpen(_tFileName, mode);
	_opened = true;
}

void WinFile::Open(const char *fileName, FileOpenMode mode)
{
	strcpy(_fileName, fileName);
	ConvertCharToTCHAR(_fileName, _tFileName);
	_hFile = tOpen(_tFileName, mode);
	_opened = true;
}

HANDLE WinFile::tOpen(const TCHAR *fileName, FileOpenMode mode)
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
	case FileOpenMode::READWRITE:
		desiredAccess = GENERIC_WRITE | GENERIC_READ;
		creationDisposition = CREATE_ALWAYS;
		break;
	case FileOpenMode::WRITENEWFILE:
		desiredAccess = GENERIC_WRITE | GENERIC_READ;
		creationDisposition = CREATE_ALWAYS;
		break;
	}

	HANDLE hfile = CreateFile(fileName, desiredAccess, FILE_SHARE_READ, NULL,
		creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		ThrowFileExceptionWithCode("Error occured while opening file.", GetLastError());
	}
	return hfile;
}

void WinFile::Close()
{
	Close(_hFile);
	_opened = false;
}

void WinFile::Close(HANDLE hFile)
{
	FlushFileBuffers(hFile); //?
	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(hFile))
		{
			ThrowFileExceptionWithCode("Error occured while closing file!", GetLastError());
		}
	}
}

void WinFile::Rename(const char *newFileName)
{
	Rename(_fileName, newFileName);

	ConvertCharToTCHAR(newFileName, _tFileName);
	strcpy(_fileName, newFileName);
}

void WinFile::Rename(const char *fileName, const char * newFileName)
{
	TCHAR tFileName[MAX_PATH];
	TCHAR tNewFileName[MAX_PATH];
	ConvertCharToTCHAR(fileName, tFileName);
	ConvertCharToTCHAR(newFileName, tNewFileName);

	if (!MoveFileEx(tFileName, tNewFileName, MOVEFILE_COPY_ALLOWED))
	{
		ThrowFileExceptionWithCode("Can't rename file", GetLastError());
	}
}

bool WinFile::Exist()
{
	return tExist(_tFileName);
}

bool WinFile::Exist(const char *fileName)
{
	TCHAR tFileName[MAX_PATH];
	ConvertCharToTCHAR(fileName, tFileName);
	return WinFile::tExist(tFileName);
}

bool WinFile::tExist(const TCHAR *tFileName)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(tFileName, &FindFileData);
	bool found = (handle != INVALID_HANDLE_VALUE);
	if (found)
	{
		FindClose(handle);
	}
	return found;
}

void WinFile::Delete()
{
	if (_opened)
	{
		Close(_hFile);
	}
	tDelete(_tFileName);
}

void WinFile::Delete(const char *fileName)
{
	TCHAR tFileName[MAX_PATH];
	ConvertCharToTCHAR(fileName, tFileName);
	WinFile::tDelete(tFileName);
}

void WinFile::tDelete(const TCHAR *tFileName)
{
	if (!DeleteFile(tFileName))
	{
		ThrowFileExceptionWithCode("Can't delete file!", GetLastError());
	}
}

size_lt WinFile::FileSize()
{
	return FileSize(_hFile);
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
	TCHAR tFileName[MAX_PATH];
	ConvertCharToTCHAR(fileName, tFileName);
	HANDLE hFile = tOpen(tFileName, FileOpenMode::READONLY);
	size_lt size = FileSize(hFile);
	Close(hFile);
	return size;
}

size_lt WinFile::Seek(size_lt offset, SeekReference move)
{
	LARGE_INTEGER ps, FilePos;
	ps.QuadPart = (size_lt)offset;

	long res = SetFilePointerEx(_hFile, ps, (PLARGE_INTEGER)&FilePos, move);
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
	bool res = ReadFile(_hFile, (LPVOID)&b, 1, (LPDWORD)&readedByte, NULL);
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
	return ReadBlock(_hFile, block, blockSize);
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
	WriteBlock(&b, 1);;
}

void WinFile::WriteBlock(byte *block, size_lt sizeBlock)
{
	WriteBlock(_hFile, block, sizeBlock);
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
	TCHAR tFileName[MAX_PATH];
	ConvertCharToTCHAR(fileName, tFileName);

	HANDLE hFile = tOpen(tFileName, FileOpenMode::READONLY);
	size_lt fSize = FileSize(hFile);
	byte *blockT = new byte[fSize];
	if (!blockT)
	{
		ThrowFileExceptionWithCode("Cant allocate memory for block array", GetLastError());
	}

	fSize = WinFile::ReadBlock(hFile, blockT, fSize);
	*block = blockT;
	Close(hFile);
	return fSize;
}

void WinFile::WriteAllBytes(const char *fileName, byte * data, size_lt size, FileOpenMode mode)
{
	TCHAR tFileName[MAX_PATH];
	ConvertCharToTCHAR(fileName, tFileName);
	HANDLE hFile = tOpen(tFileName, mode);
	WinFile::WriteBlock(hFile, data, size);
	Close(hFile);
}

FileMeta WinFile::LastModified(const char * fileName)
{
	FileMeta meta;
	return meta;
}
