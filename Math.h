#ifndef MATH_H
#define MATH_H

// GLM WRAPPER
// Static handlers for glm functions, in order to abstract away all of the glm::x everywhere.

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <limits>

typedef glm::vec1 Vec1;
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::mat2 Mat2;
typedef glm::mat3 Mat3;
typedef glm::mat4 Mat4;

namespace XIV {
    class Math {
    public:
        // FLOAT
        static float Clamp(float x, float min, float max) {
            return glm::clamp(x, min, max);
        }

        static float Pi() {
            return glm::pi<float>();
        }

        static float TwoPi() {
            return glm::two_pi<float>();
        }

        static float Epsilon() {
            return std::numeric_limits<float>::epsilon();
        }

        static float Mod(float x, float y) {
            return glm::mod(x, y);
        }

        // VECTOR 2

        // VECTOR 3
        static Vec3 Normalize(const Vec3 x) {
            return glm::normalize(x);
        }

        static float Dot(Vec3 x, Vec3 y) {
            return glm::dot(x, y);
        }
    };
} // namespace XIV

#endif