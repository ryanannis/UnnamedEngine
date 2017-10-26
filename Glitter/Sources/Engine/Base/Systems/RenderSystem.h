#pragma once

#include "Engine/Base/Types/System.h"
#include "Engine/Base/Components/RenderComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Components/CameraComponent.h"


// let's try to keep this lightweight to compile time murder

class RenderSystem : public System {
public:
	virtual void RegisterEntity(const Entity& e) override;
	virtual void Update(const float delta) override;
private:
};