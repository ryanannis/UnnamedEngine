#pragma once

#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Types/StaticComponent.h"

class FatComponent : public Component<FatComponent>
{
public:
	FatComponent();

	std::array<int, 65> mFatness;
};

template<> std::string Component<FatComponent>::sName = "FatComponent";