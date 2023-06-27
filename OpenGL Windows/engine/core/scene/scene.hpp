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
		// same as shader
		const int MAX_LIGHTS = 100;
		const std::string GLOBAL_MATRICES = "GlobalMatrices";

		Scene();
		~Scene();

		void AddActor(std::shared_ptr<Actor>);
		void RemoveActor(Actor*);
		void RemoveActor(int);

		/// <summary>
		///		Initializes the uniform buffer for camera (matrix), and projection (matrix)
		/// </summary>
		/// <param name="">	Uniform block name					</param>
		/// <param name="">	Uniform block binding index			</param>
		/// <param name="">	Total size (bytes) of the buffer	</param>
		void InitializeGlobalData(const std::string&, const uint&, const u_long&);

		/// <summary>
		///		Renders the scene
		/// </summary>
		/// <param name="">	Window's aspect ratio	</param>
		void Render(const Matrix4&, const Matrix4&);

	private:

		struct SceneActor {
			uint ID;
			std::shared_ptr<Actor> Obj;
		};

		uint innerCount;
		std::vector<SceneActor> actors;
		DirectionalLight sun;

		UBO uniformBuffer;
	};
}
