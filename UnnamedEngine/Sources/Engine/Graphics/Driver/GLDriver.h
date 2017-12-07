#pragma once
#include <memory>
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Resource/ShaderResource.h"
#include "Engine/Base/Resource/MaterialResource.h"
#include <unordered_map>

#include "Engine/Graphics/Driver/GLModel.h"
#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLTexture.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"
#include "Engine/Graphics/Driver/GLRenderTarget.h"

struct GLFWwindow;
class ResourceManager;

class GLDriver
{
public:
	GLDriver(Ptr<ResourceManager> manager);
	
	void EnableDepthTest();
	void ClearResources();
	void ClearFramebuffer(uint8_t r, uint8_t b, uint8_t g);

	Ptr<GLModel> CreateModel(const ResourceType<ModelResource>& ModelResource);
	Ptr<GLMesh> CreateMesh(const std::vector<float>& vertices, const std::vector<GLuint>& indices);
	Ptr<GLModel> RenderFullscreen();

	Ptr<GLTexture> LoadTexture(const ResourceType<MaterialResource>& textureResource);
	void DrawMesh(Ptr<GLAttributes> vao, Ptr<GLModel> mesh);
	Ptr<GLProgram> CreateProgram(
		const Ptr<GLAttributes>& vao,
		const std::shared_ptr<ShaderResource>& vertShader,
		const std::shared_ptr<ShaderResource>& fragShader
	);
	void DrawScreenQuad();
	Ptr<GLAttributes> CreateAttributes();
	Ptr<GLRenderTarget> CreateRenderTarget();
	void SwapBuffers(GLFWwindow* window);

private:
	Ptr<ResourceManager> mResourceManager;
	std::vector<GLuint> mVAOS;
	std::vector<GLMesh> mMeshes;
	std::unordered_map<std::string, GLModel> mModels;
	std::unordered_map<std::string, GLTexture> mTextures;
	std::vector<GLProgram> mPrograms;
	std::vector<GLAttributes> mAttributes;
	std::vector<GLRenderTarget> mRenderTargets;
};