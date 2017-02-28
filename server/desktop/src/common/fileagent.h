/*!
\file fileagent.h "server\desktop\src\common\fileagent.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once

#ifdef _WIN32
#include "../cross/windows/file.h"
#elif _unix_
#include "../cross/unix/file.h"
#endif

using namespace std;


/*!
\class FileAgent fileagent.h "server\desktop\src\common\fileagent.h"
\brief  File interface
Provide interface to common file structure and it's action
Factory for unix/bsd/windows structure of file
*/
class FileAgent
{
public:
	/*!
	Initialise OS depended file structure
	*/
	FileAgent();

	/*!
	Initialise OS depended file structure
	Set full file name in File class meta structure
	\param[in] filename Name of file with it's local path
	*/
	FileAgent(char* filename);

	/*!
	Dealocate OS depended File object
	*/
	~FileAgent();

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