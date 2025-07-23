#include "Mesh.hpp"

using namespace CEngine;

Mesh::Mesh() { }

/**
 * Constructs mesh with vertex buffer only
 * @param verts Pointer to data array
 * @param vertsSize Size in bytes of data array
 * @param esize Size in bytes of all attributes contained in data (vertex, normal, uv...)
*/
Mesh::Mesh(float* verts, const u_long& vertsSize, const u_long& esize) : usesEBO(false) {
    // vertex array object
    glGenVertexArrays(1, &VAO);
    Use();
    // vertex buffer
    VBO = VertexBuffer(verts, vertsSize, esize, BufferUsage::Static);
    VBO.Use();
}

/**
 * Constructs mesh with vertex buffer and index buffer
 * @param verts Pointer to data array
 * @param vertsSize Size in bytes of data array
 * @param esize Size in bytes of all attributes contained in data (vertex, normal, uv...)
 * @param indices Pointer to indices array
 * @param indicesSize Size in bytes of indices array
*/
Mesh::Mesh(
    float* verts, 
    const u_long& vertsSize, 
    const u_long& esize, 
    uint* indices, 
    const u_long& indicesSize
) : usesEBO(true) {
    // vertex array object
    glGenVertexArrays(1, &VAO);
    Use();
    // vertex buffer
    VBO = VertexBuffer(verts, vertsSize, esize, BufferUsage::Static);
    VBO.Use();
    // elements (index) buffer
    // element array object -> Gets binded to current VAO
    EBO = ElementBuffer(indices, indicesSize, BufferUsage::Static);
    EBO.Use();
}

/**
 * Configures and enables given attribute's index
 * @param index Zero-based attribute index to enabled
 * @param componentCount Number of elements that forme a single entry of the attribute
 * @param glDataType Generic number type of OpenGL
 * @param normalized Wether the data is normalized or not
 * @param stride Bytes separation between each component
 * @param startOffset Initial offset (in bytes) of where the firs entry of component is found 
*/
void Mesh::SetAttribute(
    const int& index, 
    const int& componentCount, 
    const uint& glDataType, 
    const bool& normalized, 
    const u_long& stride, 
    const u_long& startOffset) const {
    
    // make sure VAO is binded
    Use();
    // attribute
    glVertexAttribPointer(index, componentCount, glDataType, normalized, stride, (void*) startOffset);
    // enable
    glEnableVertexAttribArray(index);
}

void Mesh::Use() const {
    glBindVertexArray(VAO);
}

void Mesh::Render() const {
    Use();
}

void Mesh::Draw() const  {
    if (usesEBO)
        glDrawElements(GL_TRIANGLES, EBO.GetArraySize(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, VBO.GetArrayCount());

    // unbind whatever the shader enabled
    glDisable(GL_BLEND | GL_STENCIL);
}
