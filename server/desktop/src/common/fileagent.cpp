#include "stdafx.h"
#include "fileagent.h"

FileAgent::FileAgent()
{
	this->file = new File;
}

FileAgent::FileAgent(char* filename)
{
	this->file = new File(filename);
}

FileAgent::~FileAgent()
{
	delete this->file;
}

void FileAgent::Open(FileOpenMode mode)
{
	this->file->Open(mode);
}

void FileAgent::Close()
{
	this->file->Close();
}
