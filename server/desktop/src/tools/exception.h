#pragma once
#include <exception>

class Exception : public std::exception
{
public:
	char* message;

	Exception(char* message, int errnum = -1, const char* = nullptr, int line = -1)
	{
		this->message = message;
		//log this shit
	}

	Exception()
	{

	}

	virtual const char* what()
	{
		return this->message;
	}
};