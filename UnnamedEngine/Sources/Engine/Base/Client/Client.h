#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/ClientInputManager.h"
#include "Engine/Graphics/Renderer/Renderer.h"

#include <memory>

// Forward decls
struct GLFWwindow;
class GameFramework;

/*
 * This class 'owns' everything system-resource related.
 * If it dies then it is all over.
 */
class Client
{
public:
	Client(std::unique_ptr<GameFramework>&& target);
	~Client();

	void Initialize();
	void Terminate();
	void Run();

	Ptr<GLFWwindow> GetWindow();
	Ptr<Renderer> GetRenderer();

	Ptr<GameFramework> GetTarget() { return(mTarget.get()); }
	Ptr<Context> GetContext() { return(&mContext); }
	Ptr<GLFWwindow> GetGLFWContext() { return(mWindow); }

private:
	bool mShouldTerminate;
	Context mContext;

	Ptr<GLFWwindow> mWindow;
	Renderer mRenderer;
	ClientInputManager mInputManager;
	
	std::unique_ptr<GameFramework> mTarget;

	// Initialization convenience functions
	void InitializeContext();
	void InitializeWindow();
	void InitializeRenderer();
	void InitializeInputManager();

	// Disable copying
	Client& operator=(const Client&) = delete;
	Client(const Client&) = delete;
};