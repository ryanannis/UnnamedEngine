#pragma once
#include "Engine/Base/Common/Common.h"

#include <string>
#include <optional>

struct PropTree;
struct UDFToken;
struct PropTreeLeaf;

class PropParser
{
public:
	PropParser();
	static std::optional<PropTree> Parse(std::string str);
private:
	static void Tokenize(std::string str, std::vector<UDFToken>& tokens);
	static void ParserFailed(int lineNumber, std::string around);
	static PropTreeLeaf ParseValueTokens(std::vector<char> valueTokens, bool& status);
	static std::optional<PropTree> ParseTokens(std::vector<UDFToken>& tokens);
};

