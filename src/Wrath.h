#ifndef WRATH_H
#define WRATH_H

// GLM MATH WRAPPER = WRATH
// Static handlers for glm functions, in order to abstract away all of the glm::x everywhere.

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <limits>

typedef glm::vec1 Vec1;
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::mat2 Mat2;
typedef glm::mat3 Mat3;
typedef glm::mat4 Mat4;

namespace XIV {
    class Wrath {
    public:
        // FLOAT
        static float Pi() {
            return glm::pi<float>();
        }

        static float TwoPi() {
            return glm::two_pi<float>();
        }

        static float Epsilon() {
            return std::numeric_limits<float>::epsilon();
        }

        static float Abs(float x) {
            return glm::abs(x);
        }

        static float Clamp(float x, float min, float max) {
            return glm::clamp(x, min, max);
        }

        static float Mod(float x, float y) {
            return glm::mod(x, y);
        }

        static float Cos(float x) {
            return glm::cos(x);
        }

        static float Sin(float x) {
            return glm::sin(x);
        }

        static float Tan(float x) {
            return glm::tan(x);
        }

        static float Deg2Rad(float degrees) {
            return glm::radians(degrees);
        }

        static float Rad2Deg(float radians) {
            return glm::degrees(radians);
        }

        // VECTOR 2

        // VECTOR 3
        static Vec3 Normalize(const Vec3 x) {
            return glm::normalize(x);
        }

        static float Dot(Vec3 x, Vec3 y) {
            return glm::dot(x, y);
        }

        static Vec3 Cross(Vec3 x, Vec3 y) {
            return glm::cross(x, y);
        }
    };
} // namespace XIV

#endif