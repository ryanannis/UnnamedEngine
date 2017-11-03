#include "Renderer.h"

static GraphicsItemHandle sGraphicsItemCounter = 0;

Renderer::Renderer()
{

};

Renderer::~Renderer()
{

}

void Renderer::Render()
{

}

GraphicsItemHandle GenerateObject()
{
	GraphicsItemHandle newItem = ++sGraphicsItemCounter;

}