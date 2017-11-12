#include <Catch.h>
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/EntityResource.h"

#include "Engine/Base/Components/HealthComponent.h"
#include "Engine/Base/Components/IdentityComponent.h"


TEST_CASE("Basic Entity Resource Loading Works", "[EntityResource]") {
	std::string t = "";
	EntityResource res("Test/BasicEntity/TestEntity");
	res.Load();
	REQUIRE(res.IsReady());
}

TEST_CASE("It actually loads the entity correctly!", "[EntityResource]"){
	std::string t = "";
	EntityResource res("Test/BasicEntity/TestEntity");
	res.Load();
	REQUIRE(res.IsReady());

	EntityAdmin a;
	Entity e = res.ConstructEntity(a);
	auto hc = a.GetComponent<HealthComponent>(e);
	auto ic = a.GetComponent<IdentityComponent>(e);


}
