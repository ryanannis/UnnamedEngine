#pragma once
#include "Engine/Base/Common/Common.h"
#include "Formats/MeshFormat.h"

class URI;

namespace ModelLoader
{
	const MeshData LoadModel(URI i);
}