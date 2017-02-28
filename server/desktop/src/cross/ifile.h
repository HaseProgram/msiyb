/*!
\file ifile.h "server\desktop\src\cross\ifile.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once

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

/*!
\class IFile ifile.h "server\desktop\src\cross\ifile.h"
\brief  Class-interface for OS depended classes
Defined methods should be realized in OS depended classes
*/
class IFile
{
public:
	FileMeta* meta;	///< Meta data

	/*
	Check if file exists. Path to file will take from meta data.
	\return YES if file exists; NO in other case
	*/
	virtual bool Exists() = 0;

	/*!
	Opens file with defined open rule
	Possible values defined in structure higher
	\param[in] mode Set the rule opening file
	*/
	virtual void Open(FileOpenMode) = 0;

	/*
	Closes file
	*/
	virtual void Close() = 0;

	/*
	\todo:
	virtual void Rename() = 0;
	virtual void Move() = 0;
	virtual void Delete() = 0;
	virtual int GetChar() = 0;
	*/
};