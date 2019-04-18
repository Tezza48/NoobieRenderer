#include "Assets.h"
#include <fstream>
#include <sstream>
#include <string>
#include "vendor/tiny_gltf.h"
#include <d3d11_4.h>

using namespace tinygltf;

/**
* TODO: Support OBJ Completely
* Only works with Vertex Data and Faces, nothing else
*/
MeshData Assets::LoadObj(const char * path, float scale)
{
MeshData mesh; 
	
std::ifstream file(path, std::fstream::in);
if (file.is_open())
{
	// Read the file line by line
	std::string line = "";
	while (std::getline(file, line))
	{
		std::istringstream in(line);

		std::string type;
		in >> type;

		if (type == "v")
		{
			// it's a vertex
			float x, y, z;
			in >> x >> y >> z;
			mesh.vertices.emplace_back();
			mesh.vertices.back().position = XMFLOAT3(x * scale, y * scale, -z * scale);
			mesh.vertices.back().color = XMFLOAT4(1.0f, 1.0f, 0.01f, 1.0f);
			//vertices.back().normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		}
		else if (type == "f")
		{
			// it's an index
			// only works for objs with only face and vert data
			unsigned int a, b, c;
			in >> a >> b >> c;

			mesh.indices.push_back(a - 1);
			mesh.indices.push_back(c - 1);
			mesh.indices.push_back(b - 1);
		}
	}
	// Generate flat Normals
	for (size_t index = 0; index < mesh.indices.size(); index += 3)
	{
		XMVECTOR lhs, rhs;
		unsigned int faceVert[3] = { mesh.indices[index], mesh.indices[index + 1], mesh.indices[index + 2] };

		rhs = XMVector3Normalize(XMLoadFloat3(&mesh.vertices[faceVert[1]].position) - XMLoadFloat3(&mesh.vertices[faceVert[0]].position));
		lhs = XMVector3Normalize(XMLoadFloat3(&mesh.vertices[faceVert[1]].position) - XMLoadFloat3(&mesh.vertices[faceVert[2]].position));

		XMVECTOR faceNormal = XMVector3Normalize(XMVector3Cross(lhs, rhs));


		// Get the average of the new normal with the existing normal on the point
		// so that the normal at each vertex is the average of all faces it's used in
		XMFLOAT3 newNormal;
		XMStoreFloat3(&newNormal, faceNormal);
		for (size_t vertI = 0; vertI < 3; vertI++)
		{
			if (mesh.vertices[faceVert[vertI]].normal.x != 0 || mesh.vertices[faceVert[vertI]].normal.y != 0 || mesh.vertices[faceVert[vertI]].normal.z != 0)
			{
				mesh.vertices[faceVert[vertI]].normal.x += newNormal.x;
				mesh.vertices[faceVert[vertI]].normal.y += newNormal.y;
				mesh.vertices[faceVert[vertI]].normal.z += newNormal.z;
				mesh.vertices[faceVert[vertI]].normal.x /= 2.0f;
				mesh.vertices[faceVert[vertI]].normal.y /= 2.0f;
				mesh.vertices[faceVert[vertI]].normal.z /= 2.0f;
			}
			else
			{
				mesh.vertices[faceVert[vertI]].normal = newNormal;
			}
		}
	}
	file.close();

}
else
{
	printf("Failed to open file: %s\n", path);
}
return mesh;
}

BaseObject * Assets::LoadGLTF(const char * path)
{
	Model model;
	TinyGLTF loader;
	std::string err;
	std::string warn;

	bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);

	if (!warn.empty())
	{
		printf("AssetLoader Warning: %s\n", warn.c_str());
	}

	if (!err.empty())
	{
		printf("AssetLoader Error: %s\n", err.c_str());
	}

	if (!ret)
	{
		puts("Failed to parse glTF \n");
		return nullptr;
	}

	//for (const auto & mesh : model.meshes)
	//{
	//	vector<D3D11_INPUT_ELEMENT_DESC> ieDescs;
	//	for (const auto & primitive : mesh.primitives)
	//	{
	//		auto attr = primitive.attributes.at("POSITION");
	//		ieDescs.push_back({"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, attr, D3D11_INPUT_PER_VERTEX_DATA, 0});
	//	}
	//}

	return nullptr;
}
