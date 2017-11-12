#include <Catch.h>
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/EntityResource.h"

TEST_CASE("Basic Entity Resource Loading Works", "[urlparser]") {
	std::string t = "";
	EntityResource res("Test/BasicEntity/TestEntity");
	res.Load();
	REQUIRE(res.IsReady());
}

TEST_CASE("It actually loads the entity correctly!", "[meshloader - smoke]"){
	std::string t = "";
	EntityResource res("Test/BasicEntity/TestEntity");
	res.Load();
	REQUIRE(res.IsReady());


}
