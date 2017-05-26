#include "file.h"
#include "stringmethods.h"

File::File()
{
	_opened = false;
	_bufferSize = FILE_BUFFER_SIZE;

	_fileName = new char[MAX_PATH];
	if (!_fileName)
	{
		ThrowException("Can't allocate memory!")
	}

	_cacheReaded = new byte[_bufferSize];
	if (!_cacheReaded)
	{
		ThrowException("Can't allocate memory!");
	}
	_bytesInCacheReaded = 0;
	_posInCacheReaded = 0;

	_cacheToWrite = new byte[_bufferSize];
	if (!_cacheToWrite)
	{
		ThrowException("Can't allocate memory!");
	}
	_bytesInCacheToWrite = 0;
	_posInCacheToWrite = 0;

	_file = new OSFile();
	if (!_file)
	{
		ThrowException("Can't allocate memory!");
	}
}

File::File(const char* fileName, size_lt bufferSize)
{
	_fileName = new char[MAX_PATH];
	if (!_fileName)
	{
		ThrowException("Can't allocate memory!")
	}

	strcpy(_fileName, fileName);
	_opened = false;

	_bufferSize = bufferSize;

	_cacheReaded = new byte[_bufferSize];
	if (!_cacheReaded)
	{
		ThrowException("Can't allocate memory!");
	}
	_bytesInCacheReaded = 0;
	_posInCacheReaded = 0;

	_cacheToWrite = new byte[_bufferSize];
	if (!_cacheToWrite)
	{
		ThrowException("Can't allocate memory!");
	}
	_bytesInCacheToWrite = 0;
	_posInCacheToWrite = 0;

	_file = new OSFile(fileName);
	if (!_file)
	{
		ThrowException("Can't allocate memory!");
	}
}

File::~File()
{
	if (_opened)
	{
		Close();
	}

	delete[] _cacheReaded;
	delete[] _cacheToWrite;
	delete _file;
}

void File::Open(FileOpenMode mode)
{
	// TODO check previous opened file
	if (_opened)
	{
		Close();
	}
	if (mode == FileOpenMode::WRRITEATTEHEND)
	{
		_file->Open(FileOpenMode::READWRITE);
		_file->Seek(0, SeekReference::END);
	}
	else
	{
		_file->Open(mode);
	}
}

void File::Open(const char *fileName, FileOpenMode mode)
{
	if (_opened)
	{
		Close();
	}

	strcpy(_fileName, fileName);

	if (mode == FileOpenMode::WRRITEATTEHEND)
	{
		_file->Open(fileName, FileOpenMode::READWRITE);
		_file->Seek(0, SeekReference::END);
	}
	else
	{
		_file->Open(fileName, mode);
	}
	_opened = true;
}

void File::Close()
{
	if (_bytesInCacheToWrite > 0)
	{
		Flush();
	}

	_file->Close();
}

void File::Rename(const char *newFileName)
{
	_file->Rename(newFileName);
}

void File::Rename(const char *fileName, const char *newFileName)
{
	OSFile::Rename(fileName, newFileName);
}

bool File::Exist()
{
	return _file->Exist();
}

bool File::Exist(const char *fileName)
{
	return OSFile::Exist(fileName);
}

FileMeta File::GetInfo()
{
	return GetInfo(_fileName);
}

FileMeta File::GetInfo(const char *fileName)
{
	FileMeta meta;
	return meta;
}

void File::Delete()
{
	_file->Delete();
}

void File::Delete(const char *fileName)
{
	OSFile::Delete(fileName);
}

bool File::CheckCache()
{
	if (_bytesInCacheReaded == 0)
	{
		_bytesInCacheReaded = _file->ReadBlock(_cacheReaded, _bufferSize);
		if (_bytesInCacheReaded == 0)
		{
			return false;
		}
		_posInCacheReaded = 0;
	}
	return true;
}

int File::ReadByte()
{
	if (!CheckCache())
	{
		return -1;
	}
	byte b = _cacheReaded[_posInCacheReaded];
	_posInCacheReaded++;
	_bytesInCacheReaded--;
	return b;
}

size_lt File::ReadBlock(byte *block, size_lt blockSize)
{
	if (blockSize > _bufferSize)
	{
		/*
		If our size of block is bigger then max cache size:
		1) If our cache isn't empty: we put our cache in block
		2) Size of readed block: data which was in cache (maybe 0) + data readed from file (we read block size - data in cache)
		*/
		if (_bytesInCacheReaded > 0)
		{
			memcpy(block, _cacheReaded + _posInCacheReaded, _bytesInCacheReaded);
		}
		int size = _bytesInCacheReaded + _file->ReadBlock(block + _bytesInCacheReaded, blockSize - _bytesInCacheReaded);
		_bytesInCacheReaded = 0;
		_posInCacheReaded = 0;
		return size;
	}
	else
	{
		/*
		If our size of block is less then max cache size:
		1) If size of block is bigger then amount of bytes in cache:
		1. We put our bytes from cache in block
		2. We fill cache from file
		3. We put remaining amount of bytes in block from just filled cache
		4. If our block requires more bytes our file contains, size will be previous bytes amount in cache + size value readed from file.
		Else size = blockSize
		2) If size of block is less then amount of bytes in cache we just fill block from cache
		*/
		if (blockSize > _bytesInCacheReaded)
		{
			memcpy(block, _cacheReaded + _posInCacheReaded, _bytesInCacheReaded);
			_posInCacheReaded = 0;
			size_lt tempReadedBytes = _bytesInCacheReaded;
			_bytesInCacheReaded = _file->ReadBlock(_cacheReaded, _bufferSize);
			memcpy(block + tempReadedBytes, _cacheReaded + _posInCacheReaded, blockSize - tempReadedBytes);
			if (blockSize - tempReadedBytes > _bytesInCacheReaded)
			{
				tempReadedBytes += _bytesInCacheReaded;
				_bytesInCacheReaded = 0;
				_posInCacheReaded = 0;
				return tempReadedBytes;
			}
			else
			{
				_bytesInCacheReaded = _bufferSize - (blockSize - tempReadedBytes);
				_posInCacheReaded = blockSize - tempReadedBytes;
				return blockSize;
			}
		}
		else
		{
			memcpy(block, _cacheReaded + _posInCacheReaded, blockSize);
			_bytesInCacheReaded -= blockSize;
			_posInCacheReaded += blockSize;
			return blockSize;
		}
	}
}

