#pragma once

#include <queue>

#include "Engine/Base/Client/Context.h"

class ClientInputManager
{
public:
	ClientInputManager(Context& context);
	~ClientInputManager();

	void Update();

private:
	std::queue<InputEvent> mInputQueue;

	// Disable copying
	ClientInputManager& operator=(const ClientInputManager&) = delete;
	ClientInputManager(const ClientInputManager&) = delete;
};