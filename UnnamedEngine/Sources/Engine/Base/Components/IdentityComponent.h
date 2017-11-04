#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"

class IdentityComponent : public Component<IdentityComponent>
{
public:
	std::string entityName = "Unnamed";

	virtual Serializer& Serialize(Serializer& s) override;
};

template<> std::string Component<IdentityComponent>::sName = "IdentityComponent";
