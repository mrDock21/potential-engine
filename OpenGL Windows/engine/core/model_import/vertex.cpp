#include "vertex.hpp"

using namespace CEngine;

Vertex::Vertex() {

}

Vertex::Vertex(const Vector3& p, const Vector3& n, const Vector2& _uv) 
	: pos(p), normal(n), uv(_uv) { 

}

void Vertex::Pos(const Vector3& p) {
	pos = p;
}

void Vertex::Normal(const Vector3& n) {
	normal = n;
}

void Vertex::UV(const Vector2& _uv) {
	uv = _uv;
}

Vector3 Vertex::Pos() const {
	return pos;
}

Vector3 Vertex::Normal() const {
	return normal;
}

Vector2 Vertex::UV() const {
	return uv;
}

