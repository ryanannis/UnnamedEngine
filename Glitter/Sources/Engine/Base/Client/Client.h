#pragma once

#include "Engine/Base/Client/Context.h"

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

	const Context& GetContext() const;

private:
	bool mShouldTerminate;
	Context mContext;

	GLFWwindow* InitializeWindow();

	// Disable copying
	Client& operator=(const Client&) = delete;
	Client(const Client&) = delete;
};