#include <Catch.h>
#include "Engine/Base/Resource/PropParser.h"
#include "Engine/Base/Resource/PropTree.h"


const char *text = "char = { test }";
const char *text2 = "char { int = 5; float = 5.0;  best = \"HELLO\"; subarray { filepath = ./hello/test.txt;} }";


TEST_CASE("Prop parsing works correctly", "[propparser]") {
	std::vector<UDFToken> tokens;
	PropParser::Tokenize(std::string(text2), tokens);
	
	/*std::vector<UDFToken> tokens2;
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
	REQUIRE(fullyParsed.components[0].components[0].leaves[0].value == "5");*/
}
