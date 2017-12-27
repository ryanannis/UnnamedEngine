#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>

//todo:  should probably just automatically import this from somewhere...
//todo:  validation layer for this?
VkResult DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
