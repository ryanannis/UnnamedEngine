#include "Serializer.h"

#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/PropParser.h"
#include "Engine/Base/Types/ComponentBase.h"

////////////////////////////////////////////////////////////////////
///////////////////////Serialization//////////////////////////////// 
////////////////////////////////////////////////////////////////////

Serializer::Serializer()
{
}

SerializationState Serializer::GetSerializationState()
{
	return(mSerializationState);
}

Serializer::Serializer(PropTree tree, SerializationState s) :
	mSerializationTree(tree),
	mSerializationState(s)
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

Serializer& Serializer::Serialize(std::string tag, const Vector3f& vec)
{
	mSerializationTree.leaves.emplace(tag, CreateLeaf(vec));
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
	std::vector<UDFToken> kv{ UDFToken(ParsedTokenType::NONE, val) };
	return(PropTreeLeaf(std::move(kv)));
}

PropTreeLeaf Serializer::CreateLeaf(const Vector3f& vec) const
{
	std::vector<UDFToken> kv{
		UDFToken(
			ParsedTokenType::NONE, 
			"[" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "]"
		) 
	};
	return(PropTreeLeaf(std::move(kv)));
}