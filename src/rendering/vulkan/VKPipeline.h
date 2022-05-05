#pragma once
#include <Legio/platform/Log.h>
#include "rendering/vulkan/VKDevice.h"

#include <string>
#include <vector>
namespace LG {

    struct PipelineConfigInfo 
    {
    
    };

    class VKPipeline
    {
    public:
        VKPipeline(
            VKDevice& device,
            const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& info
        );

        VKPipeline(const VKPipeline&) = delete;
        VKPipeline& operator=(const VKPipeline&) = delete;

        static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

        ~VKPipeline() { LG_CORE_INFO("\t VKPipeline deleted"); };
    
    private:
        static std::vector<char> ReadFile(const std::string& filePath);

        void createGrpahicsPipeline(
            const std::string& vertFilepath, 
            const std::string& fragFilepath,
            const PipelineConfigInfo& info
        );

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        VKDevice& m_device;
        VkPipeline m_graphicsPipeline;
        VkShaderModule m_vertShaderModuel;
        VkShaderModule m_fragShaderModuel;
    };
}