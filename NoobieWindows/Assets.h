#pragma once
#include "MeshData.h"
#include "BaseObject.h"

namespace Assets
{
	namespace ModelEnum
	{
		inline const char * COCKPIT_OBJ = "res/model/cockpit.obj";
		inline const char * BUNNY_OBJ = "res/model/bunny.obj";
		inline const char * COCKPIT_GLTF = "res/model/cockpit.gltf";
	}

	MeshData LoadObj(const char * path, float scale);
	BaseObject * LoadGLTF(const char * path);
}