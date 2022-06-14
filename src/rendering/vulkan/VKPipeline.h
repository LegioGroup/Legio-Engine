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

        VkViewport viewport; //Transformation between pipeline's output and target image
        VkRect2D scissors; //Pixels outside this rectangle will be discarded
        VkPipelineViewportStateCreateInfo viewportState;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlending;
        VkPipelineDynamicStateCreateInfo dynamicState;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        VkFormat swapChainImageFormat;
        uint32_t subpass = 0;
    };

    class VKPipeline
    {
    public:
        VKPipeline(
            VkDevice device,
            const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& info
        );
        
        ~VKPipeline();
        VKPipeline(const VKPipeline&) = delete;
        VKPipeline& operator=(const VKPipeline&) = delete;
        static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height);

    private:
        static std::vector<char> ReadFile(const std::string& filePath);
        void CreateGraphicsPipeline(
            const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& info
        );
        void CreateRenderPass(const PipelineConfigInfo& info);
        void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    private:
        VkDevice m_device;
        VkPipeline m_graphicsPipeline;
        VkPipelineLayout m_pipelineLayout;
        VkRenderPass m_renderPass;
        VkShaderModule m_vertexShaderModule;
        VkShaderModule m_fragmentShaderModule;

    };
} // namespace LG
