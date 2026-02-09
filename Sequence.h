#pragma once

#include "Type.h"

class Sequence : public Type
{
public:
//	Sequence(void* arrOfValue);
//
	bool isPrintable() const override;
	std::string toString() const override;
//
//private:
//	void* _value;
};