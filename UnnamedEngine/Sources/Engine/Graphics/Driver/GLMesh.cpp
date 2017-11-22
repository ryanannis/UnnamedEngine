#include "GLMesh.h"
#include <memory>
#include <glad/glad.h>

#include "Engine/Base/Resource/MeshResource.h"

GLMesh::GLMesh(std::weak_ptr<MeshResource> resource) :
	mResource(resource)
{}




