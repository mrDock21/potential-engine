#include "vector4.hpp"

using namespace CEngine;

Vector4::Vector4() : vec() { }

Vector4::Vector4(const float& n) : vec(n) { }

Vector4::Vector4(const float& x, const float& y, const float& z, const float& w) 
    : vec(x, y, z, w) { }

Vector4::Vector4(const Vector3& v3, const float& w) 
    : vec(v3.X(), v3.Y(), v3.Z(), w) { }

Vector4::Vector4(const Vector4& v) : vec(v.vec) { }

void Vector4::SetX(float x) { vec.x = x; }

void Vector4::SetY(float y) { vec.y = y; }

void Vector4::SetZ(float z) { vec.z = z; }

void Vector4::SetW(float w) { vec.w = w; }

float Vector4::X() const { return vec.x; }

float Vector4::Y() const { return vec.y; }

float Vector4::Z() const { return vec.z; }

float Vector4::W() const { return vec.w; }

Vector3 Vector4::ToVector3() const {
    return Vector3( vec.x, vec.y, vec.z );
}

const Vector4& Vector4::operator = (const Vector4& r) {
    vec = r.vec;
    return *this;
}

const Vector4& Vector4::operator += (const Vector4& r) {
    vec += r.vec;
    return *this;
}

Vector4 CEngine::operator + (Vector4 l, const Vector4& r) {
    l.vec = l.vec + r.vec;
    return l;
}

Vector4 CEngine::operator - (Vector4 l, const Vector4& r) {
    l.vec = l.vec - r.vec;
    return l;
}

Vector4 CEngine::operator * (Vector4 v, float s) {
    v.vec = v.vec * s;
    return v;
}

Vector4 CEngine::operator / (Vector4 v, float s) {
    v.vec = v.vec / s;
    return v;
}

Vector4 CEngine::operator * (float s, Vector4 v) {
    v = v * s;
    return v;
}

Vector4 CEngine::operator / (float s, Vector4 v) {
    v = v / s;
    return v;
}