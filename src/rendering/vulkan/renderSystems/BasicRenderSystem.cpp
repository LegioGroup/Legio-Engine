#include "rendering/vulkan/renderSystems/BasicRenderSystem.h"

namespace LG
{
    struct SimplePushConstantData //Temporary
    {
        glm::mat4 transform{ 1.f };
        alignas(16) glm::vec3 color;
    };

    BasicRenderSystem::BasicRenderSystem(VKDevice* device, VkRenderPass renderPass, LGCamera* camera)
        : m_device(device)
        , m_camera(camera)
    {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
        LoadGameObjects();
    }

    BasicRenderSystem::~BasicRenderSystem()
    {
        vkDestroyPipelineLayout(m_device->GetDevice(), m_pipelineLayout, nullptr);
        m_pipeline.reset();
    }

    void BasicRenderSystem::LoadGameObjects()
    {
        std::shared_ptr<VKModel> veModel_0 = std::make_shared<VKModel>(m_device);

        auto gameObject = VKGameObject::CreateGameObject();
        gameObject.m_model = veModel_0;
        gameObject.m_transformComponent.translation = { 0.f, 0.f, 2.5f };
        gameObject.m_transformComponent.scale = { .5f, .5f, .5f };

        auto gameObject_2 = VKGameObject::CreateGameObject();
        gameObject_2.m_model = veModel_0;
        gameObject_2.m_transformComponent.translation = { 0.f, 0.3f, 0.5f };
        gameObject_2.m_transformComponent.scale = { .4f, .4f, .4f };

        m_gameObjects.push_back(std::move(gameObject));
        m_gameObjects.push_back(std::move(gameObject_2));
    } 

    void BasicRenderSystem::CreatePipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // Optional

        if (vkCreatePipelineLayout(m_device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        } 
    }

    void BasicRenderSystem::CreatePipeline(VkRenderPass renderPass)
    {
        PipelineConfigInfo pipelineConfig = {};
        VKPipeline::DefaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_pipelineLayout;

        m_pipeline = std::make_unique<VKPipeline>(m_device,
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv",
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv",
            pipelineConfig);
    }

    void BasicRenderSystem::Render(VkCommandBuffer commandBuffer)
    {
        m_pipeline->Bind(commandBuffer);
        auto projection = m_camera->GetProjection() * m_camera->GetView();

        for(auto& obj : m_gameObjects)
        {

            obj.m_transformComponent.rotation.y = glm::mod(obj.m_transformComponent.rotation.y + 0.0001f, glm::two_pi<float>());
            obj.m_transformComponent.rotation.x = glm::mod(obj.m_transformComponent.rotation.x + 0.0001f, glm::two_pi<float>());
            obj.m_transformComponent.rotation.z = glm::mod(obj.m_transformComponent.rotation.z - 0.0002f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.color = obj.m_color;
            push.transform = projection * obj.m_transformComponent.Mat4();

            vkCmdPushConstants(commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

            obj.m_model->Bind(commandBuffer);
            obj.m_model->Draw(commandBuffer);
        }

    }
    
} // namespace LG
