#ifndef FRAME_INFO_H
#define FRAME_INFO_H

#include "Camera.h"
#include "GameObject.h"

#include <vulkan/vulkan.h>

namespace XIV::Render {
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