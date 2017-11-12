#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/StaticComponent.h"

struct PropTree;

class HealthComponent : public Component<HealthComponent>
{
public:
	HealthComponent();

	int maxHealth;
	int currentHealth;
	
	virtual Serializer& Serialize(Serializer& s) override;
	virtual void Deserialize(const PropTree& t) override;
};

STATICDECL(HealthComponent, "HealthComponent");


template<> std::string Component<HealthComponent>::sName = "HealthComponent";