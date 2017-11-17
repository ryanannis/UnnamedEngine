#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/Renderer/GraphicsHandle.h"
#include "Engine/Graphics/Renderer/RenderSettings.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();
	GraphicsHandle GenerateObject();

private:
	RenderSettings mRenderSettings;
	// Disable copying
	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
};