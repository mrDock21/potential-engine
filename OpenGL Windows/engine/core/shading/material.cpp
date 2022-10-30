#include "material.hpp"

Material::Material() { }

/**
 * Initializes material with given shader src
 * @param vertexSrc Source code of vertex shader
 * @param fragSrc Source code of fragment shader
*/
Material::Material(const std::string& vertexSrc, const std::string& fragSrc) 
    : shader(vertexSrc, fragSrc) { }

void Material::Use() const {
    shader.Use();
}

void Material::DefineProperties(std::string* arr, int count) {
    for (int i(0); i < count; i++)
        AddProperty(arr[i]);
}

void Material::AddProperty(const std::string& prop) {
    properties.push_back(prop);
}

void Material::SetShader(const Shader& shader) {
    this->shader = shader;
}