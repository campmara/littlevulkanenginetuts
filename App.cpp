#include "App.h"
#include "SimpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

namespace XIV {
    App::App() {
        LoadGameObjects();
    }

    App::~App() {}

    void App::Run() {
        SimpleRenderSystem simpleRenderSystem{device, renderer.GetSwapChainRenderPass()};

        while (!window.ShouldClose()) {
            glfwPollEvents();
            if (auto commandBuffer = renderer.BeginFrame()) {
                renderer.BeginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.RenderGameObjects(commandBuffer, gameObjects);
                renderer.EndSwapChainRenderPass(commandBuffer);
                renderer.EndFrame();
            }
        }

        vkDeviceWaitIdle(device.VulkanDevice);
    }

    void App::LoadGameObjects() {
        std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        auto model = std::make_shared<Model>(device, vertices);

        auto triangle = GameObject::CreateGameObject();
        triangle.Model = model;
        triangle.Color = {.1f, .8f, .1f};
        triangle.Transform2d.Translation.x = .2f;
        triangle.Transform2d.Scale = {2.f, .5f};
        triangle.Transform2d.Rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }
} // namespace XIV