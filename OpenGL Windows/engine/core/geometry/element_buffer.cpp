#include "element_buffer.hpp"

ElementBuffer::ElementBuffer() : arraySize(0), bytesSize(0) {

}

/**
 * Constructs a new vertex buffer object
 * @param indices Vertices of this vertex buffer
 * @param size byte size of "indices"
 * @param glUsage OpenGL draw mode constant
 */
ElementBuffer::ElementBuffer(uint* indices, const u_long& size, const BufferUsage& glUsage) 
    : arraySize(size / sizeof(uint)), bytesSize(size) {
    // vertex buffer
    glGenBuffers(1, &ID);
    Use();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, getGlBufferUsage(glUsage));
}

/**
 * Binds this ElementBuffer
*/
void ElementBuffer::Use() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

/**
 * @returns number of elements within its array data
*/
u_long ElementBuffer::GetArraySize() const {
    return arraySize;
}

/**
 * @returns Size in bytes of inner array data
*/
u_long ElementBuffer::GetBytesSize() const {
    return bytesSize;
}