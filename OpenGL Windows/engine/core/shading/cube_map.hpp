#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>

#include "material.hpp"

#include "types/types.hpp"
#include "math_types/matrix4.hpp"

#include "geometry/mesh.hpp"

#include "stb_img_loader.hpp"

#include "Logger.hpp"

namespace CEngine {

	class CubeMap {
	public:
		enum class TextureDir{ Right = 0, Left, Top, Bottom, Back, Front };

		CubeMap();
		CubeMap(const std::vector<std::string>&);
		CubeMap(const std::vector<std::string>&, std::shared_ptr<Material>);
		~CubeMap();

		void Use() const;
		// assigns 6 textures for the sky box. Assumes correct order: RLTBBF
		void AssignTextures(const std::vector<std::string>&);
		// assigns 6 textures for the sky box. Assumes correct order: RLTBBF, and that class is binded
		void AssignTexture(const u_char*, int, int, TextureDir);
		void Skybox(std::shared_ptr<Material>);

		std::shared_ptr<Material> Skybox () const;

		void Render(const Matrix4&, const Matrix4&);

	private:

		void initMesh();

		uint ID;
		std::shared_ptr<Material> skyboxMat;
		std::unique_ptr<Mesh> cubeMesh;

		static constexpr int MAX_TEXTURES = 6;
	};
} 