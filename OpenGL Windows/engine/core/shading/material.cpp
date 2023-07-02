#include "material.hpp"

using namespace CEngine;

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

void Material::SetShader(const Shader& shader) {
    this->shader = shader;
}

void Material::SetUniformBlock(const UBO& ubo) {
    bool alreadyLinked = false;

    // make sure we are not linking it twice...
    for (int i(0); i < linkedUniformBlocks.size() && !alreadyLinked; i++) {
        alreadyLinked = linkedUniformBlocks.at(i) == ubo.ID();
    }

    if (alreadyLinked)
        return;

    // link the uniform to the shader program
    shader.SetUniformBlock(ubo.UniformBlockName(), ubo.BindingIndex());
    linkedUniformBlocks.push_back(ubo.ID());
}

void Material::SetTexture(const Texture& t) {
    textures.push_back(t);
}

void Material::Render() const {
    Use();
    // bind each texture-set
    for (int i(0); i < textures.size(); i++) {
        // first: indicate set and then bind
        glActiveTexture(GL_TEXTURE0 + i);
        textures.at(i).Use();
    }
}

void Material::SetUniform(const std::string& name, const int& value) {
    shader.SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const float& value) {
    shader.SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Vector3& value) {
    shader.SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Vector4& value) {
    shader.SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Matrix4& value) {
    shader.SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Color& value) {
    shader.SetUniform(name, value);
}

