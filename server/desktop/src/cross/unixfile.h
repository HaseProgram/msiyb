#ifndef UNIXFILE_H
#define UNIXFILE_H
#ifdef __unix__
#include "Ifile.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <unistd.h>

typedef int HANDLE;

class UnixFile: public IFile
{
    int _hFile;
    char *_fileName;
    byte *cacheBuffer;
    int sizeCacheBuffer;
    int cacheBufferPos;
    bool opened;
    struct stat fStat;

    /////////////////////////////////////////
    static void ParseMode(int *flags, mode_t *mode, FileMode fMode)
    {
        *mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        *flags = O_RDONLY; // open read
        if (fMode == FileMode::OpenWrite) *flags = O_RDWR;
        if (fMode == FileMode::OpenWriteIfExist) *flags = O_RDWR | O_CREAT;
    }

    static void ParseAcess()
    {

    }

    static HANDLE OpenFile_(const char *fileName, FileMode fMode)
    {
        int flags;
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        ParseMode(&flags, &mode, fMode);

        HANDLE hFile = open(fileName, flags, mode);
        if (hFile == -1)
            ThrowFileException("Error: open , errno ", errno);

        return hFile;
    }

    static unsigned long long ReadBlock_(HANDLE hFile,  byte* buf, unsigned long long sizeBuf = MIN_BUFFER_SIZE)
    {
        unsigned long long readedT = 0;
        long long readed = 1;

        while (readed > 0)
        {
            readed = read(hFile, buf + readedT, sizeBuf - readedT);
            readedT += readed;
        }

        if (readed == -1)
            ThrowFileException("Error: read, errno :", errno);
        return readedT;
    }

    static void WriteBlock_(HANDLE hFile,  byte *buf, unsigned long long size)
    {
         long long writed = 1;
        unsigned long long writedT = 0;

        while (writed > 0 && size)
        {
            writed = write(hFile, buf + writedT, size);
            size -= writed;
            writedT += writed;
        }
        if (writed == -1)
            ThrowFileException("Error write, errno: ", errno);
    }

    unsigned long long SetFilePointer(HANDLE hFile, unsigned long long pos, MoveMethod move)
    {

    }

    unsigned long long SetFilePointer_(unsigned long long pos, MoveMethod move)
    {

//        int err = lseek()
//        LARGE_INTEGER ps, FilePos;
//        ps.QuadPart = (unsigned long long)pos;
//        long res = SetFilePointerEx(_hFile, ps, (PLARGE_INTEGER)&FilePos, move);
//        if (res == INVALID_SET_FILE_POINTER)
//        {
//            char message[128];
//            sprintf(message, "%s %d", "Error InvalidSetFilePointer, GetLastError: ", GetLastError());
//            ThrowException(message);
//        }
//        return FilePos.QuadPart;
    }

    static void Close(HANDLE hFile)
    {
        if (close(hFile) == -1)
            ThrowExceptionWithCode("Error close, errno: ", errno);
    }

/*    public:
    static DWORD GetFileType_(HANDLE hFile)
    {
        DWORD res = GetFileType(hFile);
        return res;
    }
*/
    static void GetFileInfo_(HANDLE hFile)
    {
        /*LPVOID *FileStruct;
        DWORD type = GetFileType(hFile);
        if (type == 0)
            FileStruct = (LPVOID*)(PFILE_BASIC_INFO)malloc(sizeof(FILE_BASIC_INFO));
        if (type == )
        GetFileInformationByHandleEx(hFile, type, );*/
    }
public:
    UnixFile() : _fileName(NULL), sizeCacheBuffer(MIN_BUFFER_SIZE), cacheBufferPos(0) {}

