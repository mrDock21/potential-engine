#include "vector3.hpp"

using namespace CEngine;

Vector3::Vector3() : vec(0.0f, 0.0f, 0.0f) { }

Vector3::Vector3(const float& value) 
    : vec(value) { }


Vector3::Vector3(const float& x, const float& y, const float& z) 
    : vec(x, y, z) { }

Vector3::Vector3(const Vector3& v) : vec(v.vec) { }

Vector3::Vector3(const glm::vec3& v) : vec(v) { }

void Vector3::SetX(float x) { vec.x = x; }

void Vector3::SetY(float y) { vec.y = y; }

void Vector3::SetZ(float z) { vec.z = z; }

float Vector3::X() const { return vec.x; }

float Vector3::Y() const { return vec.y; }

float Vector3::Z() const { return vec.z; }

Vector3 Vector3::Normalized() const {
    return Normalize(*this);
}

const Vector3& Vector3::operator = (const Vector3& v) { 
    vec = v.vec;
    return *this;
}

const Vector3& Vector3::operator += (const Vector3& v) { 
    vec += v.vec;
    return *this;
}

Vector3 CEngine::operator + (Vector3 l, const Vector3& r) {
    l.vec = l.vec + r.vec;
    return l;
}

Vector3 CEngine::operator - (Vector3 l, const Vector3& r) {
    l.vec = l.vec - r.vec;
    return l;
}

Vector3 CEngine::operator - (Vector3 v) {
    v.vec = -v.vec;
    return v;
}

Vector3 CEngine::operator * (Vector3 l, float s) {
    l.vec = l.vec * s;
    return l;
}

Vector3 CEngine::operator / (Vector3 l, float s) {
    l.vec = l.vec / s;
    return l;
}

Vector3 CEngine::operator * (float s, Vector3 r) {
    r.vec = r.vec * s;
    return r;
}

Vector3 CEngine::operator / (float s, Vector3 r) {
    r.vec = r.vec / s;
    return r;
}

Vector3 Vector3::Up(){ 
    return Vector3(0.0f, 1.0f, 0.0f);
}

Vector3 Vector3::Right(){ 
    return Vector3(1.0f, 0.0f, 0.0f);
}

Vector3 Vector3::Forward(){ 
    return Vector3(0.0f, 0.0f, 1.0f);
}

float Vector3::Dot(const Vector3& l, const Vector3& r) {
    return glm::dot(l.vec, r.vec);
}

Vector3 Vector3::Cross(const Vector3& l, const Vector3& r) {
    glm::vec3 c = glm::cross(l.vec, r.vec);
    return Vector3(c.x, c.y, c.z);
}

Vector3 Vector3::Normalize(const Vector3& v) {
    glm::vec3 aux = glm::normalize(v.vec);
    return Vector3(aux.x, aux.y, aux.z);
}