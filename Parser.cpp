#include "Parser.h"
#include <iostream>

Type* Parser::parseString(std::string str)
{
	Type* type = nullptr;

	// add check for empty string (just enter without anything)

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
		std::string optimizedString = "";
		bool inTheString = false; // Used to remove excess spaces and optimize the inputted string
		int curIndex = 0;
		char curLetter = ' ';

		// Quatation string - string with "" on both sides
		if (str[0] == '"' && str[str.length() - 1] == '"')
		{
			optimizedString = "\""; // Adding a " to the start of the string and skipping it in the loop later
			bool containsApostrophe = str.find('\'') != std::string::npos ? true : false;

			// Skipping the first " and the last "
			for (curIndex = 1; curIndex < str.length()-1; ++curIndex)
			{
				curLetter = str[curIndex];
				if (curLetter == '"') // Found a Quatation
				{
					if (!inTheString) // Getting into the string
					{
						inTheString = true;
					}
					
					else // Getting out of the string
					{
						inTheString = false;
					}
				}

				if (inTheString && curLetter != '"')
				{
					optimizedString += curLetter;
				}
			}

			optimizedString.push_back('"'); // Adding a " at the back

			// Replacing the "" With '' if the string doesn't have ' in the middle
			if (!containsApostrophe)
			{
				// Replacing the "" with ''
				str[str.find('"')] = '\'';
				str[str.find('"')] = '\'';
			}

			String* stringObject = new String(optimizedString);
			stringObject->setIsTemp(true);
			return stringObject;
		}

		// Apostrophe string - string with '' on both sides
		else if ((str[0] == '\'' && str[str.length() - 1] == '\''))
		{
			optimizedString = "'"; // Adding a ' to the start of the string and skipping it in the loop later

			// Skipping the first ' and the last '
			for (curIndex = 1; curIndex < str.length()-1; ++curIndex)
			{
				curLetter = str[curIndex];
				if (curLetter == '\'') // Found a Apostrophe
				{
					if (!inTheString) // Getting into the string
					{
						inTheString = true;
					}

					else // Getting out of the string
					{
						inTheString = false;
					}
				}

				if (inTheString && curLetter != '\'')
				{
					optimizedString += curLetter;
				}
			}

			optimizedString.push_back('\''); // Adding a " at the back

			String* stringObject = new String(optimizedString);
			stringObject->setIsTemp(true);
			return stringObject;
		}
	}

	// The variable is a list
	else if (str[0] == '[' && str[str.length() - 1] == ']')
	{
		bool ignoreComma = false; // If we are reading from a list/string/etc which have commas that we want to ignore
		bool QuatationString = false; // String variable starts with "
		bool ApostropheString = false;	  // String variable starts with '
		std::string curVarStr = ""; // Holds the current variable from the gotten string as a string
		Type* curVar = nullptr;
		List* list = nullptr;
		std::deque<Type*> listVariables;
		int curIndex = 0;
		char curLetter = ' ';

		for (curIndex = 1; curIndex < str.length(); ++curIndex)
		{	
			curLetter = str[curIndex];

			// String literal detection
			if (curLetter == '\'' || curLetter == '"')
			{
				// Aphostrophe - '' string literal
				if (curLetter == '\'')
				{
					if (!ApostropheString && !QuatationString)
					{
						ApostropheString = true;
						ignoreComma = true;
					}

					else if (ApostropheString && !QuatationString)
					{
						ApostropheString = false;
						ignoreComma = false;
					}
				}

				// Quatation - "" string literal
				else
				{
					if (!ApostropheString && !QuatationString)
					{
						QuatationString = true;
						ignoreComma = true;
					}

					else if (!ApostropheString && QuatationString)
					{
						QuatationString = false;
						ignoreComma = false;
					}
				}
			}

			if (curLetter == '[' /*|| letter == '('*/ && !ignoreComma)
			{
				ignoreComma = true;
			}

			else if (curLetter == ']' /*|| letter == ')'*/ && ignoreComma)
			{
				ignoreComma = false;
			}
			
			// Add the found variable to the variables deque
			if ((curLetter == ',' && !ignoreComma) || curIndex == str.length()-1)
			{
				Helper::trim(curVarStr); // Removing excess spaces if any
				curVar = getType(curVarStr); // Checking if the current variable is an object literal
				if (curVar) 
				{
					listVariables.push_back(curVar);
					curVar = nullptr;
					curVarStr = "";
				}

				else if (!curVar) // The variable isn't an object literal, checking referencing to existing objects
				{
					try
					{
						curVar = getType(_variables.at(curVarStr)->toString());
						listVariables.push_back(curVar);
						curVar = nullptr;
						curVarStr = "";
					}
					// If entered, the current variable is invalid
					catch (...)
					{
						listVariables.clear(); // Deleting all objects created
						throw NameErrorException(curVarStr);
					}
				}

				continue; // Skip the comma adding
			}

			// Adding another letter from the variable/object literal name
			if (curLetter != ',' || ignoreComma)
			{
				curVarStr += curLetter;
			}
		}

		list = new List(listVariables);
		return list;
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
		std::string variableName = str.substr(0, str.find("="));
		std::string variableValue = str.substr(str.find("=") + 1, str.length()-str.find("=")-1);

		Helper::rtrim(variableName); // Triming the excess spaces at the end of the name
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
				// Checking if the assigning name already exists (deleting its value) and isn't self assigning
				if (isExistingVar(variableName) && variableName != variableValue)
				{
					delete _variables.at(variableName);
					_variables.erase(variableName);
				}

				// Self assignment
				if (isExistingVar(variableName) && variableName == variableValue)
				{
					return true;
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


