#pragma once

struct GLFWwindow;

class Context
{
public:
	Context();
	~Context();

	GLFWwindow* GetWindow() const;
	GLFWwindow* SetWindow(GLFWwindow* window);

private:
	GLFWwindow* mWindow;
};