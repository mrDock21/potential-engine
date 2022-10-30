#include "quaternion.hpp"

Quaternion::Quaternion() : quaternion() { }

Quaternion::Quaternion(const Vector3& angles) {
    SetEulerAngles(angles);
}

Quaternion::Quaternion(const glm::quat& quat) : quaternion(quat) { }

void Quaternion::SetEulerAngles(const Vector3& angles) {
    quaternion = glm::quat(
        // this is possible because Quaternion is a FRIEND of Vector3
        glm::radians(angles.vec)
    );
}

Vector3 Quaternion::EulerAngles() const {
    return Vector3(
        // this is possible because Quaternion is a FRIEND of Vector3
        glm::degrees( glm::eulerAngles(quaternion) )
    );
}

const Quaternion& Quaternion::operator = (const Quaternion& r) { 
    quaternion = r.quaternion;
    return *this;
}

const Quaternion& Quaternion::operator *= (const Quaternion& r) {
    quaternion *= r.quaternion;
    return *this;
}

Quaternion operator * (Quaternion l, const Quaternion& r) {
    l.quaternion = l.quaternion * r.quaternion;
    return l;
}

Quaternion Quaternion::AngleAxis(const float& angle, const Vector3& axis) {
    return Quaternion(
        // this is possible because Quaternion is a FRIEND of Vector3
        glm::angleAxis(glm::radians(angle), axis.vec)
    );
}