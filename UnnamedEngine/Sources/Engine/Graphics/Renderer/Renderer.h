#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/Renderer/GraphicsHandle.h"
#include "Engine/Graphics/Renderer/RenderSettings.h"
#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Resource/ResourceType.h"

#include "Engine/Graphics/VulkanDriver/VulkanDriver.h"

class GLDriver;
class Context;

struct RendererSettings
{
	char** windowManagerExtensions;
	size_t numExtensions;
};

struct CameraData
{
	Vector3f translation;
	Vector3f rotation;
	float fov;
	float aspectRatio;

};

struct ViewportData
{
	// In pixels
	size_t viewportWidth;
	size_t viewportHeight;
};

struct GraphicsData
{
	ResourceType<ModelResource> mesh;
	Vector3f translation;
	Quat rotation;
};

class Renderer
{
public:
	Renderer(Ptr<Context> c);
	~Renderer() = default;

	void Initialize(const RendererSettings& c);

	void Render();
	GraphicsHandle GenerateGraphicsData();
	GraphicsData& GetGraphicsData(const GraphicsHandle& handle);

	const CameraData& GetCameraData() const;
	void SetCameraData(const CameraData& data);

	void DrawFullscreenQuad();

private:
	RenderSettings mRenderSettings;

	std::unique_ptr<VulkanDriver> mDriver;

	Matrix4 GetCameraVPMatrix();

	void RenderMeshes();
	void RenderGBuffer();
	void RenderLights();

	CameraData mCameraData;
	Ptr<Context> mContext;
	std::vector<GraphicsData> mGraphicsData;

	// Disable copying
	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
};