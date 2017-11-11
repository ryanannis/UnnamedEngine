#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/StaticComponent.h"

struct PropTree;

class HealthComponent : public Component<HealthComponent>
{
public:
	HealthComponent() {};
	int maxHealth;
	int currentHealth;
	
	virtual Serializer& Serialize(Serializer& s) = 0;
	virtual void Deserialize(PropTree& t) = 0;
};

template<> std::string Component<HealthComponent>::sName = "HealthComponent";

STATICREGISTER(HealthComponent, "HealthComponent");