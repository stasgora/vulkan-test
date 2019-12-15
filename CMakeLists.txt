cmake_minimum_required(VERSION 3.7)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(test main.cpp Window.cpp Window.h components/DebugLayer.cpp components/DebugLayer.h components/GPUDeviceManager.cpp components/GPUDeviceManager.h components/SwapChain.cpp components/SwapChain.h components/ComponentStructs.h pipeline/GraphicsPipeline.cpp pipeline/GraphicsPipeline.h)
target_link_libraries(test Vulkan::Vulkan glfw glm)
