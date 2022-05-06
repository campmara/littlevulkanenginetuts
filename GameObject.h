#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Model.h"

#include <memory>

namespace XIV {
    struct Transform2dComponent {
        glm::vec2 Translation{}; // (position offset)
        glm::vec2 Scale{1.f, 1.f};
        float Rotation;

        glm::mat2 Mat2() {
            const float s = glm::sin(Rotation);
            const float c = glm::cos(Rotation);
            glm::mat2 RotMatrix{{c, s}, {-s, c}};

            glm::mat2 ScaleMat{{Scale.x, 0.0f}, {0.0f, Scale.y}};
            return RotMatrix * ScaleMat;
        }
    };

    class GameObject {
    public:
        using id_t = unsigned int;

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
        glm::vec3 Color{};
        Transform2dComponent Transform2d{};

    private:
        GameObject(id_t objId) : Id{objId} {}
    };
} // namespace XIV

#endif