#include "vector2.hpp"

Vector2::Vector2() : vec(0.0f, 0.0f) { }

Vector2::Vector2(const float& x, const float& y) : vec(x, y) { }

Vector2::Vector2(const Vector2& v) : vec(v.vec) { }

void Vector2::SetX(float x) { vec.x = x; }

void Vector2::SetY(float y) { vec.y = y; }

float Vector2::X() const { return vec.x; }

float Vector2::Y() const { return vec.y; }

Vector2 Vector2::Normalized() const {
    return Normalize(*this);
}

const Vector2& Vector2::operator = (const Vector2& v) { 
    vec = v.vec;
    return *this;
}

const Vector2& Vector2::operator + (const Vector2& v) { 
    vec += v.vec;
    return *this;
}

const Vector2& Vector2::operator - (const Vector2& v) { 
    vec -= v.vec;
    return *this;
}
const Vector2& Vector2::operator * (const float& s) { 
    vec = s * vec;
    return *this;
}
const Vector2& Vector2::operator / (const float& s) { 
    vec = vec / s;
    return *this;
}

float Vector2::Dot(const Vector2& l, const Vector2& r) {
    return l.vec.x * r.vec.x + l.vec.y * r.vec.y;
}

float Vector2::Magnitude(const Vector2& v) {
    return v.vec.length();
}

Vector2 Vector2::Normalize(const Vector2& v) {
    Vector2 norm;
    norm.vec = ((1.0f / Magnitude(v)) * v.vec);
    return norm;
}