#pragma once

#include "Sequence.h"

class String : public Sequence
{
public:
	String(const std::string value);

	bool isPrintable() const override;
	std::string toString() const override;

private:
	std::string _value;

};