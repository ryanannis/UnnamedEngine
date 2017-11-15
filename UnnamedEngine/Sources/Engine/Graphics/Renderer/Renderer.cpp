#include "Renderer.h"


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
	return(newItem);
}