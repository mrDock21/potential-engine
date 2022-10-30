#include "Shader.hpp"

Shader::Shader() : _ProgramID(0) { }

Shader::Shader(const std::string& vertexSrc, const std::string& fragSrc) {
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
    
    return glGetUniformLocation(_ProgramID, uniform.c_str());
    int location = uniforms[uniform];

    if (uniforms.count(uniform) <= 0) {
        location = glGetUniformLocation(_ProgramID, uniform.c_str());
        uniforms[uniform] = location;
    }
    return location;
}