#pragma once

#include "Type.h"

class Integer : public Type
{
public:
	Integer(int value);

	bool isPrintable() const override;
	std::string toString() const override;

private:
	int _value;
};