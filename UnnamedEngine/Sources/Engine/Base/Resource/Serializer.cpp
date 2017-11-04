#include "Serializer.h"

Serializer& Serializer::Serialize(ComponentBase& c)
{
	Serializer s;
	PropNode serializedChild = s.Serialize(c).GetNode();
	mSerializedNode.props.push_back(serializedChild);
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, std::string in)
{
	mSerializedNode.key = tag;
	mSerializedNode.value = in;
	return(*this);
}

Serializer& Serializer::Serialize(std::string tag, int in)
{
	return(Serialize("tag", Serialize(in)));
}

std::string Serializer::Serialize(std::string in) const
{
	return("\"" + in + "\"");
}
std::string Serializer::Serialize(int in) const
{
	return(std::to_string(in));
}

PropNode Serializer::GetNode() const
{
	return(mSerializedNode);
}