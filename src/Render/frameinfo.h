#ifndef FRAME_INFO_H
#define FRAME_INFO_H

#include "camera.h"
#include "gameobject.h"

#include <vulkan/vulkan.h>

namespace XIV::Render {
#define MAX_LIGHTS 10

    struct PointLight {
        Vec4 Position{}; // ignore w
        Vec4 Color{};    // w is intensity
    };

    struct GlobalUbo {
        Mat4 Projection{1.0f};
        Mat4 View{1.0f};
        Mat4 InverseView{1.0f};
        Vec4 AmbientLightColor{1.0f, 1.0f, 1.0f, 0.02f}; // w is intensity
        PointLight PointLights[MAX_LIGHTS];
        int NumLights;
    };

    struct FrameInfo {
        int FrameIndex;
        float FrameTime;
        VkCommandBuffer CommandBuffer;
        Camera &Camera;
        VkDescriptorSet GlobalDescriptorSet;
        GameObject::Map &GameObjects;
    };
} // namespace XIV::Render

#endif