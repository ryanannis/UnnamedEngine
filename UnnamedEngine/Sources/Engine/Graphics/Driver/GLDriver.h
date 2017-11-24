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
	
	void ClearResources();

	Ptr<GLMesh> CreateMesh(const std::weak_ptr<MeshResource>& meshResource);
	Ptr<GLProgram> CreateProgram(
		const std::weak_ptr<ShaderResource>& vertShader,
		const std::weak_ptr<ShaderResource>& fragShader
	);
	Ptr<GLAttributes> CreateAttributes();

private:
	std::vector<GLMesh> mMeshes;
	std::vector<GLProgram> mPrograms;
	std::vector<GLAttributes> mAttributes;
};