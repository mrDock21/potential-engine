
#include "../core/geometry/Mesh.hpp"
#include "../core/scene/actor.hpp"
#include "../core/shading/material.hpp"


namespace CEngine::Primitives {
	
    class Quad : public CEngine::Actor {
	public:
		Quad(const std::string& name, CEngine::Material* mat) { 

            build(name, mat);
        }

	private:
		void build(const std::string& name, CEngine::Material* mat) {
            float rect[] = {
                // vertices       //normals      // uv's
                // front face
                0.5f, 0.5f, 0,    0, 0, 1.0f,    1.0f, 1.0f,  // triangle
                -0.5f, 0.5f, 0,   0, 0, 1.0f,    0.0f, 1.0f,
                0.5f, -0.5f, 0,   0, 0, 1.0f,    1.0f, 0.0f,
                -0.5f, -0.5f, 0,  0, 0, 1.0f,    0.0f, 0.0f,  // triangle
                -0.5, 0.5f, 0,    0, 0, 1.0f,    0.0f, 1.0f,
                0.5f, -0.5f, 0,   0, 0, 1.0f,    1.0f, 0.0f
            };
            const u_long componentsSize = 8 * sizeof(float);
            CEngine::Mesh* mesh;

            mesh = new CEngine::Mesh(rect, sizeof(rect), componentsSize);

            // link the data
            this->Name = name;
            SetMesh(mesh);
            SetMaterial(mat);

            mesh->Use();
            // position attribute
            mesh->SetAttribute(0, 3, GL_FLOAT, false, componentsSize, 0);
            // normals
            mesh->SetAttribute(1, 3, GL_FLOAT, false, componentsSize, 3 * sizeof(float));
            // uv's attribute
            mesh->SetAttribute(2, 2, GL_FLOAT, false, componentsSize, 6 * sizeof(float));
            
		}
	};

}
