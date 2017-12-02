#include "Engine/Base/Client/Client.h"
#include "Engine/Base/Common/Common.h"

#include <memory>
#include "Engine/Base/Client/GameFramework.h"
#include "Engine/Base/Components/CameraComponent.h"

#include "Engine/Base/Types/StaticRegistry.h"
static int dummy = StaticRegister();

int main(int, char* [])
{
	Context context;
	Client c(&context, std::make_unique<GameFramework>(&context));
	c.Initialize();
	// temp for testing
	ResourceType<LevelResource> level("Levels/Sponza/sponza.ul");
	ResourceType<EntityResource> player("Test/BasicPlayer/TestPlayer");
	c.GetTarget()->LoadIntoLevel(level);
	Entity& e = c.GetTarget()->GetRegionAdmin()->CreateEntity(player, Vector3f(2, 2, 2), Vector3f(0, 0, 0), false);
	auto camera = c.GetTarget()->GetRegionAdmin()->GetEntityAdmin()->AddComponent<CameraComponent>(e);
	c.GetTarget()->GetGameClient()->SetLocalPlayerEntity(e);

	camera->pEntityCameraTranslation = Vector3f(-1, -2, 0);

	c.Run();
}