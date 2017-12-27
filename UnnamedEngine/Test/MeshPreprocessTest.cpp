#include <Catch.h>
#include "Engine/Base/Resource/ModelPreprocessor.h"
#include <iostream>

TEST_CASE("Proprocessing creates a file", "[Preprocesss]")
{
	MeshPreprocessor m;
	m.PreprocessMesh(URI("/Levels/Sponza/sponza.obj"));
}
