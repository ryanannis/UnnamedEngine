#include "Renderer.h"

static size_t sGraphicsHandleCt;

Renderer::Renderer()
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