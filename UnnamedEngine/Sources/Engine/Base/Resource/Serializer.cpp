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
	mSerializationTree.AddLeaf(tag, in);
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, const Vector3f& vec)
{
	mSerializationTree.AddLeaf(tag, vec);
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, int in)
{
	mSerializationTree.AddLeaf(tag, in);
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
