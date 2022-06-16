#include <array>
#include <set>
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <functional>
#include <Legio/ServiceLocator.h>
#include <Legio/platform/Log.h>
#include "platform/EngineWindow.h"
#include "platform/WindowsInput.h"
#include "rendering/vulkan/VKRenderer.h"
namespace LG
{
    void VKRenderer::Init(RendererSettings settings)
    {
        LG_CORE_INFO("Initialize Vulkan Renderer!");
        auto* window = static_cast<EngineWindow*>(ServiceLocator::GetWindow());
        m_device = std::make_unique<VKDevice>();
        m_swapChain = std::make_unique<VKSwapChain>(m_device.get(), window->GetExtent());
        RecreateSwapChain();
        m_renderSystem = std::make_unique<BasicRenderSystem>(m_device.get(), m_swapChain->GetRenderPass(), &m_camera);
        CreateCommandBuffers();
    }

    void VKRenderer::Shutdown()
    {
        LG_CORE_INFO("Shutting Down Vulkan Renderer!");

        m_swapChain.reset();
        m_renderSystem.reset();
        m_device.reset();
    }

    void VKRenderer::RecreateSwapChain()
    {
        auto* window = static_cast<EngineWindow*>(ServiceLocator::GetWindow());
        auto extent = window->GetExtent();

        while (extent.width == 0 || extent.height == 0)
        {
            extent = window->GetExtent();
            glfwWaitEvents();
        }

        RendererWaitIdle();
        m_swapChain.reset();

        if (m_swapChain == nullptr)
        {
            m_swapChain = std::make_unique<VKSwapChain>(m_device.get(), extent);
        }
        else
        {
            m_swapChain = std::make_unique<VKSwapChain>(m_device.get(), extent, std::move(m_swapChain));
            if (m_swapChain->ImageCount() != m_commandBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }
    }

    void VKRenderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(m_device->GetDevice(), m_device->GetCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
        m_commandBuffers.clear();
    }

    void VKRenderer::CreateCommandBuffers()
    {
        m_commandBuffers.resize(m_swapChain->ImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_device->GetCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

        if (vkAllocateCommandBuffers(m_device->GetDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void VKRenderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_swapChain->GetRenderPass();
        renderPassInfo.framebuffer = m_swapChain->GetFrameBuffer(imageIndex);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_swapChain->GetSwapChainExtent();

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

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

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        m_renderSystem->Render(commandBuffer);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

    }

    void VKRenderer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<FrameBufferResizeEvent>(LG_BIND_EVENT_FN(VKRenderer::OnFrameBufferResizeEvent));
        dispatcher.Dispatch<AppTickEvent>(LG_BIND_EVENT_FN(VKRenderer::OnAppTickEvent));
    }

    bool VKRenderer::OnAppTickEvent(AppTickEvent& event)
    {
        // float fixedTick = event.GetFixedTick();
        // glm::vec3 rotate{ 0.f };

        // if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_RIGHT))  rotate.y +=1;
        // if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_LEFT))   rotate.y -=1;
        // if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_UP))     rotate.x +=1;
        // if(ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_DOWN))   rotate.x -=1;

        // if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) 
        // {
        //     m_cameraGameObject.m_transformComponent.rotation += m_camera.GetLookSpeed() * fixedTick * glm::normalize(rotate);
        // }

        // m_cameraGameObject.m_transformComponent.rotation.x = glm::clamp(m_cameraGameObject.m_transformComponent.rotation.x, -1.5f, 1.5f);
        // m_cameraGameObject.m_transformComponent.rotation.y = glm::mod(m_cameraGameObject.m_transformComponent.rotation.y, glm::two_pi<float>());

        // float yaw = m_cameraGameObject.m_transformComponent.rotation.y;
        // const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        // const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        // const glm::vec3 upDir{ 0.f, -1.f, 0.f };

        // glm::vec3 moveDir { 0.f };
        // if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_W)) moveDir += forwardDir;
        // if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_S)) moveDir -= forwardDir;
        // if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_D)) moveDir += rightDir;
        // if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_A)) moveDir -= rightDir;
        // if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_Q)) moveDir += upDir;
        // if (ServiceLocator::GetInput()->IsKeyPressed(LG_KEY_E)) moveDir -= upDir;

    
        // if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) 
        // {
        //     m_cameraGameObject.m_transformComponent.translation += m_camera.GetMoveSpeed() * fixedTick * glm::normalize(moveDir);
        // }

        // m_camera.SetViewYXZ(m_cameraGameObject.m_transformComponent.translation, m_cameraGameObject.m_transformComponent.rotation);
        // m_camera.SetPerspectiveProjection(glm::radians(50.f), m_swapChain->ExtentAspectRatio(), 0.1f, 10000.f);

        return false;
    }

    bool VKRenderer::OnFrameBufferResizeEvent(FrameBufferResizeEvent& e)
    {
        m_needRecreateSwapChain = true;
        return false;
    }

    void VKRenderer::RenderFrame()
    {
        VkResult result = m_swapChain->AcquireNextImage(&m_currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_needRecreateSwapChain)
        {
            m_needRecreateSwapChain = false;
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        m_swapChain->ResetFences(&m_commandBuffers[m_currentImageIndex]);

        RecordCommandBuffer(m_commandBuffers[m_currentImageIndex], m_currentImageIndex);

        m_swapChain->SubmitCommandBuffers(&m_commandBuffers[m_currentImageIndex], &m_currentImageIndex);
    }

    void VKRenderer::RendererWaitIdle()
    {
        vkDeviceWaitIdle(m_device->GetDevice());
    }

} // namespace LG