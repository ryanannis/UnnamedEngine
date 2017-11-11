#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"

struct PropTree;

class HealthComponent : public Component<HealthComponent>
{
public:
	HealthComponent() {};
	int maxHealth;
	int currentHealth;
	
	Serializer& Serialize(Serializer& s);
	void Deserialize(PropTree& t);
};

template<> std::string Component<HealthComponent>::sName = "HealthComponent";