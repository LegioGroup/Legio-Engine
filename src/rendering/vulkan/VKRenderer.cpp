#include <array>
#include <Legio/legioCore.h>
#include "platform/WindowsInput.h"

#include "rendering/vulkan/VKRenderer.h"

namespace LG
{
    VKRenderer::~VKRenderer()
    {
        LG_CORE_INFO("Shutting Down Vulkan Renderer!");
        FreeCommandBuffers();
    }

    void VKRenderer::Init(RendererSettings settings)
    {
        LG_CORE_INFO("Initialize Vulkan Renderer!"); 

        m_engineWindow = static_cast<EngineWindow*>(ServiceLocator::GetWindow());
        m_device = std::make_unique<VKDevice>(*m_engineWindow);
        m_swapChain = std::make_unique<VKSwapChain>(*m_device, m_engineWindow->GetExtent());

        m_camera = LGCamera();
        m_camera.SetViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        m_simpleRenderSystem = std::make_unique<VKSimpleRenderSystem>(*m_device, m_swapChain->GetRenderPass(), m_camera);

        RecreateSwapChain();
        CreateCommandBuffers();
    }

    void VKRenderer::Shutdown()
    {
    }

    void VKRenderer::BeginFrame()
    {
        auto result = m_swapChain->AcquireNextImage(&m_currentImageIndex); //index of the frame we should render to Next

        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            RecreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) 
        {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        m_isFrameStarted = true;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_commandBuffers[m_currentImageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording Command Buffer!");
        }

    }
    void VKRenderer::EndFrame()
    {
        assert(m_isFrameStarted && "Can't call EndFrame while frame is not in progress");
        if (vkEndCommandBuffer(m_commandBuffers[m_currentImageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to end recording Command Buffer!");
        }

        auto result = m_swapChain->SubmitCommandBuffers(&m_commandBuffers[m_currentImageIndex], &m_currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_needRecreateSwapChain)
        {
            m_needRecreateSwapChain = false;
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to Submit command Buffers!");
        }

        m_isFrameStarted = false;
    }

    void VKRenderer::BeginSwapChainRenderPass()
    {
        assert(m_isFrameStarted && "Can't call BeginSwapChainRenderPass while frame is not in progress");
        //Configure render Pass.
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_swapChain->GetRenderPass();
        renderPassInfo.framebuffer = m_swapChain->GetFrameBuffer(m_currentImageIndex);

        renderPassInfo.renderArea.offset = { 0,0 };
        renderPassInfo.renderArea.extent = m_swapChain->GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 0.1f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        //Biging RenderPAss
        vkCmdBeginRenderPass(m_commandBuffers[m_currentImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_swapChain->GetSwapChainExtent().width);
        viewport.height = static_cast<float>(m_swapChain->GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{ {0,0}, m_swapChain->GetSwapChainExtent() };
        vkCmdSetViewport(m_commandBuffers[m_currentImageIndex], 0, 1, &viewport);
        vkCmdSetScissor(m_commandBuffers[m_currentImageIndex], 0, 1, &scissor);
    }

    void VKRenderer::EndSwapChainRenderPass()
    {
        assert(m_isFrameStarted && "Can't call EndSwapChainRenderPass while frame is not in progress");
        vkCmdEndRenderPass(m_commandBuffers[m_currentImageIndex]);
    }

    void VKRenderer::RenderFrame()
    {
        BeginFrame();
        BeginSwapChainRenderPass();

        m_simpleRenderSystem->RenderGameObjects(m_commandBuffers[m_currentImageIndex]);

        EndSwapChainRenderPass();
        EndFrame();
    }

    void VKRenderer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<FrameBufferResizeEvent>(LG_BIND_EVENT_FN(VKRenderer::OnFrameBufferResizeEvent));
        dispatcher.Dispatch<AppTickEvent>(LG_BIND_EVENT_FN(VKRenderer::OnAppTickEvent));
    }

    bool VKRenderer::OnAppTickEvent(AppTickEvent& event)
    {
        float fixedTick = event.GetFixedTick();
        glm::vec3 rotate{ 0.f };

        if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_RIGHT))  rotate.y +=1;
        if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_LEFT))   rotate.y -=1;
        if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_UP))     rotate.x +=1;
        if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_DOWN))   rotate.x -=1;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) 
        {
            m_cameraGameObject.m_transformComponent.rotation += m_camera.GetLookSpeed() * fixedTick * glm::normalize(rotate);
        }

        m_cameraGameObject.m_transformComponent.rotation.x = glm::clamp(m_cameraGameObject.m_transformComponent.rotation.x, -1.5f, 1.5f);
        m_cameraGameObject.m_transformComponent.rotation.y = glm::mod(m_cameraGameObject.m_transformComponent.rotation.y, glm::two_pi<float>());

        float yaw = m_cameraGameObject.m_transformComponent.rotation.y;
        const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        const glm::vec3 upDir{ 0.f, -1.f, 0.f };

        glm::vec3 moveDir { 0.f };
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_W)) moveDir += forwardDir;
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_S)) moveDir -= forwardDir;
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_D)) moveDir += rightDir;
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_A)) moveDir -= rightDir;
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_Q)) moveDir += upDir;
        if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_E)) moveDir -= upDir;

        
        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) 
        {
            m_cameraGameObject.m_transformComponent.translation += m_camera.GetMoveSpeed() * fixedTick * glm::normalize(moveDir);
        }

        m_camera.SetViewYXZ(m_cameraGameObject.m_transformComponent.translation, m_cameraGameObject.m_transformComponent.rotation);
        m_camera.SetPerspectiveProjection(glm::radians(50.f), GetAspectRatio(), 0.1f, 10000.f);

        return false;
    }

    bool VKRenderer::OnFrameBufferResizeEvent(FrameBufferResizeEvent& e)
    {
        m_needRecreateSwapChain = true;
        return false;
    }

    void VKRenderer::RendererWaitIdle()
    {
        vkDeviceWaitIdle(m_device->GetDevice()); //block CPU until all GPU operations complete.
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
    }
} // namespace LG
