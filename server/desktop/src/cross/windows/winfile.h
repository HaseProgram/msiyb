#ifndef __WINFILE__H_
#define __WINFILE__H_
#ifdef _WIN32
#include <windows.h>
#include "../ifile.h"
#include <wchar.h>
#include "../../tools/exception.h"
#define DEFAULT_BLOCK_SIZE 128
#define MINBUFFERSIZE 128


static void ctow(const char *str, wchar_t *wStr)
{
	int len = strlen(str) + 2;
	mbstowcs(wStr, str, len);
}

class WindowsFile : public IFile
{
	HANDLE _hFile;
	char *_fileName;
	wchar_t *_wFileName;
	bool opened;

	/////////////////////////////////////////
	static void ParseMode(PDWORD creationDisposition, PDWORD desiredAccess, FileMode mode)
	{
		*creationDisposition = OPEN_EXISTING;
		if (mode == FileMode::OpenRead) *desiredAccess = GENERIC_READ;
		if (mode == FileMode::OpenWrite) *desiredAccess = GENERIC_WRITE;
		if (mode == FileMode::OpenWriteIfExist)
		{
			*desiredAccess = GENERIC_WRITE | GENERIC_READ;
			*creationDisposition = CREATE_ALWAYS;
		}
	}

	static HANDLE OpenFile_(const char *fileName, FileMode mode)
	{
		DWORD creationDisposition;
		DWORD desiredAccess;
		ParseMode(&creationDisposition, &desiredAccess, mode);

		wchar_t wFileName[MAX_PATH];// = new wchar_t[MAX_PATH];
		//if (!wFileName)
		//	ThrowException("Erroe alloc memory wFileName");
		ctow(fileName, wFileName);

		HANDLE hFile = CreateFile(wFileName, desiredAccess, FILE_SHARE_READ, NULL,
			creationDisposition,
			FILE_ATTRIBUTE_NORMAL, // normal file
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			ThrowFileException("Cant open File", GetLastError());

		//printf("%D", NUMBER_OF_PROCESSORS);

		return hFile;
	}

	static unsigned long long FileSize_(HANDLE hFile)
	{
		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(hFile, &fileSize))
			ThrowFileException("Cant get File size", GetLastError());
		return fileSize.QuadPart;
	}

	int ReadByte_()
	{
		char readedByte;
		int readed;
		BOOL res = ReadFile(_hFile, (LPVOID)&readedByte, 1, (LPDWORD)&readed, NULL);
		if (!res)
			ThrowFileException("Error ReadFile GetLatError:", GetLastError());

		if (readed == 0) return 0;
		return (int)readedByte;
	}

	//int ReadBlock_()
	//{
	//	int readedBytes;
	//	BOOL res = ReadFile(_hFile, (LPVOID)cacheBuffer, MINBUFFERSIZE, (LPDWORD)&readedBytes, NULL);
	//	if (!res)
	//		ThrowFileException("Error ReadFile, GetLatError:", GetLastError());

	//	if (readedBytes < sizeCacheBuffer) sizeCacheBuffer = readedBytes;
	//	return readedBytes;
	//}

	//
	static unsigned long long ReadBlock_(HANDLE hFile, byte* buf, unsigned long long sizeBuf = MINBUFFERSIZE)
	{
		unsigned long long readedBytes = 1;
		unsigned long long readedT = 0;
		BOOL res = 1;
		int bufEmpty = 1;

		while (res && readedBytes && bufEmpty)
		{
			res = ReadFile(hFile, (LPVOID)(buf + readedT), sizeBuf - readedT, (LPDWORD)&readedBytes, NULL);
			readedT += readedBytes;
			if (readedT == sizeBuf) bufEmpty = 0;
		}
		if (!res)
			ThrowFileException("Error ReadFile, GetLatError:", GetLastError());
		return readedT;
	}

	static void WriteBlock_(HANDLE hFile, byte *buf, unsigned long long size)
	{
		unsigned long long writed = 0;
		unsigned long long writedT = 0;
		BOOL res = 1;
		while (res && size)
		{
			size -= writed;
			writedT += writed;
			WriteFile(hFile, (LPVOID)(buf + writedT), size - writedT, (LPDWORD)&writed, NULL);
		}
		if (!res)
			ThrowFileException("Error InvalidSetFilePointer, GetLastError:", GetLastError());
	}

	unsigned long long SetFilePointer_(unsigned long long pos, MoveMethod move)
	{
		LARGE_INTEGER ps, FilePos;
		ps.QuadPart = (unsigned long long)pos;

		long res = SetFilePointerEx(_hFile, ps, (PLARGE_INTEGER)&FilePos, move);
		if (res == INVALID_SET_FILE_POINTER)
			ThrowFileException("Error WiteFile, GetLatError:", GetLastError());

		return FilePos.QuadPart;
	}

	static void Close(HANDLE hFile)
	{
		HANDLE h = hFile;
		if (!CloseHandle(hFile))
			ThrowFileException("Error WiteFile, GetLatError:", GetLastError());
	}

public:
	static DWORD GetFileType_(HANDLE hFile)
	{
		DWORD res = GetFileType(hFile);
		return res;
	}

	static void GetFileInfo_(HANDLE hFile)
	{
		/*LPVOID *FileStruct;
		DWORD type = GetFileType(hFile);
		if (type == 0)
			FileStruct = (LPVOID*)(PFILE_BASIC_INFO)malloc(sizeof(FILE_BASIC_INFO));
		if (type == )
		GetFileInformationByHandleEx(hFile, type, );*/
	}

	static void GetFileSecuty_()
	{

	}
public:
	WindowsFile() : _fileName(NULL), _wFileName(NULL), sizeCacheBuffer(MINBUFFERSIZE), cacheBufferPos(0) {}

