#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/StaticComponent.h"
struct PropTree;

class IdentityComponent : public Component<IdentityComponent>
{
public:
	IdentityComponent();
	std::string entityName;

	Serializer& Serialize(Serializer& s);
	void Deserialize(const DeserializationData& s);
};

template<> std::string Component<IdentityComponent>::sName = "IdentityComponent";
template<> StorageStrategy Component<IdentityComponent>::sStorageStrategy = StorageStrategy::HashMap;
;