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
	void ClearFramebuffer(uint8_t r, uint8_t b, uint8_t g);

	Ptr<GLMesh> CreateMesh(const std::weak_ptr<MeshResource>& meshResource);
	Ptr<GLProgram> CreateProgram(
		const Ptr<GLAttributes>& vao,
		const std::weak_ptr<ShaderResource>& vertShader,
		const std::weak_ptr<ShaderResource>& fragShader
	);
	Ptr<GLAttributes> CreateAttributes();
	void SwapBuffers(GLFWwindow* window);
	void DrawElements(size_t size);

private:
	std::vector<GLuint> mVAOS;
	std::vector<GLMesh> mMeshes;
	std::vector<GLProgram> mPrograms;
	std::vector<GLAttributes> mAttributes;
};