#pragma once
#include "Engine/Base/Common/Common.h"

#include <string>
#include <optional>

struct PropTree;
struct UDFToken;
class PropTreeLeafBase;

class PropParser
{
public:
	PropParser();
	static std::optional<PropTree> Parse(std::string str);

	static void Tokenize(std::string str, std::vector<UDFToken>& tokens);
	static void ParserFailed(int lineNumber, std::string around);
	static bool ParseValueTokens(const PropTree& tree, const std::string& identifier, const std::vector<UDFToken>& valueTokens);
	static std::optional<PropTree> ParseTokens(std::vector<UDFToken>& tokens);
};

