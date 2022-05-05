@echo off
@echo Building Vulkan Shaders
%VK_SDK_PATH%\Bin\glslc.exe ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert -o ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv
%VK_SDK_PATH%\Bin\glslc.exe ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag -o ../../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv
@echo Shaders compilation finished