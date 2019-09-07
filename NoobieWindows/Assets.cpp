#include "Assets.h"
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include "vendor/OBJ_Loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

namespace  fs = std::filesystem;
using namespace Game;

std::vector<MeshData> Assets::LoadObj(const char * path, float scale)
{
	objl::Loader loader;

	loader.LoadFile(path);

	std::vector<MeshData> meshes;
	for (const auto& objlmesh : loader.LoadedMeshes)
	{
		MeshData mesh;
		for (const auto& objlvertex : objlmesh.Vertices)
		{
			Vertex vert;
			memcpy_s(&vert.position, sizeof(XMFLOAT3), &objlvertex.Position, sizeof(objl::Vector3));
			memcpy_s(&vert.normal, sizeof(XMFLOAT3), &objlvertex.Normal, sizeof(objl::Vector3));
			memcpy_s(&vert.texcoord, sizeof(XMFLOAT2), &objlvertex.TextureCoordinate, sizeof(objl::Vector2));

			vert.position.x *= scale;
			vert.position.y *= scale;
			vert.position.z *= -scale;

			vert.texcoord.y = 1 - vert.texcoord.y;

			mesh.vertices.emplace_back(vert);
		}
		mesh.indices = loader.LoadedMeshes[0].Indices;
		std::reverse(mesh.indices.begin(), mesh.indices.end());

		fs::path fsPath = path;

		auto material = objlmesh.MeshMaterial;

		if (material.map_Kd != "")
		{
			auto diffusePath = fsPath.replace_filename(material.map_Kd).wstring();
			mesh.diffuse = diffusePath;
		}

		meshes.push_back(mesh);
	}

	return meshes;
}

MeshData::TextureData Game::Assets::LoadImg(const char* path)
{
	MeshData::TextureData tex;
	tex.data = stbi_load(path, &tex.w, &tex.h, &tex.bpp, 4);

	return tex;
}
