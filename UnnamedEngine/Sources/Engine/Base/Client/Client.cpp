#include "Engine/Base/Common/Common.h"

#include "Client.h"
#include "Engine/Graphics/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Base/Client/GameFramework.h"
#include <iostream>
#include <chrono>
#include <thread>

Client::Client(std::unique_ptr<GameFramework>&& target) :
	mShouldTerminate(false),
	mContext(),
	mInputManager(&mContext),
	mTarget(std::move(target))
{
}

Client::~Client()
{
}

void Client::Initialize()
{	
	// Initialize this first so other systems can access Client during
	// their own initialization
	mContext.mClient = this;

	// This order is important - the context must be populated with 
	// the GLFW window for Renderer and InputManager to initialize correctly
	InitializeWindow();
	assert(mWindow);
	InitializeRenderer();
	InitializeInputManager();
	InitializeContext();

	GetTarget()->Initialize();
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

	mWindow = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);

	if(mWindow == nullptr){
		std::cerr << "Failed to initialize OpenGL context." << std::endl;
	}

	glfwMakeContextCurrent(mWindow);
	gladLoadGL();

	std::cerr << "Successfully initialized OpenGL context." << std::endl;
}

void Client::InitializeContext()
{
	
	
}

void Client::InitializeInputManager()
{
	mInputManager.Initialize();
}

void Client::Terminate()
{
	mShouldTerminate = true;
}

void Client::Run()
{
	auto lastTime = std::chrono::high_resolution_clock::now();

	// Rendering Loop
	while(!mShouldTerminate){
		assert(mTarget);
		auto before = std::chrono::high_resolution_clock::now();

		mTarget->Update(1.f/60);

		auto after = std::chrono::high_resolution_clock::now();
		auto elapsed = after - before;

		auto int_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
		std::cout << "Rendered frame in: " << int_ms.count() << "ns" << std::endl;

		std::this_thread::sleep_for(std::chrono::nanoseconds(16666666) - int_ms);
	}

	glfwDestroyWindow(GetWindow());
}

Ptr<GLFWwindow> Client::GetWindow()
{
	return(mWindow);
}

Ptr<Renderer> Client::GetRenderer()
{
	return(&mRenderer);
}

