#pragma once

#include <memory>

#include "object.hpp"
#include "geometry/mesh.hpp"

#include "math_types/matrix4.hpp"

namespace CEngine {

	/**
	* Represents a 3D actor in the scene (with mesh)
	*/
	class Actor : public Object {
	public:
		Actor();
		Actor(Mesh*, Material*);
		Actor(const std::string&);
		Actor(const std::string&, Mesh*, Material*);

		void SetMesh(Mesh*);
		void SetMaterial(Material*);

		virtual void Initialize(const UBO& matricesUBO, const UBO& lightsUBO);
		/// <summary>
		///		Renders and draws the entire object. 
		///		If shader properties are needed: use BeginRender() instead
		/// </summary>
		/// <param name="">	View matrix			</param>
		/// <param name="">	Projection matrix	</param>
		virtual void Render();

		/// <summary>
		///		Binds OpenGL object (gives room to set shader properties).
		///		Must call EndRender()
		/// </summary>
		/// <param name=""> View matrix			</param>
		/// <param name="">	Projection matrix	</param>
		void BeginRender();
		/// <summary>
		///		Ends render and draws the object to the screen
		/// </summary>
		void EndRender();

		Mesh* mesh() const;
		Material* material() const;

	private:
		std::unique_ptr<Mesh> meshPtr;
		std::unique_ptr<Material> materialPtr;
	};
}
