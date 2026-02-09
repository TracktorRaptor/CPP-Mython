#pragma once
#include "InterpreterException.h"
#include "IndentationException.h"
#include "SyntaxException.h"
#include "Type.h"
#include "Helper.h"
#include "Integer.h"
#include "Boolean.h"
#include "String.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>


class Parser
{
public:
    static Type* parseString(std::string str);
    static Type* getType(std::string& str);

private:
    static std::unordered_map<std::string, Type> _variables;

    static bool isLegalVarName(std::string str);
    static bool makeAssignment(std::string str);
    static Type* getVariableValue(std::string str);

};
