/*!
\file winfile.h "server\desktop\src\cross\windows\winfile.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 07 March 2017
*/

#pragma once

#include "../ifile.h"
#include "windows.h"

using namespace std;

/*!
\class WinFile winfile.h "server\desktop\src\cross\windows\winfile.h"
\brief  Windows depended structure of file.
Provides windows-specified access to file methods.
*/
class WinFile : public IFile
{
public:
	/*!
	Initialize filename with null values.
	*/
	WinFile();
	
	/*!
	Set fileName with determ value.
	\param[in] fileName Name of file.
	*/
	WinFile(const char *fileName);

	/*!
	Dealocates memory and closes file description.
	*/
	~WinFile();
	
	/*!
	Opens file in predetermined mode.
	\param[in] mode Mode to open file (list of possibles modes defined in ifile.h).
	*/
	virtual void Open(FileOpenMode mode) override;

	/*!
	Open file.
	\param[in] fileName Name of file to open.
	\param[in] mode Depends mode in which file will be opened.
	*/
	virtual void Open(const char *fileName, FileOpenMode mode) override;

	/*!
	Open file. Static. For TCHAR type.
	\param[in] fileName Name of file to open.
	\param[in] mode Depends mode in which file will be opened.
	\return Handle of file
	*/
	static HANDLE tOpen(const TCHAR *fileName, FileOpenMode mode);

	/*!
	Closes file descriptor.
	*/
	virtual void Close() override;

	/*!
	Close file. Static.
	\param[in] hFile Handle of opened file.
	*/
	static void Close(HANDLE hFile);

	/*!
	Set new file name.
	\param[in] newFileName Name to be associated to a current file.
	*/
	virtual void Rename(const char *newFileName) override;

	/*!
	Set new file name. Static.
	\param[in] fileName Current name of file.
	\param[in] newFileName Name to be associated to a current file.
	*/
	static void Rename(const char *fileName, const char *newFileName);

	/*!
	Checks if file exists.
	\return TRUE if file exists, FALSE in other case.
	*/
	virtual bool Exist() override;

	/*!
	Checks if file exists. Static.
	\param[in] fileName Name of file.
	\return TRUE if file exists, FALSE in other case.
	*/
	static bool Exist(const char *fileName);

	/*!
	Checks if file exists. Static. For TCHAR type.
	\param[in] wFileName Name of file.
	\return TRUE if file exists, FALSE in other case.
	*/
	static bool tExist(const TCHAR *tFileName);

	/*!
	Deletes file.
	*/
	virtual void Delete() override;

	/*!
	Deletes file. Static.
	\param[in] fileName Name of file.
	*/
	static void Delete(const char *fileName);

	/*!
	Deletes file. Static. For TCHAR type.
	\param[in] fileName Name of file.
	*/
	static void tDelete(const TCHAR *tFileName);

	/*!
	Return size of file.
	\return Size of file.
	*/
	virtual size_lt FileSize() override;

	/*!
	Return size of file. Static.
	\param[in] hFile Handle of opened file.
	\return Size of file.
	*/
	static size_lt FileSize(HANDLE hFile);

	/*!
	Return size of file. Static.
	\param[in] fileName Name of file.
	\return Size of file.
	*/
	static size_lt FileSize(const char *fileName);

	/*!
	Moves the file pointer of the specified file.
	\param[in] offset Offset of new pointer position
	\param[in] move Position used as reference for the offset.
	\return New pointer position
	*/
	virtual size_lt Seek(size_lt offset, SeekReference move) override;

	/*!
	Reads one byte from file.
	\return Value of readed byte or -1 if can't read (ex. EOF).
	*/
	virtual int ReadByte() override;

	/*!
	Reads block from file.
	\param[out] block Array of bytes from file.
	\param[in] sizeBlock Amount of bytes to be readed.
	\return Amount of bytes were readed from file.
	*/
	virtual size_lt ReadBlock(byte *block, size_lt blockSize = MIN_BUFFER_SIZE) override;

	/*!
	Reads block from file. Static.
	\param[in] hFile Handle of opened file.
	\param[out] block Array of bytes from file.
	\param[in] sizeBlock Amount of bytes to be readed.
	\return Amount of bytes were readed from file.
	*/
	static size_lt ReadBlock(HANDLE hFile, byte *block, size_lt blockSize = MIN_BUFFER_SIZE);

	/*!
	Writes byte in file.
	\param[in] b Byte to save.
	*/
	virtual void WriteByte(byte b) override;

	/*!
	Writes block into file.
	\param[in] block Array of bytes to be writed.
	\param[in] blockSize Amount of bytes to be writed.
	*/
	virtual void WriteBlock(byte *block, size_lt sizeBlock) override;

	/*!
	Writes block into file. Static.
	\param[in] hFile Handle of opened file.
	\param[in] block Array of bytes to be writed.
	\param[in] blockSize Amount of bytes to be writed.
	*/
	static void WriteBlock(HANDLE hFile, byte *block, size_lt sizeBlock);

	/////////////////////////////////////////////////////////////////////////////
	////////////////////           STATIC METHODS            ////////////////////
	/////////////////////////////////////////////////////////////////////////////

	/*!
	Read full file. Static.
	\param[in] fileName Name of file to read.
	\param[out] byteArr Data readed from file.
	\return Amount of readed bytes.
	*/
	static size_lt ReadAllBytes(const char *fileName, byte **block);

	/*!
	Open file and write all data in it. Static.
	\param[in] fileName Name of file to read.
	\param[in] data Data to write in file.
	\param[in] size Size of data buffer.
	\param[in] mode Open file mode.
	*/
	static void WriteAllBytes(const char *fileName, byte* data, size_lt size, FileOpenMode mode = WRITENEWFILE);

	/*!
	\TODO
	Get last modified file. Static.
	\param[in] string fileName Name of file to read.
	\return Info about last modified file.
	*/
	static FileMeta LastModified(const char *fileName);

private:
	HANDLE _hFile;			///< Handle of opened file
	char *_fileName;		///< Name of file
	WCHAR *_tFileName;		///< Name of file in unicode charset
	bool _opened;			///< Determine is file open or not
};