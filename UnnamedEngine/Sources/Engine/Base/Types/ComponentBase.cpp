#include "ComponentBase.h"

#include "Engine/Base/Resource/Serializer.h"

// Blank defs are provided since not everything is necessarily serializable
Serializer& ComponentBase::Serialize(Serializer& s)
{
	return(s);
};

void ComponentBase::Deserialize(const DeserializationData&)
{
}