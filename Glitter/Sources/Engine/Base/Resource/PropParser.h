#pragma once
#include "Engine/Base/Common/Common.h"

#include <string>

class PropParser
{
public:
	PropParser();

private:
	enum TokenType
	{
		NONE,
		LBRACKET,
		RBRACKET,
		LCURLY,
		RCURLY,
		COLON,
		ALPHANUM,
		FILEPATH,
		EQUALS,
	};

	struct UDFToken
	{
		UDFToken(TokenType type, std::string value) : type(type), value(value) {}
		TokenType type;
		std::string value;
	};

	void Tokenize(std::string str, std::vector<PropParser::UDFToken>& tokens) const;
};
