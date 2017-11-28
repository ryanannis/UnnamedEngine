#include "PropTree.h"
#include "Engine/Base/Resource/PropParser.h"

// For 'URI'
#include "Engine/Base/Resource/ResourceManager.h"

#include <sstream>

PropTreeLeaf::PropTreeLeaf(std::vector<UDFToken>&& tokens) :
	mTokens{tokens}
{}

std::string PropTreeLeaf::GetAsString() const
{
	std::stringstream s;
	
	for(UDFToken token : mTokens)
	{
		s << token.value;
	}

	return(s.str());
}

std::optional<float> PropTreeLeaf::GetAsFloat() const
{
	// there is room for improvement here
	std::stringstream ss(GetAsString());
	float fl;
	ss >> fl;
	return(fl); // todo error handling
}

std::optional<uint32_t> PropTreeLeaf::GetAsInt() const
{
	std::stringstream ss(GetAsString());
	int i;
	ss >> i;
	return(i); // todo error handling
}

std::optional<URI> PropTreeLeaf::GetAsURI() const
{
	std::stringstream ss(GetAsString());
	return(URI(ss.str())); // todo error handling
}

std::optional<Vector3f> PropTreeLeaf::GetAsVector() const
{
	assert(mTokens.size() == 7);
	assert(mTokens[0].type == ParsedTokenType::LSQUARE);
	// the parser recognizes a float as filepath (eg. folder/12321.23232 is valid)
	// as we aren't a context free grammar and have no string delimeters...  yeaaahhhh
	assert(mTokens[1].type == ParsedTokenType::FILEPATH || mTokens[1].type == ParsedTokenType::ALPHANUM);
	assert(mTokens[2].type == ParsedTokenType::COMMA);
	assert(mTokens[3].type == ParsedTokenType::FILEPATH || mTokens[3].type == ParsedTokenType::ALPHANUM);
	assert(mTokens[4].type == ParsedTokenType::COMMA);
	assert(mTokens[5].type == ParsedTokenType::FILEPATH || mTokens[5].type == ParsedTokenType::ALPHANUM);
	assert(mTokens[6].type == ParsedTokenType::RSQUARE);


	return(
		Vector3f(
			std::stof(mTokens[1].value),
			std::stof(mTokens[3].value),
			std::stof(mTokens[5].value)
		)
	);
}