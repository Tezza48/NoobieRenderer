#include "Content.h"
#include <fstream>
#include <string>
#include <sstream>

void Content::LoadOBJ(PtrDevice device, std::string filename, VertexBuffer & vBuffer, IndexBuffer & iBuffer, float scale)
{
	std::ifstream file(filename, std::fstream::in);
	if (file.is_open())
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

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
				vertices.emplace_back();
				vertices.back().position = XMFLOAT3(x * scale, y * scale, -z * scale);
				vertices.back().color = XMFLOAT4(1.0f, 1.0f, 0.01f, 1.0f);
				//vertices.back().normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
			}
			else if (type == "f")
			{
				// it's an index
				// only works for objs with only face and vert data
				unsigned int a, b, c;
				in >> a >> b >> c;
				indices.push_back(a - 1);
				indices.push_back(c - 1);
				indices.push_back(b - 1);

			}
		}
		// Generate flat Normals
		for (size_t index = 0; index < indices.size(); index+=3)
		{
			XMVECTOR lhs, rhs;
			unsigned int faceVert[3] = { indices[index], indices[index + 1], indices[index + 2] };

			lhs = XMVector3Normalize(XMLoadFloat3(&vertices[faceVert[1]].position) - XMLoadFloat3(&vertices[faceVert[0]].position));
			rhs = XMVector3Normalize(XMLoadFloat3(&vertices[faceVert[1]].position) - XMLoadFloat3(&vertices[faceVert[2]].position));

			XMVECTOR faceNormal = XMVector3Normalize(XMVector3Cross(lhs, rhs));


			// Get the average of the new normal with the existing normal on the point
			// so that the normal at each vertex is the average of all faces it's used in
			XMFLOAT3 newNormal;
			XMStoreFloat3(&newNormal, faceNormal);
			for (size_t vertI = 0; vertI < 3; vertI++)
			{
				if (vertices[faceVert[vertI]].normal.x != 0 || vertices[faceVert[vertI]].normal.y != 0 || vertices[faceVert[vertI]].normal.z != 0)
				{
					vertices[faceVert[vertI]].normal.x += newNormal.x;
					vertices[faceVert[vertI]].normal.y += newNormal.y;
					vertices[faceVert[vertI]].normal.z += newNormal.z;
					vertices[faceVert[vertI]].normal.x /= 2.0f;
					vertices[faceVert[vertI]].normal.y /= 2.0f;
					vertices[faceVert[vertI]].normal.z /= 2.0f;
				}
				else
				{
					vertices[faceVert[vertI]].normal = newNormal;
				}
			}
		}

		vBuffer.Init(device, vertices);
		iBuffer.Init(device, indices);

		file.close();

	}
	else
	{
		printf("Failed to open file: %s\n", filename.c_str());
	}
}
