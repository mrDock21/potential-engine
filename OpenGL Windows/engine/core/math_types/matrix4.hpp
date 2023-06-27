#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "math_types/vector3.hpp"
#include "math_types/vector4.hpp"
#include "math_types/quaternion.hpp"

namespace CEngine {

    /**
     * Represents a (4x4) matrix
    */
    class Matrix4 {

        // to be able to set uniforms in shaders
        friend class Shader;

        public:
            Matrix4();
            Matrix4(const Matrix4&);
            Matrix4(const glm::mat4&);

            inline const float* ValuePtr() const { return glm::value_ptr(matrix); }
            inline static unsigned long long Size() { return sizeof(glm::mat4); }

            const Matrix4& operator = (const Matrix4&);
            const Matrix4& operator *= (const Matrix4&);
            friend Matrix4 operator * (Matrix4, const Matrix4&);
        
            static Vector4 Multiply(const Matrix4&, const Vector4&);
            static Matrix4 Indentity();
            static Matrix4 FromQuaternion(const Quaternion&);
            static Matrix4 Translate(const Matrix4&, const Vector3&);
            static Matrix4 Scale(const Matrix4&, const Vector3&);
            static Matrix4 Rotate(const Matrix4&, const float&, const Vector3&);
            static Matrix4 LookAt(const Vector3&, const Vector3&, const Vector3&);
            static Matrix4 Perspective(const float&, const float&, const float&, const float&);
        
        private:
            glm::mat4 matrix;
    };
}
