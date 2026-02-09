#pragma once

#include "Type.h"

class Boolean : public Type
{
public:
	Boolean(bool value);

	bool isPrintable() const override;
	std::string toString() const override;

private:
	bool _value;
};