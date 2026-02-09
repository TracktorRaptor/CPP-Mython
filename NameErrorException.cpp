#include "NameErrorException.h"

NameErrorException::NameErrorException(std::string name)
{
	this->_name = name;
	std::stringstream ss;
	ss << "NameError: name '" + this->_name + "' is not defined\n";
	this->_name = ss.str();
}

const char* NameErrorException::what() const noexcept
{
	return this->_name.c_str();
}
