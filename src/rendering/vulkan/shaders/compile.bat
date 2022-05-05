@echo off
@echo Building Vulkan Shaders
C:\VulkanSDK\1.3.211.0\Bin\glslc.exe ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert -o ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv
C:\VulkanSDK\1.3.211.0\Bin\glslc.exe ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag -o ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv
@echo Shaders compilation finished