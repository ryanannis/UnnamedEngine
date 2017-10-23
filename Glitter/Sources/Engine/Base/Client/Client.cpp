#include "Engine/Base/Common/Common.h"

#include "Client.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

Client::Client() :
	mShouldTerminate(false),
	mContext()
{

}

Client::~Client()
{

}

void Client::Initialize()
{
	auto window = InitializeWindow();
	mContext.SetWindow(window);
}

const Context& Client::GetContext() const
{
	return(mContext);
}

GLFWwindow* Client::InitializeWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);

	if(window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	return(window);
}

void Client::Terminate()
{
	mShouldTerminate = true;
}

void Client::Run()
{
	// Rendering Loop
	while(!mShouldTerminate){
		
		// Temp
		if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			mShouldTerminate = true;
		}

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwDestroyWindow(mWindow);
}