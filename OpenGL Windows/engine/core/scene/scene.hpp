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

		const unsigned long long LIGHTS_ARRAY_SIZE = Vector4::Size() * 2 * MAX_LIGHTS,
								 SUN_STRUCT_SIZE = Vector4::Size() * 3;

		Scene();
		~Scene();

		void AddPointLight(std::shared_ptr<PointLight>);
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

		void UpdateLights();
		void UpdateSun();

		struct SceneObject {
			uint ID;
			std::shared_ptr<Object> Obj;
		};

		uint innerCount, lightCount;
		std::vector<SceneObject> actors;

		Light* lights[10];
		DirectionalLight sun;

		UBO globalMatricesBuffer, globalLightsBuffer;
	};
}
