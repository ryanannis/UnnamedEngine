#pragma once

#include <memory>
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ShaderResource.h"

class GLProgram;
class GLMesh;
class GLAttributes;

class GLDriver
{
public:
	GLDriver() = default;
	GLMesh CreateMesh(const std::weak_ptr<MeshResource>& meshResource);
	GLProgram CreateProgram(
		const std::weak_ptr<ShaderResource>& vertShader,
		const std::weak_ptr<ShaderResource>& fragShader
	);
	GLAttributes CreateAttributes();
};