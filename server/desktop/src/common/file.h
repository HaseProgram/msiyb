/*!
\file file.h "server\desktop\src\common\file.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright © MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once
#include <string>

#ifdef _WIN32
#include "../cross/windows/winfile.h"
typedef WinFile OSFile;
#elif _unix_
typedef UnixFile OSFile;
#include "../cross/unix/unixfile.h"
#endif

using namespace std;

/*!
\class File file.h "server\desktop\src\common\file.h"
\brief  File interface
Provides interface to common file structure and it's action
Factory for unix/bsd/windows structure of file
*/
class File
{
	/// \todo File(const File &file); operator = (); GetInfo();

public:
	/*!
	Initialises cache buffers and OS depended file structure.
	*/
	File();

	/*!
	Initialises cache buffers and OS depended file structure.
	\param[in] fileName Name of file to work with.
	\param[in] bufferSize Maximum size of cache buffer.
	*/
	File(const char* fileName, size_lt bufferSize = FILE_BUFFER_SIZE);

	/*!
	Dealocates memory of cache buffers and OS depended file structure.
	*/
	~File();

	/*!
	Opens file in predetermined mode.
	\param[in] fileName Name of file to work with.
	\param[in] mode Mode to open file (list of possibles modes defined in ifile.h).
	*/
	void Open(const char *fileName, FileOpenMode mode);

	/*!
	Opens file in predetermined mode.
	\param[in] mode Mode to open file (list of possibles modes defined in ifile.h).
	*/
	void Open(FileOpenMode mode);

	/*!
	Flushes bytes from cache buffer to write into file.
	Closes file descriptor.
	*/
	void Close();

	/*!
	Set new file name.
	\param[in] newFileName Name to be associated to a current file.
	*/
	void Rename(const char *newFileName);

	/*!
	Set new file name. Static.
	\param[in] fileName Current name of file to be replaced.
	\param[in] newFileName Name to be associated to a current file.
	*/
	static void Rename(const char *fileName, const char *newFileName);

	/*!
	Checks if file exists.
	\return TRUE if file exists, FALSE in other case.
	*/
	bool Exist();

	/*!
	Checks if file exists. Static.
	\param[in] fileName Name of file to be checked.
	\return TRUE if file exists, FALSE in other case.
	*/
	static bool Exist(const char *fileName);

	/*!
	Get file info.
	\return Info about file.
	*/
	FileMeta GetInfo();

	/*!
	\TODO
	Get file info. Static.
	\param[in] string fileName Name of file to read.
	\return Info about file.
	*/
	static FileMeta GetInfo(const char *fileName);

	/*!
	Deletes file.
	*/
	void Delete();

	/*!
	Deletes file. Static.
	\param[in] fileName Name of file to be deleted.
	*/
	static void Delete(const char *fileName);

	/*!
	Checks if all data from buffer already readed and fill it with new data.
	\return TRUE if cache filled with new data, FALSE in other case (ex. EOF).
	*/
	inline bool CheckCache();

	/*!
	Reads one byte from file.
	\return Value of readed byte or -1 if can't read (ex. EOF).
	*/
	int ReadByte();

	/*!
	Reads block from file.
	\param[out] block Array of bytes from file.
	\param[in] sizeBlock Amount of bytes to be readed.
	\return Amount of bytes were readed from file.
	*/
	size_lt ReadBlock(byte *block, size_lt sizeBlock);

	/*!
	Saves one byte in cache.
	Will be writed in file after cache fills or after Flush method called.
	Clears cache of readed bytes.
	\param[in] b Byte to save.
	*/
	void WriteByte(byte b);

	/*!
	Writes block into file.
	Clears cache of readed bytes.
	\param[in] block Array of bytes to be writed.
	\param[in] blockSize Amount of bytes to be writed.
	*/
	void WriteBlock(byte *block, size_lt sizeBlock);

	/*!
	Writes in to file cache of writed bytes.
	Clears cache of readed bytes.
	*/
	void Flush();

