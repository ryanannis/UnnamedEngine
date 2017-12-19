#pragma once

#pragma warning(push)
#pragma warning(disable:4201)
// truncation in 32bit
#pragma warning(disable:4310)
//GLM
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp> 
#include <glm/ext.hpp> 
#pragma warning(pop)

// Branch prediction hints
#define LIKELY(condition) __builtin_expect(static_cast<bool>(condition), 1)
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)

// Disable copying
class NonCopyable
{
public:
	NonCopyable(){}
	~NonCopyable() {};
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

typedef glm::ivec2 Vector2i;
typedef glm::vec2 Vector2f;
typedef glm::vec3 Vector3f;
typedef glm::vec4 Vector4f;
typedef glm::mat4 Matrix4;
typedef glm::quat Quat;

#ifdef _MSC_VER
	#define FORCEINLINE __forceinline
#else
	#define FORCEINLINE inline
#endif

#define M_PI 3.14159265358979323846
#define FL_PI 3.14159265358979323846f