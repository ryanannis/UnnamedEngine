#pragma once

struct GraphicsHandle
{
public:
	GraphicsHandle() : id(0) {}
	GraphicsHandle(size_t id) : id(id) {}
	size_t id;
};