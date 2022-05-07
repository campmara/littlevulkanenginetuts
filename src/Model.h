#ifndef MODEL_H
#define MODEL_H

#include "Buffer.h"
#include "Device.h"
#include "Math.h"

#include <memory>
#include <vector>

namespace XIV {
    class Model {
    public:
        struct Vertex {
            Vec3 Position{};
            Vec3 Color{};
            Vec3 Normal{};
            Vec2 Uv{};
            static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
            bool operator==(const Vertex &other) const {
                return Position == other.Position && Color == other.Color &&
                       Normal == other.Normal && Uv == other.Uv;
            }
        };

        struct Builder {
            std::vector<Vertex> Vertices{};
            std::vector<u32> Indices{};
            void LoadModel(const std::string &path);
        };

        Model(Device &device, const Model::Builder &builder);
        ~Model();
        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;

        static std::unique_ptr<Model> CreateModelFromFile(Device &device, const std::string &path);

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);
        void CreateIndexBuffers(const std::vector<u32> &indices);

        Device &device;
        std::unique_ptr<Buffer> vertexBuffer;
        u32 vertexCount;

        bool hasIndexBuffer = false;
        std::unique_ptr<Buffer> indexBuffer;
        u32 indexCount;
    };
} // namespace XIV

#endif