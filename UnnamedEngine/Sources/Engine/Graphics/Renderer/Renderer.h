#pragma once
#include "Engine/Base/Common/Common.h"

typedef uint64_t GraphicsItemHandle;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();
	GraphicsItemHandle GenerateObject();

private:
	// Disable copying
	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
};