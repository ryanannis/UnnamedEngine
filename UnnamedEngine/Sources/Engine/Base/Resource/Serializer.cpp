#include "Serializer.h"

#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/PropParser.h"
#include "Engine/Base/Types/ComponentBase.h"

////////////////////////////////////////////////////////////////////
///////////////////////Deserialization//////////////////////////////
////////////////////////////////////////////////////////////////////

DeserializationData::DeserializationData(PropTree t, Ptr<ResourceManager> resourceManager) :
	mSerializationTree(t),
	mResourceManager(resourceManager)
{}

const PropTree& DeserializationData::GetProps() const
{
	return(mSerializationTree);
}
Ptr<ResourceManager> DeserializationData::GetResourceManager() const
{
	return(mResourceManager);
}

////////////////////////////////////////////////////////////////////
///////////////////////Serialization//////////////////////////////// 
////////////////////////////////////////////////////////////////////

Serializer::Serializer()
{
}

Serializer::Serializer(PropTree tree) :
	mSerializationTree(tree)
{
}

Serializer& Serializer::Serialize(ComponentBase& c)
{
	Serializer s;
	PropTree serializedChild = s.Serialize(c).mSerializationTree;
	mSerializationTree.components.emplace(c.GetName(), std::move(serializedChild));
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, std::string in)
{
	mSerializationTree.leaves.emplace(tag, CreateLeaf(in) );
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

PropTreeLeaf Serializer::CreateLeaf(std::string val) const
{
	std::vector<UDFToken> kv{ UDFToken(TokenType::NONE, val) };
	return(PropTreeLeaf(std::move(kv)));
}