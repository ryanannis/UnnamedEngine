#pragma once
#include "Engine/Base/Common/Common.h"

#include <string>

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

class PropParser
{
public:
	PropParser();
	static void Tokenize(std::string str, std::vector<UDFToken>& tokens);
	static void ParserFailed();
};
