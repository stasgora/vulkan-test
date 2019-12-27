#ifndef TEST_OBJMODEL_H
#define TEST_OBJMODEL_H


#include <string>
#include <vector>
#include "../components/VulkanStructs.h"

namespace vkr {
	class ObjModel {
	public:
		ObjModel(std::string modelPath);

		void loadModel();

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	private:
		std::string modelPath;
	};
}


#endif //TEST_OBJMODEL_H
