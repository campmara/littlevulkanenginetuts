#include "model.h"
#include "utils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <cassert>
#include <cstring>
#include <unordered_map>

namespace std {
    template <> struct hash<XIV::Render::Model::Vertex> {
        size_t operator()(XIV::Render::Model::Vertex const &vertex) const {
            size_t seed = 0;
            XIV::HashCombine(seed, vertex.Position, vertex.Color, vertex.Normal, vertex.Uv);
            return seed;
        }
    };
} // namespace std

namespace XIV::Render {
#pragma region Vertex Struct Member Functions
    std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        attributeDescriptions.push_back(
            {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Position)});
        attributeDescriptions.push_back(
            {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Color)});
        attributeDescriptions.push_back(
            {2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, Normal)});
        attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, Uv)});
        return attributeDescriptions;
    }

    std::unique_ptr<Model> Model::CreateModelFromFile(Device &device, const std::string &path) {
        Builder builder{};
        builder.LoadModel(path);
        return std::make_unique<Model>(device, builder);
    }
#pragma endregion

#pragma region Builder Struct Member Functions

    void Model::Builder::LoadModel(const std::string &path) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        std::string finalPath = RESOURCE_DIR + path;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, finalPath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        Vertices.clear();
        Indices.clear();

        std::unordered_map<Vertex, u32> uniqueVertices{};
        for (const auto &shape : shapes) {
            for (const auto &index : shape.mesh.indices) {
                Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.Position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2],
                    };

                    vertex.Color = {
                        attrib.colors[3 * index.vertex_index + 0],
                        attrib.colors[3 * index.vertex_index + 1],
                        attrib.colors[3 * index.vertex_index + 2],
                    };
                }

                if (index.normal_index >= 0) {
                    vertex.Normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.Uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(Vertices.size());
                    Vertices.push_back(vertex);
                }
                Indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

#pragma endregion

#pragma region Model Class Member Functions
    Model::Model(Device &device, const Model::Builder &builder) : device(device) {
        CreateVertexBuffers(builder.Vertices);
        CreateIndexBuffers(builder.Indices);
    }

    Model::~Model() {}

    void Model::Bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer->VulkanBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer->VulkanBuffer, 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void Model::Draw(VkCommandBuffer commandBuffer) {
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void Model::CreateVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3.");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        u32 vertexSize = sizeof(vertices[0]);

        Buffer stagingBuffer{device,
                             vertexSize,
                             vertexCount,
                             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *)vertices.data());

        vertexBuffer = std::make_unique<Buffer>(device,
                                                vertexSize,
                                                vertexCount,
                                                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                                                    VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        device.CopyBuffer(stagingBuffer.VulkanBuffer, vertexBuffer->VulkanBuffer, bufferSize);
    }

    void Model::CreateIndexBuffers(const std::vector<u32> &indices) {
        indexCount = static_cast<u32>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) {
            return;
        }

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
        u32 indexSize = sizeof(indices[0]);

        Buffer stagingBuffer{
            device,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *)indices.data());

        indexBuffer = std::make_unique<Buffer>(device,
                                               indexSize,
                                               indexCount,
                                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                                                   VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        device.CopyBuffer(stagingBuffer.VulkanBuffer, indexBuffer->VulkanBuffer, bufferSize);
    }
#pragma endregion
} // namespace XIV