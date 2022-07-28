#include "gameobject.h"

namespace XIV {
    Mat4 TransformComponent::Matrix4() {
        const float c3 = Wrath::Cos(Rotation.z);
        const float s3 = Wrath::Sin(Rotation.z);
        const float c2 = Wrath::Cos(Rotation.x);
        const float s2 = Wrath::Sin(Rotation.x);
        const float c1 = Wrath::Cos(Rotation.y);
        const float s1 = Wrath::Sin(Rotation.y);
        return Mat4{{
                        Scale.x * (c1 * c3 + s1 * s2 * s3),
                        Scale.x * (c2 * s3),
                        Scale.x * (c1 * s2 * s3 - c3 * s1),
                        0.0f,
                    },
                    {
                        Scale.y * (c3 * s1 * s2 - c1 * s3),
                        Scale.y * (c2 * c3),
                        Scale.y * (c1 * c3 * s2 + s1 * s3),
                        0.0f,
                    },
                    {
                        Scale.z * (c2 * s1),
                        Scale.z * (-s2),
                        Scale.z * (c1 * c2),
                        0.0f,
                    },
                    {Translation.x, Translation.y, Translation.z, 1.0f}};
    }

    Mat3 TransformComponent::NormalMatrix() {
        const float c3 = Wrath::Cos(Rotation.z);
        const float s3 = Wrath::Sin(Rotation.z);
        const float c2 = Wrath::Cos(Rotation.x);
        const float s2 = Wrath::Sin(Rotation.x);
        const float c1 = Wrath::Cos(Rotation.y);
        const float s1 = Wrath::Sin(Rotation.y);
        const Vec3 invScale = 1.0f / Scale;

        return Mat3{
            {
                invScale.x * (c1 * c3 + s1 * s2 * s3),
                invScale.x * (c2 * s3),
                invScale.x * (c1 * s2 * s3 - c3 * s1),
            },
            {
                invScale.y * (c3 * s1 * s2 - c1 * s3),
                invScale.y * (c2 * c3),
                invScale.y * (c1 * c3 * s2 + s1 * s3),
            },
            {
                invScale.z * (c2 * s1),
                invScale.z * (-s2),
                invScale.z * (c1 * c2),
            },
        };
    }

    // Creates a GameObject with an attached PointLightComponent
    GameObject GameObject::CreatePointLight(float intensity, float radius, Vec3 color) {
        GameObject gameObj = GameObject::CreateGameObject();
        gameObj.Color = color;
        gameObj.Transform.Scale.x = radius;
        gameObj.PointLight = std::make_unique<PointLightComponent>();
        gameObj.PointLight->LightIntensity = intensity;
        return gameObj;
    }
} // namespace XIV