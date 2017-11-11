#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"

struct PropTree;

class IdentityComponent : public Component<IdentityComponent>
{
public:
	IdentityComponent() {}
	std::string entityName;

	Serializer& Serialize(Serializer& s);
	void Deserialize(PropTree& t);
};

template<> std::string Component<IdentityComponent>::sName = "IdentityComponent";

