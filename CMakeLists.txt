cmake_minimum_required(VERSION 3.7)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(test main.cpp Window.cpp Window.h components/DebugLayer.cpp components/DebugLayer.h components/DeviceManager.cpp
        components/DeviceManager.h rendering/SwapChain.cpp rendering/SwapChain.h components/VulkanStructs.h
        rendering/Pipeline.cpp rendering/Pipeline.h buffers/CommandBuffer.cpp buffers/CommandBuffer.h
        rendering/Renderer.cpp rendering/Renderer.h buffers/Buffer.cpp buffers/Buffer.h
        buffers/UniformBuffer.cpp buffers/UniformBuffer.h buffers/AbstractBuffer.cpp buffers/AbstractBuffer.h
        components/DescriptorSet.cpp components/DescriptorSet.h libs/stb_image.h libs/stb_image.cpp textures/TextureImage.cpp textures/TextureImage.h textures/SampledTexture.cpp textures/SampledTexture.h)
target_link_libraries(test Vulkan::Vulkan glfw glm)
