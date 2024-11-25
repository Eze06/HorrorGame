#include <iostream>
#include <fstream>

#include "LoadOBJ.h"

bool LoadOBJ(
	const char* file_path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
)
{
	//Fill up code from OBJ lecture notes
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::vector<unsigned> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	while (!fileStream.eof()) {
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0) { 
			glm::vec3 vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strncmp("vt ", buf, 3) == 0) { 
			glm::vec2 texCoord;
			sscanf_s((buf + 2), "%f%f", &texCoord.s, &texCoord.t);
			temp_uvs.push_back(texCoord);
		}
		else if (strncmp("vn ", buf, 3) == 0) { 
			glm::vec3 normal;
			sscanf_s((buf + 2), "%f%f%f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strncmp("f ", buf, 2) == 0) { 
			// process face



			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = sscanf_s((buf + 2), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2],
				&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			// Process faces
			if (matches == 9) //triangle
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			else if (matches == 12) // Do the same for quad
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
			}
			else
			{
				std::cout << "Error line: " << buf << std::endl;
				std::cout << "File can't be read by parser\n";
				return false;
			}
		}
	}
	fileStream.close(); // close file

	// For each vertex of each triangle, after fileStream.close()
	for (unsigned i = 0; i < vertexIndices.size(); ++i)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}


	return true;
}

struct PackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

bool getSimilarVertexIndex_fast(
	PackedVertex& packed,
	std::map<PackedVertex, unsigned short>& VertexToOutIndex,
	unsigned short& result
) {
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end())
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void IndexVBO(
	std::vector<glm::vec3>& in_vertices,
	std::vector<glm::vec2>& in_uvs,
	std::vector<glm::vec3>& in_normals,

	std::vector<unsigned>& out_indices,
	std::vector<Vertex>& out_vertices
)
{
	std::map<PackedVertex, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i < in_vertices.size(); ++i)
	{

		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found)
		{
			// A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else
		{
			// If not, it needs to be added in the output data.
			Vertex v;
			v.pos = glm::vec3(in_vertices[i].x, in_vertices[i].y, in_vertices[i].z);
			v.texCoord = glm::vec2(in_uvs[i].s, in_uvs[i].t);
			v.normal = glm::vec3(in_normals[i].x, in_normals[i].y, in_normals[i].z);
			v.color = glm::vec3(1, 1, 1);
			out_vertices.push_back(v);
			unsigned newindex = (unsigned)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

bool LoadMTL(const char* file_path, std::map<std::string, Material*>& materials_map)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	Material* mtl = nullptr;
	while (!fileStream.eof()) {
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("newmtl ", buf, 7) == 0) { //process newmtl
			char mtl_name[256];
			strcpy_s(mtl_name, buf + 7);
			if (mtl_name[strlen(mtl_name) - 1] == '\r')
				mtl_name[strlen(mtl_name) - 1] = '\0';
			mtl = nullptr;
			if (materials_map.find(mtl_name) == materials_map.end())
			{
				mtl = new Material();
				materials_map.insert(std::pair<std::string, Material*>(mtl_name, mtl));
			}
		}
		else if (strncmp("Ka ", buf, 3) == 0) { //process Ka
			if (mtl != nullptr)
			{
				sscanf_s((buf + 2), "%f%f%f", &mtl->kAmbient.r, &mtl->kAmbient.g, &mtl->kAmbient.b);
			}
		}
		else if (strncmp("Kd ", buf, 3) == 0) { //process Kd
			if (mtl != nullptr)
			{
				sscanf_s((buf + 2), "%f%f%f", &mtl->kDiffuse.r, &mtl->kDiffuse.g, &mtl->kDiffuse.b);
			}
		}
		else if (strncmp("Ks ", buf, 3) == 0) { //process Ks
			if (mtl != nullptr)
			{
				sscanf_s((buf + 2), "%f%f%f", &mtl->kSpecular.r, &mtl->kSpecular.g, &mtl->kSpecular.b);
			}
		}
		else if (strncmp("Ns ", buf, 3) == 0) { //process Ns
			if (mtl != nullptr)
			{
				sscanf_s((buf + 2), "%f", &mtl->kShininess);
			}
		}
	}
	fileStream.close(); // close file

	return true;
}

bool LoadOBJMTL(const char* file_path, const char* mtl_path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, std::vector<Material>& out_materials)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::vector<unsigned> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::map<std::string, Material*> materials_map;
	if(mtl_path != nullptr && !LoadMTL(mtl_path, materials_map))
		return false;

	while (!fileStream.eof()) {
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0) { 
			// process vertex glm::vec3
			glm::vec3 vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			// refer to lecture on how to do it
		}
		else if (strncmp("vt ", buf, 3) == 0) { 
			// process texcoord
			glm::vec2 texCoord;
			sscanf_s((buf + 2), "%f%f", &texCoord.s, &texCoord.t);
			temp_uvs.push_back(texCoord);
			// refer to lecture on how to do it
		}
		else if (strncmp("vn ", buf, 3) == 0) { 
			// process normal
			glm::vec3 normal;
			sscanf_s((buf + 2), "%f%f%f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
			// refer to lecture on how to do it
		}
		//else if (strncmp("mtllib ", buf, 7) == 0) { //process mtllib
		//	char mtl_path[256];
		//	strcpy_s(mtl_path, buf + 7);
		//	LoadMTL(mtl_path, materials_map);
		//}
		else if (strncmp("usemtl ", buf, 7) == 0) { 
			// process usemtl
			char mtl_name[256];
			strcpy_s(mtl_name, buf + 7);
			mtl_name[strcspn(mtl_name, "\r")] = '\0';
			if (materials_map.find(mtl_name) != materials_map.end())
			{
				Material* mtl = materials_map.find(mtl_name)->second;
				Material material = *mtl;
				out_materials.push_back(material);
			}
		}
		else if (strncmp("f ", buf, 2) == 0) { 
			// process face
			// refer to lecture on how to do it
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = sscanf_s((buf + 2), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2],
				&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			if (matches == 9) //triangle (hint: index 0,1,2)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				if (out_materials.size() > 0)
				{
					out_materials.back().size += 3;
				}
			}
			else if (matches == 12) //quad (hint: index 0,1,2 & 0,2,3)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
				if (out_materials.size() > 0)
				{
					out_materials.back().size += 6;
				}
			}
			else
			{
			}
		}
	}
	fileStream.close(); // close file

	// For each vertex of each triangle, after fileStream.close()
	for (unsigned i = 0; i < vertexIndices.size(); ++i)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}

	for (std::map<std::string, Material*>::iterator it = materials_map.begin(); it != materials_map.end(); ++it)
	{
		delete it->second;
	}
	materials_map.clear();

	return true;
}
