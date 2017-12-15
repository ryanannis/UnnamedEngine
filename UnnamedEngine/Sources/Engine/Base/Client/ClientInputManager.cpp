#include "ClientInputManager.h"

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <functional>

#include "Engine/Base/Client/Client.h"
#include "Engine/Base/Client/Keycodes.h"
#include "Engine/Base/Client/Context.h"

//////////////////////////////////////////////////////////
// GLFW C-STYLE INTERFACE

static int NumInstances = 0;

// Interfacing with evil C-style void pointers
static ClientInputManager* sActiveInstance;



#pragma warning( push )
#pragma warning( disable : 4100)
void ClientInputManager::RecieveGLFWInput(GLFWwindow* self, int keycode, int scancode, int action, int mods)
{
	InputEvent event(
		keycode,
		action == GLFW_PRESS ? InputState::INPUT_DOWN : InputState::INPUT_UP
	);

	sActiveInstance->QueueInput(event);
}
void ClientInputManager::RecieveGLFWMouseInput(GLFWwindow* window, double xpos, double ypos)
{
	Vector2f mouseMovement(xpos, ypos);
	sActiveInstance->QueueMouseInput(mouseMovement);
}
#pragma warning( pop ) 

//////////////////////////////////////////////////////////

ClientInputManager::ClientInputManager(Ptr<Context> context) :
	mContext(context),
	mLastMousePosition(Vector2f(0.f,0.f)),
	mSecondLastMousePosition(Vector2f(0.f, 0.f))
{
	/* We shouldn't ever need more than one of these active at once.
	 * If we do...  It's time to deal with the GLFW C-style fpointer 
	 * with a static map.
	 */
	assert(NumInstances == 0);
	NumInstances++;
}

ClientInputManager::~ClientInputManager()
{
	NumInstances--;
}

void ClientInputManager::QueueInput(InputEvent e)
{
	mInputQueue.emplace(e);
}

void ClientInputManager::QueueMouseInput(Vector2f pos)
{
	mSecondLastMousePosition = mLastMousePosition;
	mLastMousePosition = pos;
	InputEvent eventX(
		MOUSE_X.keycode,
		InputState::MOUSE_MOVEMENT,
		mLastMousePosition.x - mSecondLastMousePosition.x
	);
	mInputQueue.push(eventX);

	InputEvent eventY(
		MOUSE_Y.keycode,
		InputState::MOUSE_MOVEMENT,
		mLastMousePosition.y - mSecondLastMousePosition.y
	);
	mInputQueue.push(eventY);
}

void ClientInputManager::Initialize()
{
	sActiveInstance = this;

	glfwSetCursorPosCallback(
		mContext->GetClient()->GetGLFWContext(), 
		&RecieveGLFWMouseInput
	);
	glfwSetKeyCallback(
		mContext->GetClient()->GetGLFWContext(),
		&RecieveGLFWInput
	);
}

void ClientInputManager::Update()
{
	glfwPollEvents();
}

bool ClientInputManager::HasInput() const
{
	return(!mInputQueue.empty());
}

bool ClientInputManager::KeyIsDown(Keycode k) const
{
	return(glfwGetKey(mContext->GetClient()->GetGLFWContext(), k));
}

InputEvent ClientInputManager::GetInputEvent()
{
	assert(!mInputQueue.empty());
	InputEvent e = mInputQueue.front();
	mInputQueue.pop();
	return(e);
}
