#pragma once

#include <queue>

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/InputEvent.h"

class ClientInputManager
{
public:
	ClientInputManager(const Context& context);
	~ClientInputManager();

	void Update();

private:
	std::queue<InputEvent> mInputQueue;

	// Disable copying
	ClientInputManager& operator=(const ClientInputManager&) = delete;
	ClientInputManager(const ClientInputManager&) = delete;
};