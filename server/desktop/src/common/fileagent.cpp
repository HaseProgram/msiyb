#include "stdafx.h"
#include "fileagent.h"

FileAgent::FileAgent()
{
	this->info = nullptr;
	this->file = new File;
}

FileAgent::FileAgent(char* filename)
{
	this->info->filename = filename;
	this->file = new File;
}

FileAgent::~FileAgent()
{
	delete this->file;
}
