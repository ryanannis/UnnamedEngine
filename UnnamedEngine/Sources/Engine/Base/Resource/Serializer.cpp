#include "Serializer.h"

#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/PropParser.h"

Serializer& Serializer::Serialize(ComponentBase& c)
{
	Serializer s;
	PropNode serializedChild = s.Serialize(c).GetNode();
	mSerializedNode.props.push_back(serializedChild);
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, std::string in)
{
	mSerializationTree.leaves.emplace( tag, CreateLeaf(in) );
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, Resource& res)
{
	mSerializationTree.leaves.emplace(tag, CreateLeaf(Serialize(res)));
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, int in)
{
	mSerializationTree.leaves.emplace(tag, CreateLeaf(Serialize(in)));
	return(*this);
}

std::string Serializer::Serialize(std::string in) const
{
	return("\"" + in + "\"");
}
std::string Serializer::Serialize(int in) const
{
	return(std::to_string(in));
}

std::string Serializer::Serialize(Resource& res) const
{
	return(res.GetURI());
}

PropTreeLeaf Serializer::CreateLeaf(std::string val) const
{
	std::vector<UDFToken> kv{ UDFToken(TokenType::NONE, val) };
	return(PropTreeLeaf(std::move(kv)));
}