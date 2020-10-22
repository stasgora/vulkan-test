#ifndef TEST_OBJMODEL_H
#define TEST_OBJMODEL_H


#include <string>
#include <vector>
#include "../components/VulkanStructs.h"
#include "../buffers/Buffer.h"

namespace vkr {
	class ObjModel {
	public:
		ObjModel(std::string modelPath);

		void loadModels(Buffer& buffer);

		std::vector<Object> objects;
		uint32_t indexOffset;
	private:
		static uint64_t loadedIndicesCount;
		static uint64_t loadedVerticesCount;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::string modelPath;
	};
}


#endif //TEST_OBJMODEL_H
