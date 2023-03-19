#pragma once

#include <iostream>

#include "math_types/vector2.hpp"
#include "math_types/vector4.hpp"
#include "math_types/vector3.hpp"
#include "math_types/quaternion.hpp"
#include "math_types/matrix4.hpp"

namespace Components {
    /**
     * Stores position, scale and orientation of owner
    */
    class Transform {
    public:
        Transform();
        Transform(const Transform&);

        void Translate(const Vector3&);
        void Position(const Vector3&);
        void Scale(const Vector3&);
        void Scale(const float&);
        void EulerAngles(const Vector3&);
        void RotateEulers(const Vector3&);
        void Rotation(const Quaternion&);
        void Rotate(const Quaternion&);
        void Rotate(const float&, const Vector3&);
        void SetGlobalRotation(const Quaternion&);

        void Forward(const Vector3&);
        

        Vector3 Position() const;
        Vector3 Scale() const;
        Vector3 EulerAngles() const;
        Vector3 GlobalEulersAngles() const;
        Vector3 Forward() const;
        Vector3 Right() const;
        Vector3 Up() const;
        Quaternion Rotation() const;
        Quaternion GlobalRotation() const;
        Matrix4 ModelMatrix() const;

        // Copies all transform data
        Transform& operator = (const Transform&);
    private:
        Vector3 position, scale;
        Vector3 forward, right, up;
        Quaternion rotation, globalRotation;
        Matrix4 rotationMatrix, globalRotationMatrix;

        void ComputeForwardAndRight();
    };
}