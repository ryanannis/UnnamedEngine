#pragma optimize("", off)
// We need to make sure the CU getting loaded is not optimized out

#include "StaticRegistry.h"

#include "Engine/Base/Components/CameraComponent.h"
#include "Engine/Base/Components/HealthComponent.h"
#include "Engine/Base/Components/IdentityComponent.h"
#include "Engine/Base/Components/RenderComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Components/VelocityComponent.h"
#include "Engine/Base/Utility/SingletonLogger.h"

int StaticRegister()
{
	/*
	 * Call a function from the compilation unit of each statically registered component
	 * to ensure that it gets statically registered before being used. 
	 */
	{ CameraComponent c; }
	{ HealthComponent c; }
	{ IdentityComponent c; }
	{ RenderComponent c; }
	{ TransformComponent c; }
	{ VelocityComponent c; }
	{ IdentityComponent c; }

	/*
	 * Register logging channels.
	 */
	SingletonLogger::RegisterStream("General");
	SingletonLogger::RegisterStream("Graphics");

	const int DUMMY = 0;
	return(DUMMY);
};
