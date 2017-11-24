#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/Renderer/GraphicsHandle.h"
#include "Engine/Graphics/Renderer/RenderSettings.h"
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceType.h"

class GLDriver;
class Context;

struct CameraData
{
	Vector3f translation;
	Quat rotation;
	float fov;
};

struct ViewportData
{
	// In pixels
	size_t viewportWidth;
	size_t viewportHeight;
};

struct GraphicsData
{
	ResourceType<MeshResource> mesh;
	Vector3f translation;
	Vector3f rotation;
};

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	void Render();
	GraphicsHandle GenerateGraphicsData();
	GraphicsData& GetGraphicsData(GraphicsHandle& handle);

	const CameraData& GetCameraData() const;
	void SetCameraData(const CameraData& data);

private:
	RenderSettings mRenderSettings;

	std::unique_ptr<GLDriver> mDriver;

	CameraData mCameraData;
	Ptr<Context> mContext;
	std::vector<GraphicsData> mGraphicsData; //temp
	// Disable copying
	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
};