#pragma once

#include <vector>

#include "types/types.hpp"

#include "actor.hpp"
#include "directionallight.hpp"
#include "pointlight.hpp"

#include "controllers/camera.hpp"

#include "shading/uniform_buffer.hpp"

namespace CEngine {

	class Scene {
	public:
		// same as shaders
		const int MAX_LIGHTS = 10;
		const int GMATRICES_UBO_INDEX = 0, GLIGHTS_UBO_INDEX = 1;
		const std::string GMATRICES_UBO_NAME = "GlobalMatrices", 
						  GLIGHTS_UBO_NAME = "GlobalLights";

		Scene();
		~Scene();

		void AddObject(std::shared_ptr<Object>);
		void RemoveActor(Actor*);
		void RemoveActor(int);

		/// <summary>
		///		Initializes the uniform buffer for camera (matrix), and projection (matrix)
		/// </summary>
		void InitializeGlobalData();

		/// <summary>
		///		Renders the scene
		/// </summary>
		/// <param name="">	Window's aspect ratio	</param>
		void Render(const Matrix4&, const Matrix4&);

	private:

		struct SceneObject {
			uint ID;
			std::shared_ptr<Object> Obj;
		};

		uint innerCount;
		std::vector<SceneObject> actors;

		UBO globalMatricesBuffer, globalLightsBuffer;
	};
}
