#include "material.hpp"

Material::Material() : linkedToScene(false) { }

/**
 * Initializes material with given shader src
 * @param vertexSrc Source code of vertex shader
 * @param fragSrc Source code of fragment shader
*/
Material::Material(const std::string& vertexSrc, const std::string& fragSrc) 
    : shader(vertexSrc, fragSrc), linkedToScene(false) { }

void Material::Use() const {
    shader.Use();
}

void Material::SetShader(const Shader& shader) {
    this->shader = shader;
}

void Material::SetUniformBlock(const UBO& ubo) {

    if (linkedToScene)
        return;

    shader.SetUniformBlock(ubo.UniformBlockName(), ubo.BindingIndex());
    linkedToScene = true;
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

inline bool Material::LinkedToScene() const {
    return linkedToScene;
}
