#include "Shader.hpp"

using namespace CEngine;

Shader::Shader() : _ProgramID(0), _type(Type::Opaque) { }

Shader::Shader(const std::string& vertexSrc, const std::string& fragSrc) 
    : _type(Type::Opaque) {

    uint vertexShader = CompileShader(vertexSrc, GL_VERTEX_SHADER),
         fragmentShader = CompileShader(fragSrc, GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0) {
        std::cout << "[SHADER-COMPILE-ERROR] Couldn't compile shader source!" << std::endl;
        _ProgramID = 0;
        return;
    }
    _ProgramID = glCreateProgram();

    glAttachShader(_ProgramID, vertexShader);
    glAttachShader(_ProgramID, fragmentShader);
    glLinkProgram(_ProgramID);

    if (!OperationSucceded())
        std::cout << "[SHADER-CREATION-ERROR] Couldn't link program!" << std::endl;
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() const {
    glUseProgram(_ProgramID);
}

void Shader::ShaderType(Shader::Type type) {
    _type = type;
}

void Shader::SetUniformBlock(const std::string& blockName, const uint& bindingIndex) {
    uint uniformBlock = glGetUniformBlockIndex(_ProgramID, blockName.c_str());
    glUniformBlockBinding(_ProgramID, uniformBlock, bindingIndex);
}

void Shader::SetUniform(const std::string& uniform, const float& value) {
    Use();
    glUniform1f(GetUniformLocation(uniform), value);
}

void Shader::SetUniform(const std::string& uniform, const int& value) {
    Use();
    glUniform1i(GetUniformLocation(uniform), value);
}

void Shader::SetUniform(const std::string& uniform, const Vector3& v) {
    // this is possible because Shader is a FRIEND of Vector3
    SetUniform(uniform, v.vec);
}

void Shader::SetUniform(const std::string& uniform, const Vector4& v) {
    // this is possible because Shader is a FRIEND of Vector4
    SetUniform(uniform, v.vec);
}

void Shader::SetUniform(const std::string& uniform, const Matrix4& m) {
    // this is possible because Shader is a FRIEND of Matrix4
    SetUniform(uniform, m.matrix);
}

void Shader::SetUniform(const std::string& uniform, const Color& color) {
    SetUniform(uniform, color.color.vec);
}

void Shader::SetUniform(const std::string& uniform, const glm::vec2& v) {
    Use();
    glUniform2f(GetUniformLocation(uniform), v.x, v.y);
}

void Shader::SetUniform(const std::string& uniform, const glm::vec3& v) {
    Use();
    glUniform3f(GetUniformLocation(uniform), v.x, v.y, v.z);
}

void Shader::SetUniform(const std::string& uniform, const glm::vec4& v) {
    Use();
    glUniform4f(GetUniformLocation(uniform), v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(const std::string& uniform, const glm::mat4x4& mat) {
    Use();
    glUniformMatrix4fv(
        GetUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(mat)
    );
}

/// <summary>
///     Enables blending, depth, and stencil flags if needed
/// </summary>
void Shader::BindShaderFlags() const {
    if (_type == Shader::Type::Transparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
    }
    // ToDo: immplement depth and stencil
}

uint Shader::CompileShader(const std::string& shaderSrc, uint shaderType) const {
    uint id = glCreateShader(shaderType);
    const char* src = shaderSrc.c_str();

    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    if (!OperationSucceded(id))
        return 0;
    return id;
}

bool Shader::OperationSucceded() const {
    char infolog[512];
    int success;

    glGetShaderiv(_ProgramID, GL_LINK_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(_ProgramID, sizeof(infolog), NULL, infolog);
        std::cout << "[SHADER-PROGRAM-ERROR] " << infolog << std::endl;
        return false;
    }
    return true;
}

bool Shader::OperationSucceded(uint shaderID) const {
    char infolog[512];
    int success;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shaderID, sizeof(infolog), NULL, infolog);
        std::cout << "[SHADER-COMPILATION-ERROR] " << infolog << std::endl;
        return false;
    }
    return true;
}

int Shader::GetUniformLocation(const std::string& uniform) {
    
    if (!uniforms.contains(uniform)) {
        // ask OpenGL for the uniform in the shader...
        uniforms[uniform] = 
            glGetUniformLocation(_ProgramID, uniform.c_str());
    }

    return uniforms[uniform];
}