	/*!
	Moves the file pointer of the specified file.
	\param[in] offset Offset of new pointer position
	\param[in] move Position used as reference for the offset.
	\return New pointer position
	*/
	size_lt Seek(size_lt offset, SeekReference move);

	/*!
	Return size of file.
	\return Size of file.
	*/
	size_lt FileSize();

	/*!
	Return size of file. Static.
	\return Size of file.
	*/
	static size_lt FileSize(const char *fileName);



	/////////////////////////////////////////////////////////////////////////////
	////////////////////           STATIC METHODS            ////////////////////
	/////////////////////////////////////////////////////////////////////////////

	/*!
	Read full file. Static.
	\param[in] fileName Name of file to read.
	\param[out] byteArr Data readed from file.
	\return Amount of readed bytes.
	*/
	static size_lt ReadAllBytes(const char *fileName, byte **byteArr);

	/*!
	Read full file and parse it on strings. Static.
	\param[in] fileName Name of file to read.
	\param[out] byteArr Data readed from file.
	\return Amount of readed strings.
	*/
	static size_lt ReadAllCharStrings(const char *fileName, char*** charStrings);

	/*!
	Read full file and parse it on strings. Static.
	\param[in] fileName Name of file to read.
	\param[out] byteArr Data readed from file.
	\return Amount of readed strings.
	*/
	static size_lt ReadAllStrings(const char *fileName, string** stringArray);

	/*!
	Read buffer and parse it on strings. Static.
	\param[out] string String array from buffer.
	\param[in] buf Buffer to read.
	\param[in] startPos Position in buffer fom which we start read.
	\param[in] sizeBuf Buffer size.
	\return Amount of readed strings.
	*/
	static size_lt ReadStringFromBuffer(char **string, byte *buf, size_lt *startPos, size_lt sizeBuf);

	/*!
	Open file and write all data in it. Static.
	\param[in] fileName Name of file to read.
	\param[in] data Data to write in file.
	\param[in] size Size of data buffer.
	\param[in] mode Open file mode.
	*/
	static void WriteAllBytes(const char *fileName, byte* data, size_lt size, FileOpenMode mode = WRITENEWFILE);

	/*!
	Open file and write all data (in char string format) in it. Static.
	\param[in] string fileName Name of file to read.
	\param[in] charStrings Strings to write in file.
	\param[in] countStrings Amount of strings to write.
	\param[in] mode Open file mode.
	*/
	static void WriteAllCharStrings(const char *fileName, char** charStrings, size_lt countStrings, FileOpenMode mode = WRITENEWFILE);

	/*!
	Open file and write all data (in string format) in it. Static.
	\param[in] string fileName Name of file to read.
	\param[in] charStrings Strings to write in file.
	\param[in] countStrings Amount of strings to write.
	\param[in] mode Open file mode.
	*/
	static void WriteAllStrings(const char *fileName, string *strings, size_lt countStrings, FileOpenMode mode = WRITENEWFILE);
	
	/*!
	\TODO
	Get last modified file. Static.
	\param[in] string fileName Name of file to read.
	\return Info about last modified file.
	*/
	static FileMeta LastModified(const char *fileName);

private:
	IFile *_file;					///< OS depended file structure 

	char *_fileName;				///< Path to a file (including it's name)
	bool _opened;					///< Descriptor opening status

	size_lt _bufferSize;			///< Determine size of cache buffer

	byte *_cacheReaded;				///< Cache of bytes were readed from file
	size_lt _posInCacheReaded;		///< Pointer to a position in cache of readed bytes
	size_lt _bytesInCacheReaded;	///< Amount of bytes left in cache of readed bytes

	byte *_cacheToWrite;			///< Cache of bytes are ready to be writen in file
	size_lt _posInCacheToWrite;		///< Pointer to a position in cache of bytes to write in file
	size_lt _bytesInCacheToWrite;	///< Amount of bytes left in cache of bytes to write in file

};