#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"

class HealthComponent : public Component<HealthComponent>
{
public:
	HealthComponent(int health) : mMaxHealth{ health }, mCurrentHealth{ health } {}
	const int mMaxHealth;
	int mCurrentHealth;

};

template<> std::string Component<HealthComponent>::sName = "HealthComponent";

Serializer Serialize(HealthComponent& c);