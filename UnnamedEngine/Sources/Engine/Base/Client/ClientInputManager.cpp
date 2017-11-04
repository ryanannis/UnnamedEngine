#include "ClientInputManager.h"

#include <GLFW/glfw3.h>

#include <array>
#include <functional>

#include "Engine/Base/Client/Client.h"
#include "Engine/Base/Client/Keycodes.h"

//////////////////////////////////////////////////////////
// GLFW C-STYLE INTERFACE

static int NumInstances = 0;

// Interfacing with evil C-style void pointers
static ClientInputManager* sActiveInstance;

void ClientInputManager::RecieveGLFWInput(GLFWwindow* self, int keycode, int scancode, int action, int mods)
{
	InputEvent event(
		keycode,
		action == GLFW_PRESS ? InputState::INPUT_DOWN : InputState::INPUT_UP
	);

	sActiveInstance->QueueInput(event);
}

//////////////////////////////////////////////////////////

ClientInputManager::ClientInputManager(Ptr<Context> context) :
	mContext(context)
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

void ClientInputManager::Initialize()
{
	sActiveInstance = this;

	glfwSetKeyCallback(
		mContext->GetClient()->GetGLFWContext(),
		&RecieveGLFWInput
	);
}

void ClientInputManager::Update()
{
	
}