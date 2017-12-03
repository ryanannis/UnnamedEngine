#pragma once
#include <memory>
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Resource/ShaderResource.h"
#include "Engine/Base/Resource/MaterialResource.h"
#include <unordered_map>


#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLTexture.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"

struct GLFWwindow;
class ResourceManager;

class GLDriver
{
public:
	GLDriver(Ptr<ResourceManager> manager);
	
	void ClearResources();
	void ClearFramebuffer(uint8_t r, uint8_t b, uint8_t g);

	Ptr<GLMesh> CreateMesh(const ResourceType<ModelResource>& ModelResource);
	Ptr<GLTexture> LoadTexture(const ResourceType<MaterialResource>& textureResource);
	void DrawMesh(Ptr<GLMesh> mesh);
	Ptr<GLProgram> CreateProgram(
		const Ptr<GLAttributes>& vao,
		const std::shared_ptr<ShaderResource>& vertShader,
		const std::shared_ptr<ShaderResource>& fragShader
	);
	Ptr<GLAttributes> CreateAttributes();
	void SwapBuffers(GLFWwindow* window);
	void DrawElements(size_t size);

private:
	Ptr<ResourceManager> mResourceManager;
	std::vector<GLuint> mVAOS;
	std::unordered_map<std::string, GLMesh> mMeshes;
	std::unordered_map<std::string, GLTexture> mTextures;
	std::vector<GLProgram> mPrograms;
	std::vector<GLAttributes> mAttributes;
};