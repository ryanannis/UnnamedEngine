#include "GLModel.h"
#include <memory>

#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Graphics/Driver/GLDriver.h"

GLModel::GLModel(const std::shared_ptr<ModelResource>& resource, GLDriver* driver)
{
	auto res = resource;
	const auto& meshes = res->GetMeshes();
	for(auto mesh : meshes)
	{
		GLSubModel s;
		if(mesh.mDiffuseTextures.size() > 0)
		{
			s.diffuse = driver->LoadTexture(mesh.mDiffuseTextures[0]);
		}
		if(mesh.mSpecularTextures.size() > 0)
		{
			s.specular = driver->LoadTexture(mesh.mSpecularTextures[0]);
		}
		s.mesh = GLMesh(mesh.mVectors, mesh.mIndices);

		mSubmeshes.push_back(s);
	}
}

void GLModel::Free()
{
	for(auto& submesh : mSubmeshes)
	{
		submesh.mesh.Free();
	}
}

const std::vector<GLSubModel>& GLModel::GetSubmeshes() const
{
	return(mSubmeshes);
}