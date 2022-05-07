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
        Vec3 LightDirection = Math::Normalize(Vec3{1.f, -3.f, -1.f});
    };

    App::App() {
        LoadGameObjects();
    }

    App::~App() {}

    void App::Run() {
        // Buffer shit
        Buffer globalUboBuffer{
            device,
            sizeof(GlobalUbo),
            SwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            device.Properties.limits.minUniformBufferOffsetAlignment,
        };
        globalUboBuffer.Map();

        // Setup the camera
        SimpleRenderSystem simpleRenderSystem{device, renderer.GetSwapChainRenderPass()};
        Camera camera{};

        auto viewerObject = GameObject::CreateGameObject();
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
            camera.SetPerspectiveProjection(Math::Deg2Rad(50.0f), aspect, 0.1f, 10.0f);

            if (auto commandBuffer = renderer.BeginFrame()) {
                int frameIndex = renderer.GetFrameIndex();
                FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};

                // update
                GlobalUbo ubo{};
                ubo.ProjectionView = camera.ProjectionMatrix * camera.ViewMatrix;
                globalUboBuffer.WriteToIndex(&ubo, frameIndex);
                globalUboBuffer.FlushIndex(frameIndex);

                // render
                renderer.BeginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.RenderGameObjects(frameInfo, gameObjects);
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
        flatVase.Transform.Translation = {-.5f, .5f, 2.5f};
        flatVase.Transform.Scale = {3.f, 1.5f, 3.f};
        gameObjects.push_back(std::move(flatVase));

        model = Model::CreateModelFromFile(device, "models/smooth_vase.obj");
        auto smoothVase = GameObject::CreateGameObject();
        smoothVase.Model = model;
        smoothVase.Transform.Translation = {.5f, .5f, 2.5f};
        smoothVase.Transform.Scale = {3.f, 1.5f, 3.f};
        gameObjects.push_back(std::move(smoothVase));
    }
} // namespace XIV