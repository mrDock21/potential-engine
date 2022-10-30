#include "matrix4.hpp"

Matrix4::Matrix4() : matrix(0.0f) { }
        
Matrix4::Matrix4(const Matrix4& m) : matrix(m.matrix) { }

Matrix4::Matrix4(const glm::mat4& m) : matrix(m) { }

const Matrix4& Matrix4::operator = (const Matrix4& r) {
    matrix = r.matrix;
    return *this;
}

const Matrix4& Matrix4::operator *= (const Matrix4& r) {
    matrix = matrix * r.matrix;
    return *this;
}

Matrix4 operator * (Matrix4 l, const Matrix4& r) {
    return Matrix4( l.matrix * r.matrix );
}

Vector4 Matrix4::Multiply(const Matrix4& m4x4, const Vector4& v4) {
    auto res = m4x4.matrix * v4.vec;
    return Vector4(res.x, res.y, res.z, res.w);
}

Matrix4 Matrix4::Indentity() {
    return Matrix4( glm::mat4(1.0f) );
}

Matrix4 Matrix4::FromQuaternion(const Quaternion& q) {
    return Matrix4( glm::toMat4(q.quaternion) );
}

Matrix4 Matrix4::Translate(const Matrix4& m, const Vector3& v) {
    return Matrix4( glm::translate(m.matrix, v.vec) );
}

Matrix4 Matrix4::Rotate(const Matrix4& m, const float& angle, const Vector3& axis) {
    return Matrix4( glm::rotate(m.matrix, angle, axis.vec) );
}

Matrix4 Matrix4::Scale(const Matrix4& m, const Vector3& s) {
    return Matrix4( glm::scale(m.matrix, s.vec) );
}

Matrix4 Matrix4::LookAt(const Vector3& eyePos, const Vector3& posToLookAt, const Vector3& up) {
    return Matrix4( glm::lookAt(eyePos.vec, posToLookAt.vec, up.vec) );
}

Matrix4 Matrix4::Perspective(
    const float& fov, 
    const float& aspectRatio, 
    const float& nearPlane, 
    const float& farPlane
    ) {
    return Matrix4(
        glm::perspective(fov, aspectRatio, nearPlane, farPlane)
    );
}