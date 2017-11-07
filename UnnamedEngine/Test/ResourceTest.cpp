#include <Catch.h>
#include "Engine/Base/Resource/MeshResource.h"

TEST_CASE("Meshes actually load whoo!", "[meshloader - smoke]"){
	MeshResource res("Test/Bunny.obj");
	res.Load();
	REQUIRE(res.IsReady());
	REQUIRE(res.GetVertices().size() == 2503);
	REQUIRE(res.GetNormals().size() == 2503);
}
