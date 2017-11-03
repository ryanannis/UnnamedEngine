#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/ClientInputManager.h"
#include "Engine/Graphics/Renderer/Renderer.h"

#include <memory>

// Forward decl
struct GLFWwindow;
class GameFramework;

class Client
{
public:
	~Client();

	void Initialize();
	void Terminate();
	void Run();

	GLFWwindow* GetWindow();
	Renderer* GetRenderer();

	void SetTarget(std::unique_ptr<GameFramework> target); 
	Ptr<GameFramework> GetTarget() { return(mTarget.get); }

	Context* GetContext() { return(&mContext); }

private:
	bool mShouldTerminate;
	Context mContext;

	GLFWwindow* mWindow;
	Renderer mRenderer;
	ClientInputManager mInputManager;
	
	std::unique_ptr<GameFramework> mTarget;

	// Initialization convenience functions
	void InitializeWindow();
	void InitializeRenderer();
	void InitializeContext();
	void InitializeInputManager();

	// Disable copying
	Client& operator=(const Client&) = delete;
	Client(const Client&) = delete;
};