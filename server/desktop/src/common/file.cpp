#include "stdafx.h"
#include "file.h"
#include "stringmethods.h"

File::File(const char* fileName, size_lt bufferSize)
{
	strcpy(this->fileName, fileName);
	this->opened = false;

	this->bufferSize = bufferSize;

	this->cacheReaded = new byte[this->bufferSize];
	if (!this->cacheReaded)
	{
		ThrowException("Can't allocate memory!");
	}
	this->bytesInCacheReaded = 0;
	this->posInCacheReaded = 0;

	this->cacheToWrite = new byte[this->bufferSize];
	if (!this->cacheToWrite)
	{
		ThrowException("Can't allocate memory!");
	}
	this->bytesInCacheToWrite = 0;
	this->posInCacheToWrite = 0;

	this->file = new OSFile(fileName);
}

File::~File()
{
	if (this->opened)
	{
		this->Close();
	}

	delete[] this->cacheReaded;
	delete[] this->cacheToWrite;
	delete this->file;
}

void File::Open(FileOpenMode mode)
{
	this->file->Open(mode);
}

void File::Close()
{
	if (this->bytesInCacheToWrite > 0)
	{
		this->Flush();
	}
	this->file->Close();
}

void File::Rename(const char *newFileName)
{
	this->file->Rename(newFileName);
}

void File::Rename(const char *fileName, const char *newFileName)
{
	OSFile::Rename(fileName, newFileName);
}

bool File::Exist()
{
	return this->file->Exist();
}

bool File::Exist(const char *fileName)
{
	return OSFile::Exist(fileName);
}

FileMeta File::GetInfo()
{
	return this->GetInfo(this->fileName);
}

FileMeta File::GetInfo(const char * fileName)
{
	FileMeta meta;
	return meta;
}

void File::Delete()
{
	this->file->Delete();
}

void File::Delete(const char *fileName)
{
	OSFile::Delete(fileName);
}

bool File::CheckCache()
{
	if (this->bytesInCacheReaded == 0)
	{
		this->bytesInCacheReaded = file->ReadBlock(this->cacheReaded, this->bufferSize);
		if (this->bytesInCacheReaded == 0)
		{
			return false;
		}
		this->posInCacheReaded = 0;
	}
	return true;
}

int File::ReadByte()
{
	if (!this->CheckCache())
	{
		return -1;
	}
	byte b = this->cacheReaded[this->posInCacheReaded];
	this->posInCacheReaded++;
	this->bytesInCacheReaded--;
	return b;
}

size_lt File::ReadBlock(byte *block, size_lt blockSize)
{
	if (blockSize > this->bufferSize)
	{
		/*
		If our size of block is bigger then max cache size:
		1) If our cache isn't empty: we put our cache in block
		2) Size of readed block: data which was in cache (maybe 0) + data readed from file (we read block size - data in cache)
		*/
		if (this->bytesInCacheReaded > 0)
		{
			memcpy(block, this->cacheReaded + this->posInCacheReaded, this->bytesInCacheReaded);
		}
		int size = this->bytesInCacheReaded + file->ReadBlock(block + this->bytesInCacheReaded, blockSize - this->bytesInCacheReaded);
		this->bytesInCacheReaded = 0;
		this->posInCacheReaded = 0;
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
		if (blockSize > this->bytesInCacheReaded)
		{
			memcpy(block, this->cacheReaded + this->posInCacheReaded, this->bytesInCacheReaded);
			this->posInCacheReaded = 0;
			size_lt tempReadedBytes = this->bytesInCacheReaded;
			this->bytesInCacheReaded = file->ReadBlock(this->cacheReaded, this->bufferSize);
			memcpy(block + tempReadedBytes, this->cacheReaded + this->posInCacheReaded, blockSize - tempReadedBytes);
			if (blockSize-tempReadedBytes > this->bytesInCacheReaded)
			{
				tempReadedBytes += this->bytesInCacheReaded;
				this->bytesInCacheReaded = 0;
				this->posInCacheReaded = 0;
				return tempReadedBytes;
			}
			else
			{
				this->bytesInCacheReaded = this->bufferSize - (blockSize - tempReadedBytes);
				this->posInCacheReaded = blockSize - tempReadedBytes;
				return blockSize;
			}
		}
		else
		{
			memcpy(block, this->cacheReaded + this->posInCacheReaded, blockSize);
			this->bytesInCacheReaded -= blockSize;
			this->posInCacheReaded += blockSize;
			return blockSize;
		}
	}
}

void File::WriteByte(byte b)
{
	if (this->bytesInCacheToWrite == this->bufferSize)
	{
		this->Flush();
	}
	if (this->bytesInCacheToWrite < this->bufferSize)
	{
		this->cacheToWrite[this->posInCacheToWrite];
		this->posInCacheToWrite++;
		this->bytesInCacheToWrite++;
	}
}

void File::WriteBlock(byte *block, size_lt blockSize)
{
	this->Flush();
	this->file->WriteBlock(block, blockSize);
}

void File::Flush()
{
	if (this->bytesInCacheReaded > 0)
	{
		this->bytesInCacheReaded = 0;
		this->posInCacheReaded = 0;
	}
	this->file->WriteBlock(this->cacheToWrite + this->posInCacheToWrite, this->bytesInCacheToWrite);
	this->bytesInCacheToWrite = 0;
	this->posInCacheToWrite = 0;
}

size_lt File::Seek(size_lt offset, SeekReference move)
{
	return this->file->Seek(offset, move);
}

size_lt File::FileSize()
{
	return this->file->FileSize();
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
