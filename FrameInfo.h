#ifndef FRAME_INFO_H
#define FRAME_INFO_H

#include "Camera.h"

#include <vulkan/vulkan.h>

namespace XIV {
    struct FrameInfo {
        int FrameIndex;
        float FrameTime;
        VkCommandBuffer CommandBuffer;
        Camera &Camera;
        VkDescriptorSet GlobalDescriptorSet;
    };
} // namespace XIV

#endif