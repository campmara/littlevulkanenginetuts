#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Render/Model.h"
#include "Wrath.h"

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

    class GameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        static GameObject CreateGameObject() {
            static id_t currentId = 0;
            return GameObject{currentId++};
        }

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) = default;

        id_t Id;

        std::shared_ptr<Model> Model{};
        Vec3 Color{};
        TransformComponent Transform{};

    private:
        GameObject(id_t objId) : Id{objId} {}
    };
} // namespace XIV

#endif