#pragma once
#include <Legio/platform/Log.h>
#include "rendering/vulkan/VKDevice.h"

#include <string>
#include <vector>
namespace LG {

    struct PipelineConfigInfo 
    {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

        std::vector<VkDynamicState> dyamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;

        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
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
        ~VKPipeline();

        VKPipeline(const VKPipeline&) = delete;
        VKPipeline& operator=(const VKPipeline&) = delete;
        void Bind(VkCommandBuffer commandBuffer);

        static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
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