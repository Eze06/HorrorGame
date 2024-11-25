#ifndef LOAD_OBJ_H
#define LOAD_OBJ_H

#include <vector>
#include <glm\glm.hpp>
#include "Vertex.h"
#include "Material.h"
#include <map>

bool LoadOBJ(
	const char *file_path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs, 
	std::vector<glm::vec3> & out_normals
);

void IndexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned> & out_indices,
	std::vector<Vertex> & out_vertices
);

bool LoadOBJMTL(
	const char* file_path, 
	const char* mtl_path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals,
	std::vector<Material>& out_materials
);

#endif