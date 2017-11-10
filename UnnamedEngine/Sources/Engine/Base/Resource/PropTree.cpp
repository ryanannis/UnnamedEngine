#include "PropTree.h"
#include "Engine/Base/Resource/PropParser.h"
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
	float i;
	ss >> i;
	return(i); // todo error handling
}