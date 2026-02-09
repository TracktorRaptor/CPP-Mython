#include "Type.h"

Type::Type()
{
	this->_isTemp = false;
}

Type::~Type()
{
}

bool Type::getIsTemp() const
{
	return this->_isTemp;
}

void Type::setIsTemp(bool newValue)
{
	this->_isTemp = newValue;
}
