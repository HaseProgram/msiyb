#pragma once
#include <exception>

class Exception : public std::exception
{
public:
	virtual const char* what() const = 0;
};