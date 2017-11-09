#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"

class IdentityComponent : public Component<IdentityComponent>
{
public:
	IdentityComponent(std::string name) : entityName{name} {}
	const std::string entityName;
};

template<> std::string Component<IdentityComponent>::sName = "IdentityComponent";