void File::WriteByte(byte b)
{
	if (_bytesInCacheToWrite == _bufferSize)
	{
		Flush(); // After this operation write buffer is empty
	}
	if (_bytesInCacheToWrite < _bufferSize)
	{
		_cacheToWrite[_posInCacheToWrite] = b;
		_posInCacheToWrite++;
		_bytesInCacheToWrite++;
	}
}

void File::WriteBlock(byte *block, size_lt blockSize)
{
	Flush();
	_file->WriteBlock(block, blockSize);
}

void File::Flush()
{
	if (_bytesInCacheReaded > 0)
	{
		_bytesInCacheReaded = 0;
		_posInCacheReaded = 0;
	}
	_file->WriteBlock(_cacheToWrite, _bytesInCacheToWrite);
	//this->file->WriteBlock(this->cacheToWrite + this->posInCacheToWrite, this->bytesInCacheToWrite);
	_bytesInCacheToWrite = 0;
	_posInCacheToWrite = 0;
}

size_lt File::Seek(size_lt offset, SeekReference move)
{
	return _file->Seek(offset, move);
}

size_lt File::FileSize()
{
	return _file->FileSize();
}

size_lt File::FileSize(const char *fileName)
{
	return OSFile::FileSize(fileName);
}

size_lt File::ReadAllBytes(const char *fileName, byte **byteArr)
{
	size_lt size;
	byte *buf;
	size = OSFile::ReadAllBytes(fileName, &buf);
	*byteArr = buf;
	return size;
}

size_lt File::ReadAllCharStrings(const char *fileName, char*** charStrings)
{
	byte *buf;
	size_lt sizeBuf = File::ReadAllBytes(fileName, &buf);
	size_lt sizeStringArray = MIN_STRING_ARRAY_SIZE;

	char **stringArray = new char*[sizeStringArray];
	if (!stringArray)
	{
		ThrowException("Can't allocate memory for string!");
	}

	size_lt posInBuf = 0;
	size_lt countStrings = 0;
	int rCoef = 2;
	while (posInBuf < sizeBuf)
	{
		char *str;
		ReadStringFromBuffer(&str, buf, &posInBuf, sizeBuf);
		if (countStrings == sizeStringArray)
		{
			ResizeStringArray(&stringArray, &sizeStringArray, rCoef);
		}
		stringArray[countStrings++] = str;
	}
	*charStrings = stringArray;
	delete[] buf;
	return countStrings;
}

size_lt File::ReadAllStrings(const char *fileName, string** stringArray)
{
	char **charStrings;
	size_lt cnt = File::ReadAllCharStrings(fileName, &charStrings);
	string *strings = new string[cnt];
	if (!strings)
	{
		ThrowException("Cant Alloc strings");
	}

	for (size_lt i = 0; i < cnt; i++)
	{
		strings[i] = charStrings[i];
	}

	*stringArray = strings;
	return cnt;
}

size_lt File::ReadStringFromBuffer(char **string, byte *buf, size_lt *startPos, size_lt sizeBuf)
{
	size_lt stringNotReaded = 1;
	size_lt sizeString = MIN_STRING_SIZE;
	char *str = new char[sizeString];
	int idx = 0;
	int pos = *startPos;
	int rCoef = 2;

	if (!str)
	{
		ThrowException("Cant alloc memory for str");
	}

	for (int i = pos; i < sizeBuf && stringNotReaded; i++)
	{
		if (idx == sizeString)
		{
			Resize(&str, &sizeString, sizeString * rCoef);
		}

		if (buf[i] != '\n')
			str[idx++] = buf[i];

		if (buf[i] == '\n' || buf[i] == '\0' || i == sizeBuf - 1)
		{
			stringNotReaded = 0;
			*startPos = i + 1;
		}
	}

	if (idx == sizeString)
	{
		Resize(&str, &sizeString, sizeString + 1);
	}

	str[idx] = '\0';
	*string = str;
	return sizeString;
}

void File::WriteAllBytes(const char *fileName, byte* data, size_lt size, FileOpenMode mode)
{
	OSFile::WriteAllBytes(fileName, data, size, mode);
}

void File::WriteAllCharStrings(const char *fileName, char** charStrings, size_lt countStrings, FileOpenMode mode)
{
	byte* data;
	size_lt size = ConvertCharStringArrayToByte(&data, charStrings, countStrings);
	File::WriteAllBytes(fileName, data, size, mode);
}

void File::WriteAllStrings(const char *fileName, string *strings, size_lt countStrings, FileOpenMode mode)
{
	byte* data;
	size_lt size = ConvertStringArrayToByte(&data, strings, countStrings);
	File::WriteAllBytes(fileName, data, size, mode);
}

FileMeta File::LastModified(const char *fileName)
{
	FileMeta meta;
	//OSFile::LastModified(fileName);
	return meta;
}