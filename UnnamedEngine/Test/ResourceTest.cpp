#include <Catch.h>
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceManager.h"

TEST_CASE("URL Parser works!", "[urlparser]") {
	URI t1 = ResourceManager::ParseStringToURI("/hello/my/friend.txt");
	REQUIRE(t1.path == "/hello/my/");
	REQUIRE(t1.component == "");
	REQUIRE(t1.ext == "txt");
	REQUIRE(t1.file == "friend");

	URI t2 = ResourceManager::ParseStringToURI("/wow/a/data/component");
	REQUIRE(t2.path == "/wow/a/");
	REQUIRE(t2.component == "component");
	REQUIRE(t2.ext == "udf");
	REQUIRE(t2.file == "data");
}

TEST_CASE("Meshes actually load whoo!", "[meshloader - smoke]"){
	MeshResource res("Test/Bunny.obj");
	res.Load();
	REQUIRE(res.IsReady());
	REQUIRE(res.GetVertices().size() == 2503);
	REQUIRE(res.GetNormals().size() == 2503);
}
