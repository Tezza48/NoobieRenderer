#include "Content.h"

#include <fstream>
#include <string>
#include <sstream>

void Content::LoadOBJ(Renderer * renderer, std::string filename, VertexBuffer & vBuffer, IndexBuffer & iBuffer, float scale)
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
				vertices.emplace_back(x * scale, y * scale, z * scale);
			}
			else if (type == "f")
			{
				// it's an index
				// only works for objs with only face and vert data
				unsigned int a, b, c;
				in >> a >> b >> c;
				indices.push_back(a - 1);
				indices.push_back(b - 1);
				indices.push_back(c - 1);

			}
		}

		vBuffer.Init(renderer, vertices);
		iBuffer.Init(renderer, indices);

		file.close();

	}
	else
	{
		//something has gone wrong and i need to remedy it
	}
}
