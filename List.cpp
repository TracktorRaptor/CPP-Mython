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
	std::string listString = "[";

	// Incase of empty list
	if (this->variables.empty())
	{
		return "[]";
	}

	for (const auto& it : this->variables)
	{
		listString += it->toString() + ", ";
	}

	listString[listString.length()-2] = ']';
	listString.pop_back();
	return listString;
}
