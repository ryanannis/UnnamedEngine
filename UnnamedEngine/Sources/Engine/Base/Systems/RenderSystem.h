#pragma once

#include "Engine/Base/Types/System.h"
#include "Engine/Base/Components/RenderComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Components/CameraComponent.h"

class RenderSystem : public System<RenderSystem> {
public:
	virtual void Update(float dt, Ptr<EntityAdmin> entityAdmin);
};