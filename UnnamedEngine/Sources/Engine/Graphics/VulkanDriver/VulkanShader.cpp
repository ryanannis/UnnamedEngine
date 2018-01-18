#include "VulkanModel.h"

VulkanModel::VulkanModel(std::shared_ptr<ModelResource> model) :
	mModelResource(model)
{}

void VulkanModel::Initialize(VulkanMeshManager* driver)
{
	assert(driver != nullptr);
	assert(mModelResource->IsReady());

}

void VulkanModel::Release(VulkanMeshManager * driver)
{
	assert(driver != nullptr);


}

