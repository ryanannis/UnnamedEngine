#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/PropTree.h"

#include <string>
#include <optional>

enum class TokenType
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
	SEMI,
};

struct UDFToken
{
	UDFToken(TokenType type, std::string value) : type(type), value(value) {}
	const TokenType type;
	const std::string value;
};

class PropParser
{
public:
	PropParser();
	static void Tokenize(std::string str, std::vector<UDFToken>& tokens);
	static void PropParser::ParserFailed(int lineNumber, std::string around);
	static std::optional<PropTree> ParseTokens(std::vector<UDFToken>& tokens);
};
