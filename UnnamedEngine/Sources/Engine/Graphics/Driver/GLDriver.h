#pragma once
#include <memory>
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ShaderResource.h"

#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"

struct GLFWwindow;

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
	void SwapBuffers(GLFWwindow* window);

private:
	std::vector<GLMesh> mMeshes;
	std::vector<GLProgram> mPrograms;
	std::vector<GLAttributes> mAttributes;
};