#include "Assets.h"
#include <fstream>
#include <sstream>
#include <string>
#include "vendor/OBJ_Loader.h"


/**
 * TODO: Support OBJ Completely
 * Only works with Vertex Data and Faces, nothing else
 */
MeshData Assets::LoadObj(const char * path, float scale)
{
	objl::Loader loader;

	loader.LoadFile(path);

	MeshData mesh;

	for (const auto& objlvertex : loader.LoadedMeshes[0].Vertices)
	{
		Vertex vert;
		memcpy_s(&vert.position, sizeof(XMFLOAT3), &objlvertex.Position, sizeof(objl::Vector3));
		memcpy_s(&vert.normal, sizeof(XMFLOAT3), &objlvertex.Normal, sizeof(objl::Vector3));

		vert.position.x *= scale;
		vert.position.y *= scale;
		vert.position.z *= -scale;

		vert.normal.x *= -1.0f;
		vert.normal.y *= -1.0f;
		vert.normal.z *= 1.0f;

		mesh.vertices.emplace_back(vert);
	}

	mesh.indices = loader.LoadedMeshes[0].Indices;
	std::reverse(mesh.indices.begin(), mesh.indices.end());
	
	
	return mesh;
}
