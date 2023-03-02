#pragma once

#include <iostream>

#include <glad/glad.h>

#include "geometry/vertex_buffer.hpp"
#include "geometry/element_buffer.hpp"
#include "shading/material.hpp"
#include "shading/Texture.hpp"
#include "math_types/transform.hpp"
#include "types/types.hpp"

class Mesh {
    public:
        Mesh();
        Mesh(float[], const u_long&, const u_long&, uint[], const u_long&);
        Mesh(float[], const u_long&, const u_long&);

        void SetAttribute(const int&, const int&, const uint&, const bool&, const u_long&, const u_long&) const;
        void Use() const;
        void Render() const;
        void Draw() const;

    private:
        uint VAO;
        VertexBuffer VBO;
        ElementBuffer EBO;
        bool usesEBO;
};
