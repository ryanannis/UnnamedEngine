#include "Engine/Base/Common/Common.h"

#include "Client.h"
#include "Engine/Graphics/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
	InitializeWindow();
	InitializeRenderer();
}

void Client::InitializeRenderer()
{
}

void Client::InitializeWindow()
{
	glfwInit();
	// The GL stuff should really be in the renderer... 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);

	if(window == nullptr){
		std::cerr << "Failed to initialize OpenGL context." << std::endl;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	std::cerr << "Successfully initialized OpenGL context." << std::endl;
}

void Client::InitializeContext()
{
	mContext.SetClient(this);
}

void Client::Terminate()
{
	mShouldTerminate = true;
}

void Client::Run()
{
	// Rendering Loop
	while(!mShouldTerminate){
		mRenderer.Render();
	}

	glfwDestroyWindow(GetWindow());
}

GLFWwindow* Client::GetWindow()
{
	return(mWindow);
}

Renderer* Client::GetRenderer()
{
	return(&mRenderer);
}