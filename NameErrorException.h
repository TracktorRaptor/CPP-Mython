#pragma once

#include "InterpreterException.h"
#include <sstream>
#include <string>

class NameErrorException : public InterpreterException
{
public:
    explicit NameErrorException(std::string name);
	virtual const char* what() const noexcept;

private:
	std::string _name;
};