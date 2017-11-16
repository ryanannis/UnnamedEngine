#include "Renderer.h"

static size_t sGraphicsHandleCt;

Renderer::Renderer(RenderSettings&& settings) :
	mRenderSettings(settings)
{}

Renderer::~Renderer()
{

}

void Renderer::Render()
{

}

GraphicsHandle Renderer::GenerateObject()
{
	// Register object
	GraphicsHandle g(++sGraphicsHandleCt);

	return(g);
}