#include <array>
#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>

#include "rendering/vulkan/VKRenderer.h"

namespace LG
{
    VKRenderer::~VKRenderer()
    {
        LG_CORE_INFO("Shutting Down Vulkan Renderer!");
        vkDestroyPipelineLayout(m_device->GetDevice(), m_pipelineLayout, nullptr);
    }

    void VKRenderer::Init(RendererSettings settings)
    {
        m_engineWindow = static_cast<EngineWindow*>(ServiceLocator::GetWindow().get());
        m_device = std::make_unique<VKDevice>(*m_engineWindow);
        m_swapChain = std::make_unique<VKSwapChain>(*m_device, m_engineWindow->GetExtent());

        LoadModels();
        CreatePipelineLayout();
        RecreateSwapChain();
        CreateCommandBuffers();

        LG_CORE_INFO("Initialize Vulkan Renderer!"); 
    }

    void VKRenderer::Shutdown()
    {
    }

    void VKRenderer::RenderFrame()
    {
        uint32_t imageIndex;
        auto result = m_swapChain->AcquireNextImage(&imageIndex); //index of the frame we should render to Next

        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) 
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        RecordCommandBuffer(imageIndex);
        result = m_swapChain->SubmitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_engineWindow->WasWindowResized())
        {
            m_engineWindow->ResetWindowResizedFlag();
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to Submit command Buffers!");
        }
    }

    void VKRenderer::LoadModels()
    {
        std::vector<VKModel::Vertex> vertices
        {
            {{0.0f, -0.5f}, {1.0f,0.0f,0.0f}},
            {{0.5f, 0.5f}, {0.0f,1.0f,0.0f}},
            {{-0.5f, 0.5f}, {0.0f,0.0f,1.0f}}
        };

        m_model = std::make_unique<VKModel>(*m_device, vertices);
    }

    void VKRenderer::RendererWaitIdle()
    {
        vkDeviceWaitIdle(m_device->GetDevice()); //block CPU until all GPU operations complete.
    }

    void VKRenderer::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(m_device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Pipeline Layout");
        }
    }

    void VKRenderer::CreatePipeline()
    {
        assert(m_swapChain != nullptr && "Cannot create pipeline before swapChain!");
        assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

        PipelineConfigInfo pipelineConfig = {};
        VKPipeline::DefaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = m_swapChain->GetRenderPass();
        pipelineConfig.pipelineLayout = m_pipelineLayout;
        m_pipeline = std::make_unique<VKPipeline>(  
            *m_device,
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv",
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv",
            pipelineConfig
        );
    }

    void VKRenderer::CreateCommandBuffers()
    {
        m_commandBuffers.resize(m_swapChain->ImageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_device->GetCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

        if (vkAllocateCommandBuffers(m_device->GetDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate Command Buffers!");
        }
    }

    
    void VKRenderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(m_device->GetDevice(), m_device->GetCommandPool(),static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
        m_commandBuffers.clear();
    }

    void VKRenderer::RecreateSwapChain()
    {
        auto extent = m_engineWindow->GetExtent();
        while (extent.width == 0 || extent.height == 0) 
        {
            extent = m_engineWindow->GetExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_device->GetDevice());
        m_swapChain.reset();

        if(m_swapChain == nullptr)
        {
            m_swapChain = std::make_unique<VKSwapChain>(*m_device, extent);
        }
        else
        {
            m_swapChain = std::make_unique<VKSwapChain>(*m_device, extent, std::move(m_swapChain));
            if(m_swapChain->ImageCount() != m_commandBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }

        CreatePipeline();
    }

    void VKRenderer::RecordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording Command Buffer!");
        }

        //Configure render Pass.
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_swapChain->GetRenderPass();
        renderPassInfo.framebuffer = m_swapChain->GetFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0,0 };
        renderPassInfo.renderArea.extent = m_swapChain->GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 0.1f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        //Biging RenderPAss
        vkCmdBeginRenderPass(m_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_swapChain->GetSwapChainExtent().width);
        viewport.height = static_cast<float>(m_swapChain->GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{ {0,0}, m_swapChain->GetSwapChainExtent() };
        vkCmdSetViewport(m_commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(m_commandBuffers[imageIndex], 0, 1, &scissor);

        m_pipeline->Bind(m_commandBuffers[imageIndex]); //Bind Graphics pipeline
        m_model->Bind(m_commandBuffers[imageIndex]); //Bind model that contains vertex data
        m_model->Draw(m_commandBuffers[imageIndex]); //Draw

        vkCmdEndRenderPass(m_commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(m_commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to end recording Command Buffer!");
        }
    }

} // namespace LG
