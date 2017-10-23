#include "Context.h"

GLFWwindow* Context::GetWindow() const
{
	return(mWindow);
}

void Context::SetWindow(GLFWwindow* window)
{
	GLFWwindow = window;
}