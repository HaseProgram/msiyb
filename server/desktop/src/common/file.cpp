#include "stdafx.h"
#include "file.h"

//File::File()
//{
//	this->file = new FileImpl;
//}
//
//File::File(char* filename)
//{
//	this->file = new FileImpl(filename);
//}
//
//File::~File()
//{
//	delete this->file;
//}
//
//void File::Open(FileOpenMode mode)
//{
//	this->file->Open(mode);
//}
//
//void File::Close()
//{
//	this->file->Close();
//}
//
//

//byte buf[FILE_BUFFER_SIZE];

static int File::ReadStringFromBuffer(char **string, byte *buf, int *startPos, int sizeBuf)
{
	int stringNotReaded = 1;
	int sizeString = MIN_STRING_SIZE;
	char *str = new char[sizeString];
	int idx = 0;
	int pos = *startPos;
	int rCoef = 2;

	if (!str) ThrowException("Cant alloc memory for str");

	for (int i = pos; i < sizeBuf && stringNotReaded; i++)
	{
		if (idx == sizeString)
			Resize(&str, &sizeString, sizeString * rCoef);
		//ResizeString(&str, &sizeString, 2);

		str[idx++] = buf[i];

		if (buf[i] == '\n' || buf[i] == '\0' || i == sizeBuf - 1)
		{
			stringNotReaded = 0;
			*startPos = i + 1;
		}
	}
	if (idx == sizeString)
		Resize(&str, &sizeString, sizeString + 1);
	str[idx] = '\0';
	*string = str;
	return sizeString;
}


//inline void File::CheckAllocate(byte *arr)
//{
//	if (!arr)
//		ThrowException("Cant allocate memory");
//}

File::File()
{
	fileReadedBytesBuffer = fileWritedBytesBuffer = NULL;
	readedBytes = writedBytes = 0;
	buffersSize = FILE_BUFFER_SIZE;
	posInReadedBuf = posInWritedBuf = 0;

	fileReadedBytesBuffer = new byte[buffersSize];
	//CheckAllocate(fileReadedBytesBuffer);
	if (!fileReadedBytesBuffer)
		ThrowException("Cant allocate memory");

	fileWritedBytesBuffer = new byte[buffersSize];
	//CheckAllocate(fileWritedBytesBuffer);
	if (!fileReadedBytesBuffer)
		ThrowException("Cant allocate memory");
	//if (!fileReadedBytesBuffer)
	//	ThrowException("Cant allocate memory");

	//if ()

	file = new OsFile();
}

FIle::~File()
{
	// check write buffer
	if (opened)
	{
		Close();
	}

	delete[] fileReadedBytesBuffer;
	delete[] fileWritedBytesBuffer;
	delete file;
}

void File::Open(const char *fileName, FileMode mode)
{
	return file->Open(fileName, mode);
}

void Close()
{
	// check write buffer
	if (writedBytes > 0)
		Fflush()
	file->Close();
}

void Rename(const char *newFileName)
{
	file->Rename(newFileName);
}

bool File::Exist()
{
	return Exist(fileName);
}

static bool Exist(const char *fileName)
{
	OsFile::Exist(fileName);
}

void Delete()
{
	file->Delete();
}

inline void CheckBuffer()
{
	if (readedBytes == 0)// == posInReadedBuf)
		readedBytes = file->ReadFromFile(fileWritedBytesBuffer, buffersSize);
}

int ReadByte()
{
	CheckBuffer();
	//if (readedBytes = posInBuf)
	//int readed = file->ReadFromFile(fileBuffer, bufferSize);
	//if (readedBytes == posInReadedBuf)

	int b = fileReadedBytesBuffer[posInReadedBuf];//file->ReadByte();
	posInReadedBuf++;
	readedBytes--;
	return b;
}

unsigned long long ReadFromFile(byte *block, unsigned long long sizeBlock)
{
	if (sizeBlock > buffersSize)
	{
		if (readedBytes > 0)
			memcpy(block, fileReadedBytesBuffer, readedBytes);
		int size = readedBytes + file->ReadFromFile(block + readedBytes, sizeBlock - readedBytes);
		readedBytes = 0;
		return size;
	}
	else
	{
		/// TODO CHECK WORK OF UP CODE ON ALL IN
	}
	unsigned long long size = file->ReadFromFile(block, sizeBlock);
	return size;
}

void WriteToFile(byte *block, unsigned long long sizeBlock)
{
	file->WriteToFile(block, sizeBlock);
}

void WriteByte(byte b)
{
	if (posInWritedBuf == buffersSize)
	{
		//	
		fileWrtedBytesBuffer[posInWritedBuf];
		posInWritedBuf++;

		file->WriteByte(b);
	}

}

unsigned long long Seek()
{
	unsigned long long seek = file->Seek();
	return seek;
}

unsigned long long FileSize()
{
	return file->FileSize();
}

void SetOffset(unsigned long long distance, MoveMethod move)
{
	file->SetOffset(distance, move);
}

/////// static Maybe
static int ReadAllBytes(const char *fileName, byte **byteArr)
{
	unsigned long long size;
	byte *buf;
	size = OsFile::ReadAllBytes(&buf, fileName);
	*byteArr = buf;
	return size;
}


static int ReadAllCharStrings(const char *fileName, char*** charStrings)
{
	byte *buf;
	int sizeBuf = File::ReadAllBytes(fileName, &buf);
	int sizeStringArray = MIN_STRING_ARRAY_SIZE;

	char **stringArray = new char*[sizeStringArray];
	if (!stringArray) ThrowException("Cant alloc mem for string");

	int posInBuf = 0;

	int countStrings = 0;
	int rCoef = 2;
	while (posInBuf < sizeBuf)
	{
		char *str;
		ReadStringFromBuffer(&str, buf, &posInBuf, sizeBuf);
		if (countStrings == sizeStringArray)
			ResizeStringArray(&stringArray, &sizeStringArray, rCoef);
		stringArray[countStrings++] = str;
	}
	*charStrings = stringArray;
	delete[] buf;
	return countStrings;
}


///////////////////////////////////////////////////
static char* LastModified(const char *fileName)
{
	return OsFile::LastModified(fileName);
}
/////////////////////////////////////////////////



static int ReadAllStrings(const char *fileName, string** stringArray)
{
	char **charStrings;
	int cnt = File::ReadAllCharStrings(fileName, &charStrings);
	string *strings = new string[cnt];
	if (!strings) ThrowException("Cant Alloc strings");

	for (int i = 0; i < cnt; i++)
		strings[i] = charStrings[i];
	*stringArray = strings;
	return cnt;
}

static unsigned long long FileSize(const char *fileName)
{
	unsigned long long size;
	size = OsFile::FileSize(fileName);
	return size;
}

static void Delete(const char *fileName)
{
	OsFile::Delete(fileName);
}

static void WriteAllBytes(const char *fileName, byte* data, int size, FileMode mode = OpenWriteIfExist)
{
	OsFile::WriteAllBytes(data, size, fileName, mode);
}

static void WriteAllCharStrings(const char *fileName, char** charStrings, int countStrings)
{
	byte* data;
	int size = LightConverter<0, char**, byte>::ConvertCharStringArrayTobyte(&data, charStrings, countStrings);
	File::WriteAllBytes(fileName, data, size);
}

static void WriteAllStrings(const char *fileName, string *strings, int countStrings)
{
	byte* data;
	int size = LightConverter<0, char**, byte>::ConvertStringArrayTobyte(&data, strings, countStrings);
	File::WriteAllBytes(fileName, data, size);
}
