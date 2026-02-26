#include "Type.h"
#include "InterpreterException.h"
#include "Parser.h"
#include <iostream>

#define WELCOME "Welcome to Magshimim Python Interperter version 1.0 by "
#define YOUR_NAME "Adam Karkpenko Kovach"

using std::cout;
using std::endl;
using std::cin;

/*
Notes for one's self - To do:
1. String - Add edge case for invalid string - " (only one quote)
2. Change ints so a really big number could work - 999999999999999999999999999999999999999999999999999 for example
3. Add indentations
4. 
*/

std::unordered_map<std::string, Type*> Parser::_variables;

int main(int argc,char **argv)
{
	cout << WELCOME << YOUR_NAME << " :D\n" << endl;

	std::string input_string = "";
	Type* type = nullptr;

	// get new command from user
	cout << ">>> ";
	std::getline(cin, input_string);
	
	while (input_string != "quit()")
	{
		// parsing command
		try
		{
			type = Parser::parseString(input_string);
		}
		catch (std::exception& e)
		{
			cout << e.what();
		}

		if (type)
		{
			if (type->isPrintable())
			{
				cout << type->toString() << endl;
			}

			if (type->getIsTemp())
			{
				delete type;
				type = nullptr;
			}
		}

		// get new command from user
		std::cout << ">>> ";
		std::getline(std::cin, input_string);
	}

	Parser::cleanAllocatedVariables();

	return 0;
}
