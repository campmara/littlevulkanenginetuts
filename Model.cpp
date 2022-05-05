#include "Model.h"

#include <cassert>
#include <cstring>

namespace XIV {
#pragma region Vertex Struct Member Functions
    std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, Position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, Color);
        return attributeDescriptions;
    }
#pragma endregion

#pragma region Model Class Member Functions
    Model::Model(Device &device, const std::vector<Vertex> &vertices) : device(device) {
        CreateVertexBuffers(vertices);
    }

    Model::~Model() {
        vkDestroyBuffer(device.VulkanDevice, vertexBuffer, nullptr);
        vkFreeMemory(device.VulkanDevice, vertexBufferMemory, nullptr);
    }

    void Model::Bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void Model::Draw(VkCommandBuffer commandBuffer) {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    void Model::CreateVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3.");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        device.CreateBuffer(bufferSize,
                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                            vertexBuffer,
                            vertexBufferMemory);

        void *data;
        vkMapMemory(device.VulkanDevice, vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device.VulkanDevice, vertexBufferMemory);
    }
#pragma endregion
} // namespace XIV