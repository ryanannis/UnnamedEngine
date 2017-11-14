#include <Catch.h>
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/EntityResource.h"

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Level/LevelResource.h"

TEST_CASE("Basic Level Loading Works", "[Levels]") {
	Context c;
	ResourceManager r(&c);
	std::string t = "";
	LevelResource res("Levels/Test/TestLevel.ul");
	res.Load(&r);
	REQUIRE(res.IsReady());
}
