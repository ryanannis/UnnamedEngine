#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/Renderer/GraphicsHandle.h"
#include "Engine/Graphics/Renderer/RenderSettings.h"
#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Graphics/Driver/GLDriver.h"

class GLDriver;
class Context;

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

	void Initialize();

	void Render();
	GraphicsHandle GenerateGraphicsData();
	GraphicsData& GetGraphicsData(const GraphicsHandle& handle);

	const CameraData& GetCameraData() const;
	void SetCameraData(const CameraData& data);

private:
	RenderSettings mRenderSettings;

	std::unique_ptr<GLDriver> mDriver;

	Matrix4 GetCameraVPMatrix();

	void RenderMeshes();

	CameraData mCameraData;
	Ptr<Context> mContext;
	std::vector<GraphicsData> mGraphicsData;

	// Hardcoded Resources
	std::shared_ptr<ShaderResource> mBasicVert;
	std::shared_ptr<ShaderResource> mBasicFrag;

	// Temp
	Ptr<GLAttributes> mVao;
	Ptr<GLProgram> mBasicProgram;

	// Disable copying
	Renderer& operator=(const Renderer&) = delete;
	Renderer(const Renderer&) = delete;
};