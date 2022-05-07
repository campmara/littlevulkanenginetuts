#include "App.h"
#include "KeyboardMovementController.h"
#include "Buffer.h"
#include "Camera.h"
#include "SimpleRenderSystem.h"
#include "Math.h"

#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace XIV {
    struct GlobalUbo {
        Mat4 ProjectionView{1.f};
        Vec4 AmbientLightColor{1.0f, 1.0f, 1.0f, 0.02f}; // w is intensity
        Vec3 LightPosition{-1.0f};
        alignas(16) Vec4 LightColor{1.0f}; // w is light intensity
    };

    App::App() {
        globalPool =
            DescriptorPool::Builder(device)
                .SetMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                .Build();
        LoadGameObjects();
    }

    App::~App() {}

    void App::Run() {
        // Buffer shit
        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < uboBuffers.size(); ++i) {
            uboBuffers[i] = std::make_unique<Buffer>(device,
                                                     sizeof(GlobalUbo),
                                                     1,
                                                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->Map();
        }

        auto globalSetLayout =
            DescriptorSetLayout::Builder(device)
                .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                .Build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < globalDescriptorSets.size(); ++i) {
            auto bufferInfo = uboBuffers[i]->DescriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .WriteBuffer(0, &bufferInfo)
                .Build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{device,
                                              renderer.GetSwapChainRenderPass(),
                                              globalSetLayout->VulkanDescriptorSetLayout};
        Camera camera{};

        auto viewerObject = GameObject::CreateGameObject();
        viewerObject.Transform.Translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        // dt stuff
        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window.ShouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime)
                    .count();
            currentTime = newTime;

            cameraController.MoveInPlaneXZ(window.GlfwWindow, frameTime, viewerObject);
            camera.SetViewXYZ(viewerObject.Transform.Translation, viewerObject.Transform.Rotation);

            float aspect = renderer.GetAspectRatio();
            camera.SetPerspectiveProjection(Math::Deg2Rad(50.0f), aspect, 0.1f, 100.0f);

            if (auto commandBuffer = renderer.BeginFrame()) {
                int frameIndex = renderer.GetFrameIndex();
                FrameInfo frameInfo{frameIndex,
                                    frameTime,
                                    commandBuffer,
                                    camera,
                                    globalDescriptorSets[frameIndex],
                                    gameObjects};

                // update
                GlobalUbo ubo{};
                ubo.ProjectionView = camera.ProjectionMatrix * camera.ViewMatrix;
                uboBuffers[frameIndex]->WriteToBuffer(&ubo);
                uboBuffers[frameIndex]->Flush();

                // render
                renderer.BeginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.RenderGameObjects(frameInfo);
                renderer.EndSwapChainRenderPass(commandBuffer);
                renderer.EndFrame();
            }
        }

        vkDeviceWaitIdle(device.VulkanDevice);
    }

    void App::LoadGameObjects() {
        std::shared_ptr<Model> model = Model::CreateModelFromFile(device, "models/flat_vase.obj");
        auto flatVase = GameObject::CreateGameObject();
        flatVase.Model = model;
        flatVase.Transform.Translation = {-.5f, .5f, 0.0f};
        flatVase.Transform.Scale = {3.f, 1.5f, 3.f};
        gameObjects.emplace(flatVase.Id, std::move(flatVase));

        model = Model::CreateModelFromFile(device, "models/smooth_vase.obj");
        auto smoothVase = GameObject::CreateGameObject();
        smoothVase.Model = model;
        smoothVase.Transform.Translation = {.5f, .5f, 0.0f};
        smoothVase.Transform.Scale = {3.f, 1.5f, 3.f};
        gameObjects.emplace(smoothVase.Id, std::move(smoothVase));

        model = Model::CreateModelFromFile(device, "models/quad.obj");
        auto floor = GameObject::CreateGameObject();
        floor.Model = model;
        floor.Transform.Translation = {0.f, .5f, 0.f};
        floor.Transform.Scale = {3.f, 1.f, 3.f};
        gameObjects.emplace(floor.Id, std::move(floor));
    }
} // namespace XIV