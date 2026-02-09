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
		throw SyntaxException();
	}

	return type;
}

Type* Parser::getType(std::string& str)
{
	if (Helper::isInteger(str))
	{
		Helper::trim(str);
		Helper::removeLeadingZeros(str);
		Integer* intNumber = new Integer(std::stoi(str));;
		intNumber->setIsTemp(true);
		return intNumber;
	}

	else if (Helper::isBoolean(str))
	{
		Helper::trim(str);
		bool boolValue = (str == "True") ? true : false;
		Boolean* boolObject = new Boolean(boolValue);
		boolObject->setIsTemp(true);
		return boolObject;
	}

	else if (Helper::isString(str))
	{
		Helper::trim(str);

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

//bool Parser::makeAssignment(std::string str)
//{
//	return false;
//}
//
//Type* Parser::getVariableValue(std::string str)
//{
//	return nullptr;
//}


