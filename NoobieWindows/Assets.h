#pragma once
#include "NoobieCore/MeshData.h"

namespace Game::Assets
{
	namespace ModelEnum
	{
		inline const char * COCKPIT_OBJ = "res/model/cockpit.obj";
		inline const char * BUNNY_OBJ = "res/model/bunny.obj";
	}

	std::vector<MeshData> LoadObj(const char * path, float scale);

	MeshData::TextureData LoadImg(const char* path);
}