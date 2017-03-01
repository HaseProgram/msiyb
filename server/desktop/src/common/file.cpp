#include "stdafx.h"

File::File()
{
	this->file = new FileImpl;
}

File::File(char* filename)
{
	this->file = new FileImpl(filename);
}

File::~File()
{
	delete this->file;
}

void File::Open(FileOpenMode mode)
{
	this->file->Open(mode);
}

void File::Close()
{
	this->file->Close();
}
