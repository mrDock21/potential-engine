#include "material.hpp"

using namespace CEngine;

Material::Material() { }

Material::Material(std::shared_ptr<Shader> ptr) {
    // assuming another material is using the same shader...
    shader = ptr;
}

/**
 * Initializes material with given shader src
 * @param vertexSrc Source code of vertex shader
 * @param fragSrc Source code of fragment shader
*/
Material::Material(const std::string& vertexSrc, const std::string& fragSrc) {
    // assuming this is the first time the shader is created...
    shader = std::make_shared<Shader>(vertexSrc, fragSrc);
}

void Material::Use() const {
    shader->Use();
}

void Material::SetShader(std::shared_ptr<Shader> shaderPtr) {
    shader = shaderPtr;
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
    shader->SetUniformBlock(ubo.UniformBlockName(), ubo.BindingIndex());
    linkedUniformBlocks.push_back(ubo.ID());
}

void Material::AddTexture(std::shared_ptr<Texture> tex, const std::string& textureNameInShader) {
    int textureIndex = textures.size();
    // to bind with current VAO and material
    Use();
    // bind image with texture set 0 + N
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    tex->Use();
    // connect the texture set and the uniform in the shader 
    SetUniform(textureNameInShader, textureIndex);
    textures.push_back(tex);
}

void Material::Render() const {
    
    Use();
    shader->BindShaderFlags();

    // bind each texture-set
    for (int i(0); i < textures.size(); i++) {
        // first: indicate set and then bind
        glActiveTexture(GL_TEXTURE0 + i);
        textures.at(i)->Use();
    }
}

void Material::SetUniform(const std::string& name, const int& value) {
    shader->SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const float& value) {
    shader->SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Vector3& value) {
    shader->SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Vector4& value) {
    shader->SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Matrix4& value) {
    shader->SetUniform(name, value);
}

void Material::SetUniform(const std::string& name, const Color& value) {
    shader->SetUniform(name, value);
}

std::shared_ptr<Shader> Material::InnerShader() const {
    return shader;
}
