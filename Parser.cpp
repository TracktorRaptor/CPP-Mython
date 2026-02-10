#include "Parser.h"
#include <iostream>

Type* Parser::parseString(std::string str)
{
	Type* type = nullptr;

	if (str[0] == '\t')
	{
		throw IndentationException();
	}

	if (str[0] == ' ')
	{
		throw IndentationException();
	}

	type = getType(str);
	if (!type)
	{
		if (makeAssignment(str))
		{
			Void* variable = new Void();
			variable->setIsTemp(true);
		}

		else
		{
			type = getVariableValue(str);

			if (!type)
			{
				throw SyntaxException();
			}
		}		
	}

	return type;
}

Type* Parser::getType(std::string str)
{
	Helper::trim(str);

	if (Helper::isInteger(str))
	{
		Helper::removeLeadingZeros(str);
		Integer* intNumber = new Integer(std::stoi(str));;
		intNumber->setIsTemp(true);
		return intNumber;
	}

	else if (Helper::isBoolean(str))
	{
		bool boolValue = (str == "True") ? true : false;
		Boolean* boolObject = new Boolean(boolValue);
		boolObject->setIsTemp(true);
		return boolObject;
	}

	else if (Helper::isString(str))
	{
		// String with "" on the two sides
		if (str[0] == '"' && str[str.length() - 1] == '"')
		{
			int quotesCount = 0;

			for (char letter : str)
			{
				if (letter == '"')
				{
					quotesCount++;
				}
			}

			// If there are only the "" for the two sides of the string and non in the middle
			if (quotesCount == 2)
			{
				// Replacing the "" with ''
				int quoteIndex = str.find('"');
				str[quoteIndex] = '\''; 
				quoteIndex = str.find('"');
				str[quoteIndex] = '\'';

				String* stringObject = new String(str);
				stringObject->setIsTemp(true);
				return stringObject;
			}
		}

		else if ((str[0] == '\'' && str[str.length() - 1] == '\''))
		{
			int quotesCount = 0;

			for (char letter : str)
			{
				if (letter == '\'')
				{
					quotesCount++;
				}
			}

			// If there are only the '' for the two sides of the string and non in the middle
			if (quotesCount == 2)
			{
				String* stringObject = new String(str);
				stringObject->setIsTemp(true);
				return stringObject;
			}
		}
	}

	return nullptr;
}

bool Parser::isLegalVarName(std::string str)
{
	for (char character : str)
	{
		// If the letter is different than an English letter, number, or underscore 
		if (!Helper::isDigit(character) && !Helper::isUnderscore(character) && !Helper::isLetter(character))
		{
			return false;
		}
	}

	// The first letter is a number
	if (Helper::isDigit(str[0]))
	{
		return false;
	}

	// All checks were passed and no illegal characters were found
	return true;
}

bool Parser::makeAssignment(std::string str)
{
	bool isEqualsFound = str.find("=") != std::string::npos ? true : false; // Checking if str is a variable assignment
	bool isAssignment = false;
	Type* variable = nullptr; // Variable to hold the found type of the variable in the str

	// Only if the str is a variable assignment continue to assigning
	if (isEqualsFound)
	{
		// Getting the variable name and value
		std::string variableName = str.substr(0, str.find("=") - 1);
		std::string variableValue = str.substr(str.find("=") + 1, str.length() - 1);

		Helper::ltrim(variableName); // Triming the excess spaces at the end of the name
		Helper::trim(variableValue); // Triming the excess spaces at the begining and end of the value

		// Checking if the name is legal, if not - throw exception
		if (!isLegalVarName(variableName))
		{
			throw SyntaxException();
		}

		// Getting the type of the variable and allocating it on the heap if is a legal type
		variable = getType(variableValue);

		// Incase of an illegal type, checking for an already existing var assignment
		if (!variable)
		{
			// Checking if the assignment is towards another already existing variable
			if (isExistingVar(variableValue))
			{
				// Checking if the assigning name already exists
				if (isExistingVar(variableName))
				{
					delete _variables.at(variableName);
					_variables.erase(variableName);
				}

				variable = getVariableValue(variableValue);
				variable = getType(variable->toString());
				variable->setIsTemp(false); // The variable we are assigning isn't a temporary one
				_variables.insert({ variableName, variable });
				return true;
			}

			else
			{
				throw NameErrorException(variableValue);
			}
		}
		
		variable->setIsTemp(false); // The variable we are assigning isn't a temporary one

		// All checks passed and is a legal assignment sentence, insert the variable to the variables unordered_map
		// If the variable doesn't already exist
		if (_variables.find(variableName) == _variables.end())
		{
			_variables.insert({ variableName, variable});
		}

		else
		{
			delete _variables.at(variableName); // Deleting old value object
			_variables.at(variableName) = variable;
		}

		// The variable assignment was a success
		isAssignment = true;
	}

	return isAssignment;
}

bool Parser::isExistingVar(std::string str)
{
	return _variables.find(str) != _variables.end() ? true : false;
}

Type* Parser::getVariableValue(std::string str)
{
	Type* variable = nullptr;

	// Variable already exists
	if (_variables.find(str) != _variables.end())
	{
		variable = _variables.at(str);
	}

	else
	{
		throw NameErrorException(str);
	}

	return variable;
}

void Parser::cleanAllocatedVariables()
{
	for (auto it : _variables)
	{
		delete it.second;
	}
}


