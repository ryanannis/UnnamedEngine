#pragma once

#pragma warning(push)
#pragma warning(disable:4201)
//GLM
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp> 
#pragma warning(pop)

// Branch prediction hints
#define LIKELY(condition) __builtin_expect(static_cast<bool>(condition), 1)
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)

// Disable copying
class NonCopyable
{
public:
	NonCopyable(){}
	virtual ~NonCopyable() {};
private:
	NonCopyable& operator=(const NonCopyable&) = delete;
	NonCopyable(const NonCopyable&) = delete;
};

// Common STD Containers
#include <array>
#include <vector>
#include <memory>
#include <optional>
#include <utility>

// Common Pointer Functions
#include "Engine/Base/Common/Ptr.h"
#include "Engine/Graphics/Renderer/GraphicsHandle.h"

typedef glm::vec3 Vector3f;
typedef glm::quat Quat;
