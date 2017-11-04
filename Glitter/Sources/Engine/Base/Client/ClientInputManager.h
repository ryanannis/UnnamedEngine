#pragma once

#include <deque>
#include <queue>

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/InputEvent.h"

class GLFWwindow;

class ClientInputManager
{
public:
	explicit ClientInputManager(Ptr<Context> context);
	~ClientInputManager();
	
	void Initialize();
	void Update();

private:
	void QueueInput(InputEvent e);
	static void RecieveGLFWInput(GLFWwindow* self, int keycode, int scancode, int action, int mods);
	Ptr<Context> GetContext() { return(mContext); }
	
	std::queue<InputEvent> mInputQueue;

	Ptr<Context> mContext;

	// Disable copying
	ClientInputManager& operator=(const ClientInputManager&) = delete;
	ClientInputManager(const ClientInputManager&) = delete;
};
