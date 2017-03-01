/*!
\file file.h "server\desktop\src\common\file.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once

#ifdef _WIN32
#include "../cross/windows/fileimpl.h"
#elif _unix_
#include "../cross/unix/fileimpl.h"
#endif

using namespace std;


/*!
\class File file.h "server\desktop\src\common\file.h"
\brief  File interface
Provide interface to common file structure and it's action
Factory for unix/bsd/windows structure of file
*/
class File
{
public:
	/*!
	Initialise OS depended file structure
	*/
	File();

	/*!
	Initialise OS depended file structure
	Set full file name in File class meta structure
	\param[in] filename Name of file with it's local path
	*/
	File(char* filename);

	/*!
	Dealocate OS depended File object
	*/
	~File();

	/*!
	Opens file with defined open rule
	Possible values defined in /src/cross/ifile.h
	\param[in] mode Set the rule opening file
	*/
	void Open(FileOpenMode mode);

	/*!
	Closes file
	*/
	void Close();

private:
	IFile* file;	/*!< OS depended file structure */

};