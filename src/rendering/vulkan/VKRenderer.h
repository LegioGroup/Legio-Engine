#pragma once
#include <Legio/rendering/LGCamera.h>
#include "rendering/vulkan/VKSwapChain.h"
#include "rendering/vulkan/renderSystems/VKSimpleRenderSystem.h"
namespace LG
{
    class VKRenderer : public Renderer
    {
    public:

    struct Vulkan_Editor_InitInfo
    {
        VkInstance                      Instance;
        VkPhysicalDevice                PhysicalDevice;
        VkDevice                        Device;
        uint32_t                        QueueFamily;
        VkQueue                         Queue;
        VkPipelineCache                 PipelineCache;
        VkDescriptorPool                DescriptorPool;
        uint32_t                        Subpass;
        uint32_t                        MinImageCount;          // >= 2
        uint32_t                        ImageCount;             // >= MinImageCount
        VkSampleCountFlagBits           MSAASamples;            // >= VK_SAMPLE_COUNT_1_BIT (0 -> default to VK_SAMPLE_COUNT_1_BIT)
        const VkAllocationCallbacks*    Allocator;
        void                            (*CheckVkResultFn)(VkResult err);
    };
        VKRenderer() = default;
        VKRenderer(const VKRenderer&) = delete;
        VKRenderer& operator=(const VKRenderer&) = delete;

        virtual ~VKRenderer();

        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame()  override;
        virtual void RendererWaitIdle() override;
        inline float GetAspectRatio() const { return m_swapChain->ExtentAspectRatio(); }
        virtual void OnEvent(Event& event) override;

        // inline VKDevice* GetDevice() const { return m_device.get(); };
        inline VkRenderPass GetRenderPass() const { return m_swapChain->GetRenderPass(); };

        inline Vulkan_Editor_InitInfo& GetRendererEditorInitInfo() { return m_renderEditorInitInfo; }
    private:

        void BeginFrame();
        void EndFrame();

        void BeginSwapChainRenderPass();
        void EndSwapChainRenderPass();

        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void RecreateSwapChain();

        bool OnFrameBufferResizeEvent(FrameBufferResizeEvent& e);
        bool OnAppTickEvent(AppTickEvent& event);
    private:
        EngineWindow* m_engineWindow;
        std::unique_ptr<VKDevice> m_device;
        std::unique_ptr<VKSwapChain> m_swapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::unique_ptr<VKSimpleRenderSystem> m_simpleRenderSystem;

        Vulkan_Editor_InitInfo m_renderEditorInitInfo;
        //To keep track of the camera transform.
        VKGameObject m_cameraGameObject = VKGameObject::CreateGameObject();
        LGCamera m_camera;
        uint32_t m_currentImageIndex;
        bool m_isFrameStarted;
        bool m_needRecreateSwapChain = false;
    };
} // namespace LG
