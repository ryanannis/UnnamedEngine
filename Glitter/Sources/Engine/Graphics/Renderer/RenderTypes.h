#include "Renderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp> 

#include "Engine/Base/Resource/Resource.h"

struct EntityRegistrationData
{
	glm::fvec4 pEntityWorldTranslation;
	glm::fquat pEntityWorldRotation;
};