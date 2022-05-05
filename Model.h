#ifndef MODEL_H
#define MODEL_H

#include "Device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace XIV {
    class Model {
    public:
        struct Vertex {
            glm::vec2 Position;
            glm::vec3 Color;
            static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
        };

        Model(Device &device, const std::vector<Vertex> &vertices);
        ~Model();
        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);

        Device &device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        u32 vertexCount;
    };
} // namespace XIV

#endif