#include "Boolean.h"

Boolean::Boolean(bool value)
{
	this->_value = value;
}

bool Boolean::isPrintable() const
{
	return true;
}

std::string Boolean::toString() const
{
	return (this->_value == false) ? "False" : "True";
}
