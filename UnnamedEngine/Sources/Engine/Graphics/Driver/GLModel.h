#pragma once
#include "Engine/Base/Common/Common.h"

#include <memory>
#include <glad/glad.h>
#include "Engine/Graphics/Driver/GLMesh.h"

class ModelResource;
class GLTexture;
class GLDriver;

struct GLSubModel
{
	GLMesh mesh;
	Ptr<GLTexture> diffuse;
	Ptr<GLTexture> specular;
};

class GLModel
{
public:
	GLModel(const std::shared_ptr<ModelResource>& resource, GLDriver* driver);
	void Free();
	void Bind();
	const std::vector<GLSubModel>& GetSubmeshes() const;

private:
	std::vector<GLSubModel> mSubmeshes;
};