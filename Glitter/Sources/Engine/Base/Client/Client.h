#pragma once

#include "Engine/Base/Client/Context.h"
#include "Engine/Graphics/Renderer/Renderer.h"

#include <memory>

// Forward decl
struct GLFWwindow;
class GameFramework;

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

	void SetTarget(std::unique_ptr<GameFramework> target); 
	Context* GetContext() { return(mContext) };

private:
	bool mShouldTerminate;
	Context mContext;

	GLFWwindow* mWindow;
	Renderer mRenderer;
	
	std::unique_ptr<GameFramework> mTarget;

	// Initialization convenience functions
	void InitializeWindow();
	void InitializeRenderer();
	void InitializeContext();

	// Disable copying
	Client& operator=(const Client&) = delete;
	Client(const Client&) = delete;
};