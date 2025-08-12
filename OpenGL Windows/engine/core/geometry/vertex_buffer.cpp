#include "vertex_buffer.hpp"

using namespace CEngine;

uint CEngine::getGlBufferUsage(const BufferUsage& usage) {
    switch (usage)
    {
        case BufferUsage::Static:
            return GL_STATIC_DRAW;
        case BufferUsage::Dynamic:
            return GL_DYNAMIC_DRAW;
    }
    return 0;
}

VertexBuffer::VertexBuffer() : elementSize(0), bytesSize(0), ID(0) { 
    // vertex buffer
    glGenBuffers(1, &ID);
}

VertexBuffer::~VertexBuffer() {

    glDeleteBuffers(1, &ID);
}

/**
 * Constructs a new vertex buffer object
 * @param verts Vertices of this vertex buffer
 * @param size byte size of "verts"
 * @param eSize Bytes size of each single index of data (vertex + normal + uv's...)
 * @param glUsage OpenGL draw mode constant
 */
VertexBuffer::VertexBuffer(
        float verts[], const u_long& size, const u_long& eSize, const BufferUsage& glUsage
    )
    : elementSize(eSize), bytesSize(size) {
    // vertex buffer
    glGenBuffers(1, &ID);
    Use();
    glBufferData(GL_ARRAY_BUFFER, size, verts, CEngine::getGlBufferUsage(glUsage));
}

/**
 * Binds this VertexBuffer
*/
void VertexBuffer::Use() const {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

/**
 * @returns number of elements within its array data
*/
u_long VertexBuffer::GetArrayCount() const {
    return (elementSize != 0) ? bytesSize / elementSize : 0;
}

/**
 * @returns Size in bytes of inner array data
*/
u_long VertexBuffer::GetBytesSize() const {
    return bytesSize;
}