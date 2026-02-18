#include "List.h"

List::List(std::deque<Type*> variables)
{
	this->variables = variables;
}

bool List::isPrintable() const
{
	return true;
}

std::string List::toString() const
{
	return std::string();
}
