#pragma once
#include "Engine/Base/Common/Common.h"
#include "Formats/MeshFormat.h"

struct URI;

namespace ModelLoader
{
	const MeshData LoadModel(URI i);
}