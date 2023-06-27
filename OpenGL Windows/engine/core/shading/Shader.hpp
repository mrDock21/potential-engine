#pragma once

#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "math_types/vector3.hpp"
#include "math_types/vector4.hpp"
#include "math_types/matrix4.hpp"
#include "types/types.hpp"

#include "types/color.hpp"

namespace CEngine {

    class Shader {
    public:
        Shader();
        Shader(const std::string&, const std::string&);
        void Use() const;

        void SetUniformBlock(const std::string&, const uint&);

        void SetUniform(const std::string&, const float&);
        void SetUniform(const std::string&, const int&);

        void SetUniform(const std::string&, const Vector3&);
        void SetUniform(const std::string&, const Vector4&);
        void SetUniform(const std::string&, const Matrix4&);
        void SetUniform(const std::string&, const Color&);

        void SetUniform(const std::string&, const glm::vec2&);
        void SetUniform(const std::string&, const glm::vec3&);
        void SetUniform(const std::string&, const glm::vec4&);
        void SetUniform(const std::string&, const glm::mat4x4&);
    private:
        uint CompileShader(const std::string&, uint) const;
        bool OperationSucceded() const;
        bool OperationSucceded(uint) const;

        int GetUniformLocation(const std::string&);

        uint _ProgramID;
        std::map<std::string, int> uniforms;
    };
}

