#pragma once

#include <glm/glm.hpp>

/**
 * Represents a three component vector (X, Y, Z)
*/
class Vector3 {
    // Quaternion may now have access private things
    friend class Quaternion;
    // to be able to manipulate vectors in glm
    friend class Matrix4;
    // to be able to set uniforms in shaders
    friend class Shader;

    public:
        Vector3();
        Vector3(const float&);
        Vector3(const float&, const float&, const float&);
        Vector3(const Vector3&);

        void SetX(float);
        void SetY(float);
        void SetZ(float);

        float X() const;
        float Y() const;
        float Z() const;

        const Vector3& operator = (const Vector3&);
        const Vector3& operator += (const Vector3&);
        friend Vector3 operator + (Vector3, const Vector3&);
        friend Vector3 operator - (Vector3, const Vector3&);
        friend Vector3 operator - (Vector3);
        friend Vector3 operator * (Vector3, float);
        friend Vector3 operator / (Vector3, float);
        friend Vector3 operator * (float, Vector3);
        friend Vector3 operator / (float, Vector3);

        static Vector3 Up();
        static Vector3 Right();
        static Vector3 Forward();
        static float Dot(const Vector3&, const Vector3&);
        static Vector3 Cross(const Vector3&, const Vector3&);
    private:
        glm::vec3 vec;

        Vector3(const glm::vec3&);
};