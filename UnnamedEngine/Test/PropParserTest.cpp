#include <Catch.h>
#include "Engine/Base/Resource/PropParser.h"

const char *text = "char = { test }";
const char *text2 = "char { test = 5; best = HELLO; subarray { new = 5;} }";

/*
Changed interface - off to the test graveyard :)
TEST_CASE("Prop parsing works correctly", "[propparser]") {
	std::vector<UDFToken> tokens;
	PropParser::Tokenize(std::string(text), tokens);
	REQUIRE(tokens[0].value == "char");
	REQUIRE(tokens[0].type == TokenType::ALPHANUM);
	REQUIRE(tokens[1].value == "=");
	REQUIRE(tokens[1].type == TokenType::EQUALS);
	REQUIRE(tokens[2].value == "{");
	REQUIRE(tokens[2].type == TokenType::LCURLY);
	REQUIRE(tokens[3].value == "test");
	REQUIRE(tokens[3].type == TokenType::ALPHANUM);
	REQUIRE(tokens[4].value == "}");
	REQUIRE(tokens[4].type == TokenType::RCURLY);

	std::vector<UDFToken> tokens2;
	PropParser::Tokenize(std::string(text2), tokens2);
	PropTree fullyParsed = *PropParser::ParseTokens(tokens2);
	REQUIRE(fullyParsed.typeName == "Root");
	REQUIRE(fullyParsed.components[0].typeName == "char");
	REQUIRE(fullyParsed.components[0].leaves[0].key == "test");
	REQUIRE(fullyParsed.components[0].leaves[0].value == "5");
	REQUIRE(fullyParsed.components[0].leaves[1].key == "best");
	REQUIRE(fullyParsed.components[0].leaves[1].value == "HELLO");
	REQUIRE(fullyParsed.components[0].components[0].typeName == "subarray");
	REQUIRE(fullyParsed.components[0].components[0].leaves[0].key == "new");
	REQUIRE(fullyParsed.components[0].components[0].leaves[0].value == "5");
}
*/
