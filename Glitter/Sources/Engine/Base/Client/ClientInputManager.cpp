#include "ClientInputManager.h"

#include <GLFW/glfw3.h>
#include <array>

ClientInputManager::ClientInputManager(const Context& context)
{
}

ClientInputManager::~ClientInputManager()
{

}

void ClientInputManager::Update()
{
	glfwPollEvents();

}