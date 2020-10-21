#include "ObjModel.h"
#include "../../libs/tiny_obj_loader.h"

#include <utility>
#include <unordered_map>

vkr::ObjModel::ObjModel(std::string modelPath) : modelPath(std::move(modelPath)) {}

void vkr::ObjModel::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
		throw std::runtime_error(warn + err);

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
	for (const auto &shape : shapes) {
		for (const auto &index : shape.mesh.indices) {
			Vertex vertex = {
				{
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				},
				{1, 1, 1},
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				},
				{
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				}
			};
			if (!uniqueVertices.contains(vertex)) {
				uniqueVertices[vertex] = vertices.size();
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
