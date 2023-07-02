#include "uniform_buffer.hpp"

using namespace CEngine;

UBO::UBO (
	const std::string& name, 
	const uint& bindingPoint, 
	const u_long& offset, 
	const u_long& esize
	) : initialized(true), uniformBlockName(name)
{
	bindingIndex = bindingPoint;
	
	glGenBuffers(1, &id);

	Use();
	// allocate with size of buffer
	glBufferData(GL_UNIFORM_BUFFER, esize, NULL, GL_STATIC_DRAW);
	// unbind to previus state (before bind from above)
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// func(command, bindingPoint, bufferID, offset, size)
	// This will bind the amount given in "size" to the location given in "bindingIndex"
	//	for the buffer "bufferID"
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, id, offset, esize);
}

UBO::UBO() : initialized(false) {

}

void UBO::Use() {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
}

void UBO::Upload(const Matrix4& m, const unsigned long long& offset) {
	Use();
	// func(command, offset, size, ptr)
	// Will upload the data in "M" to the uniform block buffer data
	glBufferSubData(GL_UNIFORM_BUFFER, offset, Matrix4::Size(), m.ValuePtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint UBO::ID() const {
	return id;
}

uint UBO::BindingIndex() const {
	return bindingIndex;
}

std::string UBO::UniformBlockName() const {
	return uniformBlockName;
}