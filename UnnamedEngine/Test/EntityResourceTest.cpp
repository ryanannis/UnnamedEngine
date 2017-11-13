#include <Catch.h>
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/EntityResource.h"

#include "Engine/Base/Client/Context.h"

#include "Engine/Base/Components/HealthComponent.h"
#include "Engine/Base/Components/IdentityComponent.h"
#include "Engine/Base/Components/RenderComponent.h"



TEST_CASE("Basic Entity Resource Loading Works", "[EntityResource]") {
	Context c;
	ResourceManager r(&c);
	std::string t = "";
	EntityResource res("Test/BasicEntity/TestEntity");
	res.Load(&r);
	REQUIRE(res.IsReady());
}

TEST_CASE("It actually loads the entity correctly!", "[EntityResource]")
{
	Context c;
	ResourceManager r(&c);
	std::string t = "";
	EntityResource res("Test/BasicEntity/TestEntity");
	res.Load(&r);
	REQUIRE(res.IsReady());

	EntityAdmin a;
	Entity e = res.ConstructEntity(a);
	auto hc = a.GetComponent<HealthComponent>(e);
	auto ic = a.GetComponent<IdentityComponent>(e);
	auto rc = a.GetComponent<RenderComponent>(e);
}
