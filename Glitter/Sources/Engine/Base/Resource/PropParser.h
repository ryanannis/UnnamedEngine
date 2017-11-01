#pragma once
#include "Engine/Base/Common/Common.h"

#include<string>

class PropParser
{
public:
	PropParser();

private:
	enum TokenType
	{
		LBRACKET,
		RBRACKET,
		LCURLY,
		RCURLY,
		ALPHANUM,
		EQUALS
	};

	struct Token
	{
		TokenType t;
		std::string value;
	};

	std::vector<Token> PropParser::Tokenize(std::string str) const;

};
