#include "rendering/vulkan/renderSystems/VKSimpleRenderSystem.h"
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
namespace LG
{
    struct SimplePushConstantData //Temporary
    {
        glm::mat4 transform{ 1.f };
        glm::mat4 normalMatrix{ 1.f };
    };

    VKSimpleRenderSystem::VKSimpleRenderSystem(VKDevice& device, VkRenderPass renderPass, LGCamera& camera)
        : m_device(device)
        , m_camera(camera)
    {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
        LoadGameObjects();
        LG_CORE_WARN("\t\tVKSimpleRenderSystem Created!");
    }

    VKSimpleRenderSystem::~VKSimpleRenderSystem()
    {
        LG_CORE_WARN("\t\tVKSimpleRenderSystem Destroyed!");
        vkDestroyPipelineLayout(m_device.GetDevice(), m_pipelineLayout, nullptr);
    }

    void VKSimpleRenderSystem::CreatePipelineLayout()
    {
        VkPushConstantRange pushConstantRange;
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(m_device.GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Pipeline Layout");
        }
    }

    void VKSimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
    {
        assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
        PipelineConfigInfo pipelineConfig = {};
        VKPipeline::DefaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_pipelineLayout;
        m_pipeline = std::make_unique<VKPipeline>(
            m_device,
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.vert.spv",
            "../../external/engine/src/rendering/vulkan/shaders/basic_shader.frag.spv",
             pipelineConfig
        );
    }

    void VKSimpleRenderSystem::LoadGameObjects()
    {
        std::shared_ptr<VKModel> veModel_0 = VKModel::CreateModelFromFile(m_device, "../../external/engine/models/colored_cube.obj");
        std::shared_ptr<VKModel> veModel_1 = VKModel::CreateModelFromFile(m_device, "../../external/engine/models/cube.obj");
        std::shared_ptr<VKModel> veModel_2 = VKModel::CreateModelFromFile(m_device, "../../external/engine/models/flat_vase.obj");
        std::shared_ptr<VKModel> veModel_3 = VKModel::CreateModelFromFile(m_device, "../../external/engine/models/smooth_vase.obj");

        auto gameObject_0 = VKGameObject::CreateGameObject();
        gameObject_0.m_model = veModel_0;
        gameObject_0.m_transformComponent.translation = { 0.f, 0.f, 2.5f };
        gameObject_0.m_transformComponent.scale = { 0.5f, 0.5f, .5f };

        auto gameObject_1 = VKGameObject::CreateGameObject();
        gameObject_1.m_model = veModel_1;
        gameObject_1.m_transformComponent.translation = { -2.3f, -0.4f, 3.5f };
        gameObject_1.m_transformComponent.scale = { 0.5f, 0.5f, .5f };
        gameObject_1.m_transformComponent.rotation = { glm::radians(45.f), glm::radians(-4.f), glm::radians(32.f) };

        auto gameObject_2 = VKGameObject::CreateGameObject();
        gameObject_2.m_model = veModel_2;
        gameObject_2.m_transformComponent.translation = { 1.1f, 2.f, 5.5f };
        //gameObject_3.m_transformComponent.scale = { 0.5f, 0.5f, .5f };
        gameObject_2.m_transformComponent.rotation = { glm::radians(32.f), glm::radians(-45.f), glm::radians(15.f) };

        auto gameObject_3 = VKGameObject::CreateGameObject();
        gameObject_3.m_model = veModel_3;
        gameObject_3.m_transformComponent.translation = { 2.1f, 2.f, 6.5f };
        gameObject_3.m_transformComponent.scale = { 2.5f, 2.5f, 2.5f };
        gameObject_3.m_transformComponent.rotation = { glm::radians(15.f), glm::radians(-3.f), glm::radians(45.f) };

        m_gameObjects.push_back(std::move(gameObject_0));
        m_gameObjects.push_back(std::move(gameObject_1));
        m_gameObjects.push_back(std::move(gameObject_2));
        m_gameObjects.push_back(std::move(gameObject_3));
    }

    void VKSimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer)
    {
        m_pipeline->Bind(commandBuffer); //Bind Pipeline
        auto projectionView = m_camera.GetProjection() * m_camera.GetView();
        for(auto& obj : m_gameObjects)
        {
            obj.m_transformComponent.rotation.y = glm::mod(obj.m_transformComponent.rotation.y + 0.0001f, glm::two_pi<float>());
            obj.m_transformComponent.rotation.x = glm::mod(obj.m_transformComponent.rotation.x + 0.0001f, glm::two_pi<float>());
            obj.m_transformComponent.rotation.z = glm::mod(obj.m_transformComponent.rotation.z - 0.0002f, glm::two_pi<float>());

            SimplePushConstantData push{};
            auto modelMatrix = obj.m_transformComponent.Mat4();
            push.transform = projectionView * modelMatrix; //Might want to do in the vertex Shader
            push.normalMatrix = obj.m_transformComponent.NormalMatrix();;

            vkCmdPushConstants(commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            obj.m_model->Bind(commandBuffer); //Bind model that contains vertex data

            obj.m_model->Draw(commandBuffer); //Draw
        }
    }
}