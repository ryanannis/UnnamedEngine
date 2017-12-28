#include <Catch.h>
#include "Formats/ModelPreprocessor.h"
#include "Formats/ModelLoader.h"
#include <iostream>
#include <chrono>


TEST_CASE("Proprocessing creates a file", "[Preprocesss]")
{
	MeshPreprocessor m;
	URI meshUri("/Levels/Sponza/sponza.obj");
	m.PreprocessMesh(meshUri);
	MeshData d = ModelLoader::LoadModel(meshUri);


}
