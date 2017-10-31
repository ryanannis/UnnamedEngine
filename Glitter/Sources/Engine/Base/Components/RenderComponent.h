#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"

class RenderComponent : Component<RenderComponent> {

};

template<> std::string Component<RenderComponent>::sName = "RenderComponent";