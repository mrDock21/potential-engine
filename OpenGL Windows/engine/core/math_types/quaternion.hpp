#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "math_types/vector3.hpp"

class Quaternion {

    // Matrix4 may now have access private things
    friend class Matrix4;

    public:
        Quaternion();
        Quaternion(const Vector3&);

        void SetEulerAngles(const Vector3&);
        Vector3 EulerAngles() const;

        const Quaternion& operator = (const Quaternion&);
        const Quaternion& operator *= (const Quaternion&);
        friend Quaternion operator * (Quaternion, const Quaternion&);

        static Quaternion AngleAxis(const float&, const Vector3&);

    private:
        glm::quat quaternion;

        Quaternion(const glm::quat&);
};