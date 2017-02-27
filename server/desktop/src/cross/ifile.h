#pragma once

class IFile
{
public:
	virtual bool Exists() = 0;
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual void Rename() = 0;
	virtual void Move() = 0;
	virtual void Delete() = 0;
	virtual int GetChar() = 0;
};