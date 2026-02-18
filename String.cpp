#include "String.h"

String::String(const std::string& str)
{
	this->_value = str;
}

bool String::isPrintable() const
{
	return true;
}

std::string String::toString() const
{
	return this->_value;
}
