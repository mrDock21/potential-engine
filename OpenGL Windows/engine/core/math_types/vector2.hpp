#pragma once

#include <glm/glm.hpp>

namespace CEngine {

    /**
     * Represents a two component vector (X, Y)
    */
    class Vector2 {
        public:
            Vector2();
            Vector2(const float&, const float&);
            Vector2(const Vector2&);

            void SetX(float);
            void SetY(float);

            float X() const;
            float Y() const;

            Vector2 Normalized() const;

            const Vector2& operator = (const Vector2&);
            const Vector2& operator + (const Vector2&);
            const Vector2& operator - (const Vector2&);
            const Vector2& operator * (const float&);
            const Vector2& operator / (const float&);

            static float Dot(const Vector2&, const Vector2&);
            inline static float Magnitude(const Vector2&);
            static Vector2 Normalize(const Vector2&);
        private:
            glm::vec2 vec;
    };
}
