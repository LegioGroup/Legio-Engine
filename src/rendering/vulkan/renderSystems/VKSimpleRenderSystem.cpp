#include "rendering/vulkan/renderSystems/VKSimpleRenderSystem.h"
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
namespace LG
{
    struct SimplePushConstantData //Temporary
    {
        glm::mat4 transform{ 1.f };
        alignas(16) glm::vec3 color;
    };

    VKSimpleRenderSystem::VKSimpleRenderSystem(VKDevice& device, VkRenderPass renderPass, VKCamera& camera)
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
        VKModel::Builder modelBuilder{};
        modelBuilder.vertices =
        {
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };

        modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        auto model = std::make_shared<VKModel>(m_device, modelBuilder);

        auto cube = VKGameObject::CreateGameObject();
        cube.m_model = model;
        cube.m_transformComponent.scale = {0.5f, 0.5f, 0.5f};
        cube.m_transformComponent.translation = { 0.0f, 0.0f, 2.5f };

        auto cube_2 = VKGameObject::CreateGameObject();
        cube_2.m_model = model;
        cube_2.m_transformComponent.translation = { -0.3f, -0.4f, 1.5f };
        cube_2.m_transformComponent.scale = { 0.5f, 0.5f, .5f };
        cube_2.m_transformComponent.rotation = {glm::radians(45.f), glm::radians(-4.f), glm::radians(32.f) };

        auto cube_3 = VKGameObject::CreateGameObject();
        cube_3.m_model = model;
        cube_3.m_transformComponent.translation = { 1.1f, 2.f, 5.5f };
        cube_3.m_transformComponent.scale = { 0.5f, 0.5f, .5f };
        cube_3.m_transformComponent.rotation = { glm::radians(32.f), glm::radians(-45.f), glm::radians(15.f) };

        m_gameObjects.push_back(std::move(cube));
        m_gameObjects.push_back(std::move(cube_2));
        m_gameObjects.push_back(std::move(cube_3));
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
            push.color = obj.m_color;
            push.transform = projectionView * obj.m_transformComponent.mat4(); //Do it in Vertex Shader?

            vkCmdPushConstants(commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            obj.m_model->Bind(commandBuffer); //Bind model that contains vertex data

            obj.m_model->Draw(commandBuffer); //Draw
        }
    }
}