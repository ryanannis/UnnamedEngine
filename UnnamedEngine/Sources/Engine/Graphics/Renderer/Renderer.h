#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/Renderer/GraphicsHandle.h"
#include "Engine/Graphics/Renderer/RenderSettings.h"
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceType.h"

class GLDriver;

struct CameraData
{
	Vector3f translation;
	Quat rotation;
	float fov;
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
	Renderer() = default;
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
	std::vector<GraphicsData> mGraphicsData; //temp
	// Disable copying
	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
};