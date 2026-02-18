#pragma once

#include "Sequence.h"
#include "deque"
#include "Helper.h"

class List : public Sequence
{
public:
	List(std::deque<Type*> variables);
	bool isPrintable() const override;
	std::string toString() const override;

private:
	std::deque<Type*> variables;
};