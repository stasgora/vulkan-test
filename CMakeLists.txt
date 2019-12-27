cmake_minimum_required(VERSION 3.7)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(test src/main.cpp src/Window.cpp src/Window.h src/components/DebugLayer.cpp src/components/DebugLayer.h src/components/DeviceManager.cpp
        src/components/DeviceManager.h src/rendering/SwapChain.cpp src/rendering/SwapChain.h src/components/VulkanStructs.h
        src/rendering/Pipeline.cpp src/rendering/Pipeline.h src/buffers/CommandBuffer.cpp src/buffers/CommandBuffer.h
        src/rendering/Renderer.cpp src/rendering/Renderer.h src/buffers/Buffer.cpp src/buffers/Buffer.h
        src/buffers/UniformBuffer.cpp src/buffers/UniformBuffer.h src/buffers/BufferUtils.cpp src/buffers/BufferUtils.h
        src/components/DescriptorSet.cpp src/components/DescriptorSet.h libs/stb_image.h libs/stb_image.cpp
        src/image/TextureImage.cpp src/image/TextureImage.h src/image/SampledTexture.cpp src/image/SampledTexture.h
        src/image/DepthImage.cpp src/image/DepthImage.h src/image/ImageUtils.cpp src/image/ImageUtils.h
        src/components/RendererComponent.h src/components/RendererComponent.cpp src/image/BaseImage.cpp src/image/BaseImage.h
        libs/tiny_obj_loader.h libs/tiny_obj_loader.cpp src/model/ObjModel.cpp src/model/ObjModel.h)
target_link_libraries(test Vulkan::Vulkan glfw glm)
