#pragma once

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh_data.hpp"
#include "scene/actor.hpp"

#include "./../utils/Logger.hpp"

namespace CEngine {

	class ModelImporter {
	public:
		ModelImporter();

		void Import(const std::string&);
		void BaseShader(std::shared_ptr<Shader>);

		const std::vector<Actor*>& Data();

	private:

		void clear();

		void importData(const aiNode*, const aiScene*);
		void checkNode(const aiNode*, const aiScene*);

		void processMesh(const aiMesh*, const aiScene*);

		void buildMeshes();

		std::shared_ptr<Shader> baseShader;

		std::vector<MeshData> sceneData;
		// make sure to pass ownership to some Scene object, to avoid memory leaks...
		std::vector<Actor*> models;		
	};
}