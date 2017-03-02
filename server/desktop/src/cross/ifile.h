/*!
\file ifile.h "server\desktop\src\cross\ifile.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once
#define MIN_BUFFER_SIZE 128
typedef unsigned char byte;
typedef enum MoveMethod_ { Start, Current, End } MoveMethod;
typedef enum FileMode_ { OpenRead, OpenWriteIfExist, OpenWrite, OpenReadWrite } FileMode;
#include <time.h>
//typedef enum ReturnRes_ { FileNotFound, FileOk, FileCantWrite } FileState;

/// File meta data structure
typedef struct
{
	char* filename;			///< File name with is's local path
	char* filepath;			///< Path to dir where is our file
	char* filetype;			///< File extention
	time_t creationdate;	///< File creation date
} FileMeta;

/// Opening file rule
typedef enum
{
	READ,			///< Open READ ONLY mode
	WRITEONEXISTS,	///< Write if file EXISTS
	WRITE,			///< Write and create file if not exists
	READWRITE		///< Open file for reading and writing
} FileOpenMode;


class IFile
{
public:
	// read write and other params
	virtual void Open(const char *fileName, FileMode mode) = 0;
	virtual void Close() = 0; //+
	virtual ~IFile() {};

	virtual void Rename(const char *newFileName) = 0;
	virtual bool Exist() = 0;
	virtual void Delete() = 0;

	virtual unsigned long long FileSize() = 0; //+
	virtual unsigned long long Seek() = 0; // +
	virtual void SetOffset(unsigned long long distance, MoveMethod move) = 0; //+

																			  // ReadFile
	virtual int ReadByte() = 0; //+
	virtual unsigned long long ReadFromFile(byte* arr, unsigned long long size) = 0; //+

																					 // WriteFile
	virtual void WriteToFile(byte *data, unsigned long long size) = 0; //+
	virtual void WriteByte(byte b) = 0; //+
};



/*!
\class IFile ifile.h "server\desktop\src\cross\ifile.h"
\brief  Class-interface for OS depended classes
Defined methods should be realized in OS depended classes
*/

//
//class IFile
//{
//public:
//	FileMeta* meta;	///< Meta data
//
//	/*
//	Check if file exists. Path to file will be taken from meta data.
//	\return YES if file exists; NO in other case
//	*/
//	virtual bool Exists() = 0;
//
//	/*!
//	Opens file with defined open rule
//	Possible values defined in structure higher
//	\param[in] mode Set the rule opening file
//	*/
//	virtual void Open(FileOpenMode) = 0;
//
//	/*
//	Closes file
//	*/
//	virtual void Close() = 0;
//
//	/*
//	\todo:
//	virtual void Rename() = 0;
//	virtual void Move() = 0;
//	virtual void Delete() = 0;
//	virtual int GetChar() = 0;
//	*/
//};