	WindowsFile(const char *fileName, FileMode mode)
	{
		_fileName = new char[MAX_PATH];
		if (!_fileName)
			ThrowException("Cant alloc memory for fileName");

		_wFileName = new wchar_t[MAX_PATH];
		if (!_wFileName)
			ThrowException("Cant Alloc memory wchar_t");

		strcpy(_fileName, fileName);
		_fileName[strlen(fileName) - 1] = '\0';
		ctow(fileName, _wFileName);
		opened = 0;
	}

	~WindowsFile()
	{
		if (opened) Close();
		if (_fileName) delete _fileName;
		if (_wFileName) delete _wFileName;
	}

	static char* GetFileInfo()
	{

	}
	
	static char* LastModified(const wchar_t *wFileName)
	{
		char *charTime = new char[40];
		if (!charTime) ThrowException("Cant alloc for charTi");
		WIN32_FIND_DATA FindFileData;
		HANDLE handle = FindFirstFile(wFileName, &FindFileData);
		if (handle == INVALID_HANDLE_VALUE)
			ThrowFileException("Cant find file", GetLastError());
		//FILETIME time = FindFileData.ftLastWriteTime;
		SYSTEMTIME sysTime;
		FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &sysTime);
		sprintf(charTime, "%d %d %d", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
		return charTime;
	}


	void Delete()
	{
		Delete(_wFileName);
	}

	static void Delete(const wchar_t *wFileName)
	{
		if (!DeleteFile(wFileName))
			ThrowFileException("Error Delete File", GetLastError());
	}

	static void Delete(const char *fileName)
	{
		wchar_t wFileName[MAX_PATH];
		mbstowcs(wFileName, fileName, strlen(fileName) + 1);
		//if (!wFileName) ThrowException("Cant alloc memory!");
		Delete(wFileName);
		//delete[] wFileName;
	}

	static char* LastModified(const char *fileName)
	{

	}


	bool Exist()
	{
		return Exist(_wFileName);
	}


	static bool Exist(const char *fileName)
	{
		wchar_t wFileName[MAX_PATH];
		//if (!wFileName) ThrowException("Cant alloc memory");
		mbstowcs(wFileName, fileName, strlen(fileName) + 1);
		bool res = Exist(wFileName);
		///delete[] wFileName;
		return res;
	}

	static bool Exist(const wchar_t *wFileName)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE handle = FindFirstFile(wFileName, &FindFileData);
		int found = handle != INVALID_HANDLE_VALUE;
		if (found)
			FindClose(handle);
		return found;
	}

	void Open(const char *fileName, FileMode mode)
	{
			_hFile = OpenFile_(fileName, mode);
			if (!_fileName) _fileName = new char[MAX_PATH];
			if (!_fileName) ThrowException("Cant alloc memory fileName");
			if (!_wFileName) _wFileName = new wchar_t[MAX_PATH];
			if (!_fileName) ThrowException("Cant alloc memoru wFileName");
			strcpy(_fileName, fileName);
			ctow(fileName, _wFileName);
			opened = 1;
		//	return FileState::FileOk;
		/*}
		catch (FileException &e)
		{
			if (e.GetErrorCode() == ERROR_FILE_NOT_FOUND)
			{
				return FileState::FileNotFound;
			}
			if (e.GetErrorCode() == ERROR_FILE_EXISTS)
			{
				return FileState::FileCantWrite;
			}
			FileException nE = e;
			throw nE;
		}*/
	}

	void Close() { Close(_hFile); }

	void Rename(const char *newFileName)
	{
		wchar_t wNewFileName[MAX_PATH];
		ctow(newFileName, wNewFileName);

		if (!MoveFileEx(_wFileName, wNewFileName, MOVEFILE_COPY_ALLOWED))
			ThrowFileException("Cant Rename, GetLastError:", GetLastError());

		lstrcpyW(_wFileName, wNewFileName);
		strcpy(_fileName, newFileName);
	}


	// TODO IF WE KNOW THAT WE WORKS WITH WINDOWS
	void Rename(const wchar_t *wFileName)
	{

	}


	unsigned long long Seek()
	{	return SetFilePointer_(0, MoveMethod::Current);	}

	unsigned long long FileSize() 
	{return FileSize_(_hFile); }

	void SetOffset(unsigned long long distance, MoveMethod move) 
	{ SetFilePointer_(distance, move);	}


	unsigned long long ReadFromFile(byte* byteArr, unsigned long long size)
	{	return ReadBlock_(_hFile,  byteArr, size);	}

	void WriteToFile(byte *data, unsigned long long size)
	{	WriteBlock_(_hFile, data, size);	}

	void WriteByte(byte b)
	{	WriteBlock_(_hFile, &b, 1);	}

	///////////////
	// Static
	static unsigned long long ReadAllBytes(byte **block, const char *fileName)
	{
		HANDLE hFile = OpenFile_(fileName, FileMode::OpenRead);
		unsigned long long fSize = FileSize(fileName);
		byte *blockArr = new byte[fSize];
		if (!blockArr)
			ThrowException("Cant Aloc memory for block arr");
		
		ReadBlock_(hFile, blockArr, fSize);
		*block = blockArr;
		return fSize;
	}
	
	static void WriteAllBytes(byte* data, unsigned long long size, const char* fileName, FileMode mode = OpenWrite)
	{
		HANDLE hFile = OpenFile_(fileName, mode);
		WriteBlock_(hFile, data, size);
		Close(hFile);
	}

	static unsigned long long FileSize(const char *fileName)
	{
		HANDLE _hFile = OpenFile_(fileName, FileMode::OpenRead);
		unsigned long long size = FileSize_(_hFile);
		return size;
	}
};
#endif
#endif