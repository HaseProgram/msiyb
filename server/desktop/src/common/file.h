/*!
\file file.h "server\desktop\src\common\file.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once
//#include "Ifile.h"
//#include "exeption.h"
//#include "convert.h"
#define FILE_BUFFER_SIZE 1024
#ifdef _WIN32
#include "../cross/windows/winfile.h"
typedef WindowsFile OsFile;
#elif _unix_
typedef UnixFile OsFile;
#include "../cross/unix/fileimpl.h"
#endif

#include <iostream>


using namespace std;


void ResizeStringArray(char ***arrayStrings, int *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char **oldPtr = *arrayStrings;
	char **newPtr = new char*[newSize];//(char**)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string array");
	memcpy(newPtr, oldPtr, *oldSize * sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*arrayStrings = newPtr;
}

void ResizeString(char **string, int *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char *oldPtr = *string;
	char *newPtr = new char[newSize];// (char*)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string");
	memcpy(newPtr, oldPtr, *oldSize * sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

void Resize(char **string, int *oldSize, int newSize)
{
	char *oldPtr = *string;
	char *newPtr = new char[newSize];// (char*)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string");
	memcpy(newPtr, oldPtr, *oldSize);
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

class File : public IFile
{
private:
	IFile *file;					/*!< OS depended file structure */
	int buffersSize;				/// size of read and write cahce buffer
	byte *fileReadedBytesBuffer;	/// read cache buffer
	byte *fileWritedBytesBuffer;
	int posInReadedBuf;
	int posInWritedBuf;
	int readedBytes;
	int writedBytes;
	char fileName[MAX_PATH];

	//byte buf[FILE_BUFFER_SIZE];

	static int ReadStringFromBuffer(char **string, byte *buf, int *startPos, int sizeBuf);
	// TODO:
	inline void CheckAllocate(byte *arr);
public:
	File();
	~File();

	void Open(const char *fileName, FileMode mode);

	void Close();
	void Rename(const char *newFileName);
	bool Exist();
	void Delete();
	inline void CheckBuffer();
	int ReadByte();
	unsigned long long ReadFromFile(byte *block, unsigned long long sizeBlock);
	void WriteToFile(byte *block, unsigned long long sizeBlock);
	void WriteByte(byte b);
	unsigned long long Seek();
	void Fflush();
	unsigned long long FileSize();
	void SetOffset(unsigned long long distance, MoveMethod move);

	/*! static Methods */

	/*! Read all informaion from file */
	static int ReadAllBytes(const char *fileName, byte **byteArr);
	static int ReadAllCharStrings(const char *fileName, char*** charStrings);
	static int ReadAllStrings(const char *fileName, string** stringArray);

	/*! Get size of file in ull format */
	static unsigned long long FileSize(const char *fileName);
	static void Delete(const char *fileName);
	static bool Exist(const char *fileName);

	static void WriteAllBytes(const char *fileName, byte* data, int size, FileMode mode = OpenWriteIfExist);
	static void WriteAllCharStrings(const char *fileName, char** charStrings, int countStrings);
	static void WriteAllStrings(const char *fileName, string *strings, int countStrings);


	// TODO: i dont know what i need to return ...
	static char* LastModified(const char *fileName);
};


/*!
\class File file.h "server\desktop\src\common\file.h"
\brief  File interface
Provide interface to common file structure and it's action
Factory for unix/bsd/windows structure of file
*/
//class File
//{
//public:
//	/*!
//	Initialise OS depended file structure
//	*/
//	File();
//
//	/*!
//	Initialise OS depended file structure
//	Set full file name in File class meta structure
//	\param[in] filename Name of file with it's local path
//	*/
//	File(char* filename);
//
//	/*!
//	Dealocate OS depended File object
//	*/
//	~File();
//
//	/*!
//	Opens file with defined open rule
//	Possible values defined in /src/cross/ifile.h
//	\param[in] mode Set the rule opening file
//	*/
//	void Open(FileOpenMode mode);
//
//	/*!
//	Closes file
//	*/
//	void Close();
//
//private:
//	IFile* file;	/*!< OS depended file structure */
//
//};