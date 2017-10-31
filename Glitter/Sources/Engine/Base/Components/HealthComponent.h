#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"

class HealthComponent : public Component<HealthComponent>
{
public:
	int mMaxHealth = 250;
	int mCurrentHealth = 250;

	virtual Serializer& Serialize(Serializer& s) override;
};

template<> std::string Component<HealthComponent>::sName = "HealthComponent";
