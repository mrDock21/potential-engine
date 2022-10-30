#pragma once

#include <glm/glm.hpp>

#include "math_types/vector3.hpp"

/**
 * Represents a four component vector (X, Y, Z, W)
*/
class Vector4 {
    // friends may now have access private things
    friend class Matrix4;
    // to be able to set uniforms in shaders
    friend class Shader;

    public:
        Vector4();
        Vector4(const float&);
        Vector4(const float&, const float&, const float&, const float&);
        Vector4(const Vector3&, const float&);
        Vector4(const Vector4&);

        void SetX(float);
        void SetY(float);
        void SetZ(float);
        void SetW(float);

        float X() const;
        float Y() const;
        float Z() const;
        float W() const;

        Vector3 ToVector3() const;

        const Vector4& operator = (const Vector4&);
        const Vector4& operator += (const Vector4&);
        friend Vector4 operator + (Vector4, const Vector4&);
        friend Vector4 operator - (Vector4, const Vector4&);
        friend Vector4 operator * (Vector4, float);
        friend Vector4 operator / (Vector4, float);
        friend Vector4 operator * (float, Vector4);
        friend Vector4 operator / (float, Vector4);
    private:
        glm::vec4 vec;

        Vector4(const glm::vec4&);
};