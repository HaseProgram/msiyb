/*!
\file fileimpl.h "server\desktop\src\cross\windows\fileimpl.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once
#include "../ifile.h"

/*!
\class FileImpl fileimpl.h "server\desktop\src\cross\windows\fileimpl.h"
\brief  Windows depended structure of file
Provides windows-specified access to file methods
*/
class FileImpl : public IFile
{
public:
	HANDLE hfile;	/// File descriptor
	bool inuse;		/// Define if file already opened

	/*!
	Allocate memory for meta structure
	Set file usage state: false
	*/
	FileImpl();

	/*!
	Allocate memory for meta structure and set path to file (filename)
	Set file usage state: false
	*/
	FileImpl(char* filename);

	/*!
	Dealocates memory
	*/
	~FileImpl();

	/*
	Check if file exists. Path to file will be taken from meta data.
	\return YES if file exists; NO in other case
	*/
	virtual bool Exists() override;

	/*
	Check if file exists. May be called out of class
	\param[in] filename Path to file (include filename)
	\return YES if file exists; NO in other case
	*/
	static bool Exists(const char* filename);

	/*
	Check if file exists. May be called out of class
	\param[in] filename Path to file (include filename)
	\return YES if file exists; NO in other case
	*/
	static bool Exists(const wchar_t*);

	/*!
	Opens file with defined open rule. Path to file will be taken from meta data.
	Possible values defined in structure higher
	\param[in] mode Set the rule opening file
	*/
	virtual void Open(FileOpenMode) override;

	/*!
	Opens file with defined open rule. May be called out of class
	Possible values defined in structure higher
	\param[in] filename Path to file (include filename)
	\param[in] mode Set the rule opening file
	*/
	static HANDLE Open(const char*, FileOpenMode);

	/*!
	Opens file with defined open rule. May be called out of class
	Possible values defined in structure higher
	\param[in] filename Path to file (include filename)
	\param[in] mode Set the rule opening file
	*/
	static HANDLE Open(const wchar_t*, FileOpenMode);

	/*
	Closes file
	*/
	virtual void Close() override;

	/*
	Closes file. For files created out of class.
	\param[in] hfile File deskriptor
	*/
	static void Close(HANDLE hfile);
};
