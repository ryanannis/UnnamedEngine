#pragma once
#include "Engine/Base/Common/Common.h"

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
	bool KeyIsDown(Keycode k) const;
	InputEvent GetInputEvent();

private:
	void QueueInput(InputEvent e);
	void QueueMouseInput(Vector2f pos);
	static void RecieveGLFWInput(GLFWwindow* self, int keycode, int scancode, int action, int mods);
	static void RecieveGLFWMouseInput(GLFWwindow* window, double xpos, double ypos);
	Ptr<Context> GetContext() { return(mContext); }
	
	std::queue<InputEvent> mInputQueue;

	Vector2f mLastMousePosition;
	Vector2f mSecondLastMousePosition;

	Ptr<Context> mContext;

	// Disable copying
	ClientInputManager& operator=(const ClientInputManager&) = delete;
	ClientInputManager(const ClientInputManager&) = delete;
};
