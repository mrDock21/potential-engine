#include "model_importer.hpp"

using namespace CEngine;

ModelImporter::ModelImporter() {

}

void ModelImporter::Import(const std::string& file) {
	Assimp::Importer importer;
	const aiScene* scene =
		importer.ReadFile(file.c_str(), aiProcess_Triangulate/* | aiProcess_FlipUVs */ );

	Logger::Log("ASSIMP => Imports: " + file);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		scene->mRootNode == nullptr) {
		Logger::Log("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
		return;
	}

	clear();
	importData(scene->mRootNode, scene);
	buildMeshes();
}

void ModelImporter::BaseShader(std::shared_ptr<Shader> ptr) {
	// make copy
	baseShader = ptr;
}

void ModelImporter::clear() {
	sceneData.clear();
	models.clear();
}

const std::vector<Actor*>& ModelImporter::Data() {
	return models;
}

void ModelImporter::importData(const aiNode* node, const aiScene* scene) {
	checkNode(node, scene);
}

void ModelImporter::checkNode(const aiNode* node, const aiScene* scene) {

	if (node == nullptr)
		return;

	for (int i(0); i < node->mNumMeshes; i++) {
		processMesh(scene->mMeshes[ node->mMeshes[i] ], scene);
	}

	// check the rest
	for (int i(0); i < node->mNumChildren; i++)
		checkNode(node->mChildren[i], scene);
}

void ModelImporter::processMesh(const aiMesh* meshPtr, const aiScene* scene) {
	MeshData newMesh;
	Vertex vertex;
	uint vIndex;

	aiVector3D pos, normal, uv;

	for (int i(0); i < meshPtr->mNumVertices; i++) {

		pos = meshPtr->mVertices[i];
		normal = meshPtr->mNormals[i];
		uv.x = uv.y = 0;

		if (meshPtr->mTextureCoords[0])
			// only care about basic UV (set 0)
			uv = meshPtr->mTextureCoords[0][i]; 
	
		vertex.Pos(Vector3( pos.x, pos.y, pos.z ));
		vertex.Normal(Vector3(normal.x, normal.y, normal.z));
		vertex.UV(Vector2(uv.x, uv.y));

		newMesh.AddVertex(vertex);
	}

	// now get indices
	for (int i(0); i < meshPtr->mNumFaces; i++) {
		// check each by face...
		for (int j(0), indices(meshPtr->mFaces[i].mNumIndices); j < indices; j++)
			newMesh.AddIndex(meshPtr->mFaces[i].mIndices[j]);
	}

	newMesh.Name(meshPtr->mName.C_Str());
	sceneData.push_back(newMesh);
}

void ModelImporter::buildMeshes() {
	// attribute size for shader: point, normal, UVs
	const unsigned long long componentsSize = sizeof(float) * 8;
	std::vector<float> auxData;
	std::vector<Vertex> *verts;
	std::vector<uint> *indices;
	Vector3 pos, normal;
	Vector2 uvs;
	Mesh* mesh;
	Material *material;

	for (int i(0); i < sceneData.size(); i++) {

		// clear data for each Mesh
		verts = &(sceneData.at(i).Vertices());
		indices = &(sceneData.at(i).Indices());

		auxData.clear();

		for (int j(0); j < verts->size(); j++) {
			pos = verts->at(j).Pos();
			normal = verts->at(j).Normal();
			uvs = verts->at(j).UV();
			// insert raw data
			auxData.insert(auxData.end(), { 
				pos.X(), pos.Y(), pos.Z(),			// position => Attribute 0
				normal.X(), normal.Y(), normal.Z(),	// normal	=> Attribute 1
				uvs.X(), uvs.Y()					// UVs		=> Attribute 2
			});
		}

		// build the Mesh...
		mesh = new Mesh(
			auxData.data(), sizeof(float) * auxData.size(), // vertices
			componentsSize,									// size of verts, indices, UVs
			indices->data(), sizeof(uint) * indices->size() // indices
		);
		
		// position attribute
		mesh->SetAttribute(0, 3, GL_FLOAT, false, componentsSize, 0);
		// normals
		mesh->SetAttribute(1, 3, GL_FLOAT, false, componentsSize, 3 * sizeof(float));
		// uv's attribute
		mesh->SetAttribute(2, 2, GL_FLOAT, false, componentsSize, 6 * sizeof(float));

		material = new Material(baseShader);
		// add material (assimp links only ONE material per mesh)
		mesh->Use();
		material->Use();

		models.push_back( 
			new Actor(sceneData.at(i).Name(), mesh, material)
		);
	}

}
