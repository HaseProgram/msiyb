#include "stdafx.h"
#include "../../stdafx.h"
#include "windir.h"
#include "../../common/thread.h"
#include "../../common/stringmethods.h"

WinDir::WinDir()
{
	strcpy(this->workingDir, "/");
	ctow(this->workingDir, this->wWorkingDir);
}

WinDir::WinDir(const char *path)
{
	strcpy(this->workingDir, path);
	ctow(this->workingDir, this->wWorkingDir);
}

size_lt WinDir::GetDirSize()
{
	return this->GetDirSize(this->wWorkingDir);
}

size_lt WinDir::GetDirSize(const char *path)
{
	wchar_t wPath[PATH_MAX];
	ctow(path, wPath);
	return WinDir::GetDirSize(wPath);
}

size_lt WinDir::GetDirSize(const wchar_t *wPath)
{
	WIN32_FIND_DATA fdFindData;
	HANDLE hFind;
	size_lt size = 0;

	size_lt (*GDS)(const wchar_t *) = WinDir::GetDirSize;

	hFind = FindFirstFile(wPath, &fdFindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		ThrowDirExceptionWithCode("Can not find first file!", GetLastError());
	}

	vector<size_lt> results;
	int resultCount = 0;

	Thread* thr = new Thread();
	do
	{
		if (lstrcmp(fdFindData.cFileName, TEXT(".")) == 0 || lstrcmp(fdFindData.cFileName, TEXT("..")) == 0)
		{
			continue;
		}
		
		LPWSTR filePath;
		wcscpy(filePath, wPath);
		lstrcatW(filePath, fdFindData.cFileName);

		if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			lstrcatW(filePath, TEXT("\\"));
			
			size_lt res = 0;
			Thread::CheckListCompleted();
			if (!thr || thr->Start(GDS, filePath, &res) < 0)
			{
				size += WinDir::GetDirSize(filePath);
			}
		}
		else
		{
			char path[PATH_MAX];
			wcstombs(path, wPath, (size_t)PATH_MAX);
			size += File::FileSize(path);
		}
	} while (FindNextFile(hFind, &fdFindData));
	thr->WaitToComplete();
	delete thr;
	FindClose(hFind);
	return size;
}