    UnixFile(const char *fileName, FileMode mode = OpenRead)
    {
        try
        {
            _fileName = new char[PATH_MAX];
            if (!_fileName)
                ThrowException("Cant alloc memory for fileName");

            sizeCacheBuffer = MIN_BUFFER_SIZE;
            cacheBuffer = new byte[MIN_BUFFER_SIZE];
            cacheBufferPos = 0;

            strcpy(_fileName, fileName);
            _fileName[strlen(fileName) - 1] = '\0';
            opened = 0;
            //_hFile = OpenFile_(_fileName, mode);
        }
        catch (Exception &e)
        {
            throw e;
        }
    }

    ~UnixFile()
    {
        if (opened) Close();
        if (_fileName) delete _fileName;
        if (cacheBuffer) delete cacheBuffer;
    }

    static char* GetFileInfo()
    {

    }



    static bool Exist(const char *fileName)
    {
        struct stat st;
        int result = stat(fileName, &st);
        return result == 0;
    }

    bool Exist()
    {
        return Exist(_fileName);
    }

    void Delete()
    {
        Delete(_fileName);
    }

    static void Delete(const char *fileName)
    {
        if(remove(fileName) == -1)
            ThrowFileException("error remove, errno", errno);
    }


    void Open(const char *fileName, FileMode mode)
    {
        //try
        //{
			_hFile = OpenFile_(fileName, mode);
			if (!_fileName) _fileName = new char[PATH_MAX];
			if (!_fileName) ThrowException("Cant alloc memory fileName");
			strcpy(_fileName, fileName);
			opened = 1;
//			return FileState::FileOk;
        //}
        //catch (FileException &e)
        //{

        //}
    }

    void Close()
    {
        Close(_hFile);
    }

    static void Rename(const char *oldFileName, const char *newFileName)
    {
        if (rename(oldFileName, newFileName) == -1)
            ThrowFileException("Error rename, errno :", errno);
    }

    void Rename(const char *newFileName)
    {
        Rename(_fileName, newFileName);
        strcpy(_fileName, newFileName);
    }






    unsigned long long Seek()
    {
      //  unsigned long long FilePos = SetFilePosition(0, MoveMethod::Current);
        return 0;//FilePos;
    }

    unsigned long long FileSize()
    {
        return FileSize(this->_fileName);
    }

    void SetOffset(unsigned long long distance, MoveMethod move)
    {
    //    SetFilePointer_(distance, move);
    }




    int ReadByte()
    {
        byte c;
        ReadBlock_(_hFile, &c, 1);
        return (int)c;
    }

    unsigned long long ReadFromFile(byte* byteArr, unsigned long long size)
    {
        return ReadBlock_(_hFile,  byteArr, size);
    }



    void WriteToFile(byte *data, unsigned long long size)
    {
        WriteBlock_(_hFile, data, size);
    }

    void WriteByte(byte b)
    {
        WriteBlock_(_hFile, &b, 1);
    }

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
    }

    static unsigned long long FileSize(const char *fileName)
    {
        struct stat st;
        int err = stat(fileName, &st);
        if (err == -1)
            ThrowFileException("Error stat, errno", errno);
        return st.st_size;
    }
};

#endif
#endif // UNIXFILE_H

/*
    int ReadByte_()
    {
        char readedByte;
        int readed;
        BOOL res = ReadFile(_hFile, (LPVOID)&readedByte, 1, (LPDWORD)&readed, NULL);
        if (!res)
        {
            char message[128];
            sprintf(message, "%s %d", "Error: GetLatError:", GetLastError());
            ThrowException(message);
        }
        if (readed == 0) return 0;
        return (int)readedByte;
    }

    int ReadBlock_()
    {
        int readedBytes;
        BOOL res = ReadFile(_hFile, (LPVOID)cacheBuffer, MIN_BUFFER_SIZE, (LPDWORD)&readedBytes, NULL);
        if (!res)
        {
            char message[128];
            sprintf(message, "%s %d", "Error: GetLatError:", GetLastError());
            ThrowException(message);
        }
        if (readedBytes < sizeCacheBuffer) sizeCacheBuffer = readedBytes;
        return readedBytes;
    }

    */
    //
