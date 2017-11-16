#pragma once
#include <memory>

class MeshResource;

class GLMesh
{
public:
	GLMesh(std::shared_ptr<MeshResource> resource);

private:
	std::shared_ptr<MeshResource> mResource;
};