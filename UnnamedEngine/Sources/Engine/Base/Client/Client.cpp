#include "Engine/Base/Common/Common.h"

#include "Client.h"
#include "Engine/Graphics/Renderer/Renderer.h"

#include <glad/glad.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Engine/Base/Client/GameFramework.h"
#include "Engine/Base/Client/Context.h"

#include <iostream>
#include <chrono>
#include <thread>

const int SCREENWIDTH = 1280;
const int SCREENHEIGHT = 720;

Client::Client(Ptr<Context> context, std::unique_ptr<GameFramework>&& target) :
	mShouldTerminate(false),
	mContext(context), // we create this in main to avoid a depedency betwen GameFramework and Client
	mInputManager(context),
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
	mContext->mClient = this;

	// This order is important - the context must be populated with 
	// the GLFW window for Renderer and InputManager to initialize correctly
	InitializeWindow();
	assert(mWindow); 
	InitializeRenderer();
	InitializeInputManager();
	InitializeContext();

	GetTarget()->Initialize();
	mRenderer->Initialize();
}

void Client::InitializeRenderer()
{
	RenderSettings s;
	// hardcoded for now
	s.screenHeight = SCREENHEIGHT;
	s.screenWidth = SCREENWIDTH;

	mRenderer = std::make_unique<Renderer>(mContext);
}

void Client::InitializeWindow()
{
	//assert(glfwVulkanSupported());
	glfwInit();
	// The GL stuff should really be in the renderer... 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	mWindow = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "UnnamedEngine", nullptr, nullptr);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::cout << extensionCount << " extensions supported" << std::endl;

	if(mWindow == nullptr){
		std::cerr << "Failed to initialize OpenGL context." << std::endl;
	}

	glfwMakeContextCurrent(mWindow);
	gladLoadGL();

	std::cerr << "Successfully initialized OpenGL context." << std::endl;

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

		mInputManager.Update();
		mTarget->Update(1.f/60);
		mRenderer->Render();

		auto after = std::chrono::high_resolution_clock::now();
		auto elapsed = after - before;

		auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
		std::cout << "Rendered frame in: " << int_ms.count() << "ns" << std::endl;

		std::this_thread::sleep_for(std::chrono::nanoseconds(16666666) - int_ms);

		mShouldTerminate = mShouldTerminate || glfwWindowShouldClose(mWindow);
	}

	glfwDestroyWindow(GetWindow());
}

Ptr<GLFWwindow> Client::GetWindow()
{
	return(mWindow);
}

Ptr<Renderer> Client::GetRenderer()
{
	return(mRenderer.get());
}

Vector2i Client::GetWindowSize() const
{
	return(Vector2i(SCREENWIDTH, SCREENHEIGHT));
}

