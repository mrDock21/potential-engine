#pragma once

#include <memory>

#include "object.hpp"
#include "mesh.hpp"

#include "matrix4.hpp"

/**
* Represents a 3D actor in the scene (with mesh)
*/
class Actor : public Object {
public:
	Actor();
	Actor(Mesh*, Material*);
	Actor(const std::string&, Mesh*, Material*);

	void Render(const Matrix4&, const Matrix4&);


private:
	std::unique_ptr<Mesh> mesh;
	std::unique_ptr<Material> material;
};
