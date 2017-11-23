#pragma once
#include <memory>

class MeshResource;

class GLMesh
{
public:
	GLMesh(const std::weak_ptr<MeshResource>& resource);
};