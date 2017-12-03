#pragma once
#include "Engine/Base/Common/Common.h"

#include <memory>
#include <glad/glad.h>

class ModelResource;
class GLTexture;
class GLDriver;

struct GLSubmesh
{
	GLuint verticesbuffer;
	GLuint indicesBuffer;
	size_t numIndices;
	Ptr<GLTexture> diffuse;
	Ptr<GLTexture> specular;
};

class GLMesh
{
public:
	// todo:  move tex initialization and remove driver ptr in cstr
	GLMesh(const std::shared_ptr<ModelResource>& resource, GLDriver* driver);
	void Free();
	const std::vector<GLSubmesh>& GetSubmeshes() const;

private:
	std::vector<GLSubmesh> mSubmeshes;
};