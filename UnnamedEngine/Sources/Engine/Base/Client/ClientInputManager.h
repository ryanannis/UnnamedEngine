#pragma once
#include "Engine/Base/Common/Ptr.h"

#include <deque>
#include <queue>

#include "Engine/Base/Client/InputEvent.h"

struct GLFWwindow;
class Context;

class ClientInputManager
{
public:
	explicit ClientInputManager(Ptr<Context> context);
	~ClientInputManager();
	
	void Initialize();
	void Update();
	bool HasInput() const;
	InputEvent GetInputEvent();


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
