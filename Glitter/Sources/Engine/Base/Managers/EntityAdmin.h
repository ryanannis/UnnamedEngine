#pragma once
#include "Engine/Base/Common/Common.h"

#include <map>

#include "Engine/Base/Managers/SystemAdmin.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"

class EntityAdmin : public NonCopyable
{
public:
	EntityAdmin();
	~EntityAdmin();

	void Update(const float delta);

private:
	void RegisterEntity(const Entity const* entity);

	SystemAdmin mSystemAdmin;

	std::vector<Entity> mEntities;
	std::map<uint32_t, Entity*> mEntityMap;
};