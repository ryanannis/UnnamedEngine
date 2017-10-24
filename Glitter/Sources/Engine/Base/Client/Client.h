#pragma once

#include "Engine/Base/Client/Context.h"
#include "Engine/Graphics/Renderer/Renderer.h"

// Forward decl
struct GLFWwindow;

class Client
{
public:
	Client();
	~Client();

	void Initialize();
	void Terminate();
	void Run();

	GLFWwindow* GetWindow();
	Renderer* GetRenderer();

private:
	bool mShouldTerminate;
	Context mContext;

	GLFWwindow* mWindow;
	Renderer mRenderer;

	// Initialization convenience functions
	void InitializeWindow();
	void InitializeRenderer();
	void InitializeClient();

	// Disable copying
	Client& operator=(const Client&) = delete;
	Client(const Client&) = delete;
};