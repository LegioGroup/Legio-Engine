#pragma once
#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>
namespace LG
{
    struct PipelineConfigInfo
    {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportState;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlending;

        VkPipelineDynamicStateCreateInfo dynamicState;
        std::vector<VkDynamicState> dyamicStateEnables;
        
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    class VKDevice;
    class VKPipeline
    {
    public:
        VKPipeline(
            VKDevice* device,
            const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& info
        );
        
        ~VKPipeline();
        VKPipeline(const VKPipeline&) = delete;
        VKPipeline& operator=(const VKPipeline&) = delete;
        static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
        VkPipeline GetGraphicsPipeline() const { return m_graphicsPipeline; }
        void Bind(VkCommandBuffer commandBuffer);
    private:
        static std::vector<char> ReadFile(const std::string& filePath);
        void CreateGraphicsPipeline(
            const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& info
        );
        void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    private:
        VKDevice* m_device;
        VkPipeline m_graphicsPipeline;
        VkShaderModule m_vertexShaderModule;
        VkShaderModule m_fragmentShaderModule;

    };
} // namespace LG
