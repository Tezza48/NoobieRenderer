#pragma once
#include "NoobieCore/MeshData.h"

namespace Assets
{
	namespace ModelEnum
	{
		inline const char * COCKPIT_OBJ = "res/model/cockpit.obj";
		inline const char * BUNNY_OBJ = "res/model/bunny.obj";
	}

	MeshData LoadObj(const char * path, float scale);
}