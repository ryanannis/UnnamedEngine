#include <Catch.h>
#include "Engine/Base/Resource/PropParser.h"

const char *text = "char = { test }";

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
}
