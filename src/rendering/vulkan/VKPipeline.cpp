#include "rendering/vulkan/VKPipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
namespace LG 
{
    VKPipeline::VKPipeline(
        VKDevice& device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& info
    )
        : m_device{ device }
    {
        createGrpahicsPipeline(vertFilepath, fragFilepath, info);
    }


    std::vector<char> VKPipeline::ReadFile(const std::string& filePath)
    {
        std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

        if (!file.is_open()) 
        {
            throw std::runtime_error("File to open file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());

        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void VKPipeline::createGrpahicsPipeline(
        const std::string& vertFilepath, 
        const std::string& fragFilepath, 
        const PipelineConfigInfo& info
    )
    {
        auto vertCode = ReadFile(vertFilepath);
        auto fragCode = ReadFile(fragFilepath);

        std::cout << "vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "fragment Shader Code Size: " << fragCode.size() << '\n';
    }
    void VKPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(m_device.GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create Shader Module");
        }
    }

    PipelineConfigInfo VKPipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};
        return configInfo;
    }
}