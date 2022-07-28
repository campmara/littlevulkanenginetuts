#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "render/model.h"
#include "wrath.h"

#include <memory>
#include <unordered_map>

using namespace XIV::Render;

namespace XIV {
    struct TransformComponent {
        Vec3 Translation{};
        Vec3 Scale{1.0f, 1.0f, 1.0f};
        Vec3 Rotation{};
        Mat4 Matrix4();
        Mat3 NormalMatrix();
    };

    struct PointLightComponent {
        float LightIntensity = 1.0f;
    };

    class GameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        static GameObject CreateGameObject() {
            static id_t currentId = 0;
            return GameObject{currentId++};
        }

        static GameObject
        CreatePointLight(float intensity = 10.0f, float radius = 0.1f, Vec3 color = Vec3(1.0f));

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) = default;

        id_t Id;

        Vec3 Color{};
        TransformComponent Transform{};

        // Optional pointer components
        std::shared_ptr<Model> Model{};
        std::unique_ptr<PointLightComponent> PointLight = nullptr;

    private:
        GameObject(id_t objId) : Id{objId} {}
    };
} // namespace XIV

#endif