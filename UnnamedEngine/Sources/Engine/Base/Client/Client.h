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
	Client(Ptr<Context> context, std::unique_ptr<GameFramework>&& target);
	~Client();

	void Initialize();
	void Terminate();
	void Run();

	Ptr<GLFWwindow> GetWindow();
	Ptr<Renderer> GetRenderer();

	Ptr<GameFramework> GetTarget() { return(mTarget.get()); }
	Ptr<Context> GetContext() { return(mContext); }
	Ptr<GLFWwindow> GetGLFWContext() { return(mWindow); }
	Ptr<ClientInputManager> GetInputManager() { return(&mInputManager); }
	Vector2i GetWindowSize() const;

private:
	bool mShouldTerminate;
	Ptr<Context> mContext;

	Ptr<GLFWwindow> mWindow;

	// Renderer is stored in client since GameFramework also will handle simulation on headless servers
	std::unique_ptr<Renderer> mRenderer;
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