#include "vector2.hpp"

Vector2::Vector2() : x(0.0f), y(0.0f) { }

Vector2::Vector2(const float& _x, const float& _y) : x(_x), y(_y) { }

Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) { }

void Vector2::SetX(float x) { this->x = x; }

void Vector2::SetY(float y) { this->y = y; }

float Vector2::X() const { return x; }

float Vector2::Y() const { return y; }

const Vector2& Vector2::operator = (const Vector2& v) { 
    SetX(v.x);
    SetY(v.y);
    return *this;
}

const Vector2& Vector2::operator + (const Vector2& v) { 
    x += v.x;
    y += v.y;
    return *this;
}

const Vector2& Vector2::operator - (const Vector2& v) { 
    x -= v.x;
    y -= v.y;
    return *this;
}
const Vector2& Vector2::operator * (const float& s) { 
    x = s * x;
    y = s * y;
    return *this;
}
const Vector2& Vector2::operator / (const float& s) { 
    x = x / s;
    y = y / s;
    return *this;
}

float Vector2::Dot(const Vector2& l, const Vector2& r) {
    return l.x * r.x + l.y * r.y;
}