#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Client/InputEvent.h"

struct PropTree;

class InputComponent : public Component<InputComponent>
{
public:
	InputComponent() = default;

	std::vector<InputEvent> inputEvents;
};

template<> std::string Component<InputComponent>::sName = "InputComponent";
template<> StorageStrategy Component<InputComponent>::sStorageStrategy = StorageStrategy::Singleton;
