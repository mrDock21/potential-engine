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
        void SetMaterial(const Material&);
        void SetTexture(const Texture&);
        void SetAttribute(const int&, const int&, const uint&, const bool&, const u_long&, const u_long&) const;
        void Use() const;
        void Render() const;
        void Draw() const;

        template<class T>
        void SetUniform(const std::string&, const T&);

        const Components::Transform& Transform();
    private:
        uint VAO;
        Components::Transform transform;
        Material material;
        std::vector<Texture> textures;
        VertexBuffer VBO;
        ElementBuffer EBO;
        bool usesEBO;
};

/**
 * Sets uniform in inner material
*/
template<class T>
void Mesh::SetUniform(const std::string& uniform, const T& value) {
    material.SetUniform<T>(uniform, value);
}