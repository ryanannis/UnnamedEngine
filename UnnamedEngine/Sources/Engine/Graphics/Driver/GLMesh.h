#pragma once
#include <memory>

class MeshResource;

class GLMesh
{
public:
	GLMesh(std::weak_ptr<MeshResource> resource);

private:
	std::weak_ptr<MeshResource> mResource;
};