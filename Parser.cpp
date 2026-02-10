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
	std::string strCopy = str;
	Helper::trim(strCopy);

	if (Helper::isInteger(strCopy))
	{
		Helper::removeLeadingZeros(strCopy);
		Integer* intNumber = new Integer(std::stoi(strCopy));;
		intNumber->setIsTemp(true);
		return intNumber;
	}

	else if (Helper::isBoolean(strCopy))
	{
		bool boolValue = (strCopy == "True") ? true : false;
		Boolean* boolObject = new Boolean(boolValue);
		boolObject->setIsTemp(true);
		return boolObject;
	}

	else if (Helper::isString(strCopy))
	{
		// String with "" on the two sides
		if (strCopy[0] == '"' && strCopy[strCopy.length() - 1] == '"')
		{
			int quotesCount = 0;

			for (char letter : strCopy)
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
				int quoteIndex = strCopy.find('"');
				strCopy[quoteIndex] = '\''; 
				quoteIndex = strCopy.find('"');
				strCopy[quoteIndex] = '\'';

				String* stringObject = new String(strCopy);
				stringObject->setIsTemp(true);
				return stringObject;
			}
		}

		else if ((strCopy[0] == '\'' && strCopy[strCopy.length() - 1] == '\''))
		{
			int quotesCount = 0;

			for (char letter : strCopy)
			{
				if (letter == '\'')
				{
					quotesCount++;
				}
			}

			// If there are only the '' for the two sides of the string and non in the middle
			if (quotesCount == 2)
			{
				String* stringObject = new String(strCopy);
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
	Type* variableType = nullptr; // Variable to hold the found type of the variable in the str

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
		variableType = getType(variableValue); 
		if (!variableType) // Incase of an illegal type, throw exception
		{
			throw SyntaxException();
		}

		// All checks passed and is a legal assignment sentence, insert the variable to the variables unordered_map
		// If the variable doesn't already exist
		if (_variables.find(variableName) == _variables.end())
		{
			_variables.insert({ variableName, variableType });
		}

		else
		{
			_variables.at(variableName) = variableType;
		}

		// The variable assignment was a success
		isAssignment = true;
	}

	return isAssignment;
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


