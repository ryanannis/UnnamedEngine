#pragma once

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();

private:
	// Disable copying
	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
};