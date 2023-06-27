#pragma once

#include <iostream>

#include <glad/glad.h>

#include "types/types.hpp"

namespace CEngine {

    enum class BufferUsage { 
        Static, Dynamic 
    };

    uint getGlBufferUsage(const BufferUsage&);

    class VertexBuffer {
        public:
            VertexBuffer();
            VertexBuffer(float[], const u_long&, const u_long&, const BufferUsage&);
            void Use() const;
            u_long GetArrayCount() const;
            u_long GetBytesSize() const;
        private:
            uint ID;
            u_long elementSize, bytesSize;
    };
}
