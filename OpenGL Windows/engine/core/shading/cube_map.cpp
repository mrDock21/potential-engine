#include "cube_map.hpp"

using namespace CEngine;

CubeMap::CubeMap() : cubeMesh(nullptr), skyboxMat(nullptr) {

	glGenTextures(1, &ID);
	Use();

	initMesh();
}

CubeMap::CubeMap(const std::vector<std::string>& imgs) : CubeMap() {

	AssignTextures(imgs);
}

CubeMap::CubeMap(const std::vector<std::string>& imgs, std::shared_ptr<Material> mat) 
	: CubeMap(imgs) {

	skyboxMat = mat;

	// bind all together
	cubeMesh->Use();
	skyboxMat->Use();
	Use();
}

CubeMap::~CubeMap() {
	glDeleteTextures(1, &ID);
}

void CubeMap::Use() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void CubeMap::AssignTextures(const std::vector<std::string>& imgs) {
	int width, height;
	TextureDir dirs[]{ 
		TextureDir::Right, TextureDir::Left, TextureDir::Top, 
		TextureDir::Bottom, TextureDir::Front, TextureDir::Back
	};

	Use();

	for (int i(0); i < imgs.size() && i < MAX_TEXTURES; i++) {

		STBImageLoader loader;

		loader.LoadCubeMap(imgs.at(i), &width, &height);

		if (!loader.HasData()) {
			Logger::Warning("---CubeMap: Could not read tex= " + imgs.at(i));
			continue;
		}

		AssignTexture(loader.Data(), width, height, dirs[i]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// since it uses z for vector dir
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::AssignTexture(const u_char* imgData, int width, int height, TextureDir texDir) {
	uint texSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X);

	switch (texDir)
	{
		case CEngine::CubeMap::TextureDir::Left:
			texSide = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			break;
		case CEngine::CubeMap::TextureDir::Top:
			texSide = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			break;
		case CEngine::CubeMap::TextureDir::Bottom:
			texSide = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			break;
		case CEngine::CubeMap::TextureDir::Back:
			texSide = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			break;
		case CEngine::CubeMap::TextureDir::Front:
			texSide = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			break;
		default:
			break;
	}

	// gen base image
	glTexImage2D(
		texSide,
		0, // 0 means this is the base texture
		GL_RGB,
		width,
		height,
		0,  // legacy stuff (must always be 0)
		GL_RGB,
		GL_UNSIGNED_BYTE,
		imgData
	);
}

void CubeMap::Skybox(std::shared_ptr<Material> mat) {
	skyboxMat = mat;

	// bind all together
	cubeMesh->Use();
	skyboxMat->Use();
}

std::shared_ptr<Material> CubeMap::Skybox() const {
	return skyboxMat;
}

void CubeMap::Render(const Matrix4& view, const Matrix4& proj) {
	
	glDepthFunc(GL_LEQUAL);

	cubeMesh->Use();
	skyboxMat->Use();
	Use();

	cubeMesh->Render();

	// (Global matrices are set by the main scene itself)
	// skyboxMat->SetUniform("projection", proj);
	// skyboxMat->SetUniform("view", view);
	cubeMesh->Draw();

	// return thye func to the def value
	glDepthFunc(GL_LESS);
}

void CubeMap::initMesh() {
	float verts[] = {
		// vertices (already in clip space)
		// front face
		 1.0f,  1.0f, 1, 
		-1.0f,  1.0f, 1,
		 1.0f, -1.0f, 1,
		-1.0f, -1.0f, 1,
		// back face
		 1.0f,  1.0f, -1,
		-1.0f,  1.0f, -1,
		 1.0f, -1.0f, -1,
		-1.0f, -1.0f, -1,
	};
	uint indices[] = {
		// front face
		0, 1, 2,    // triangle 1
		3, 1, 2,     // triangle 2
		// back face
		4, 5, 6,
		7, 5, 6,
		// right face
		0, 4, 6,
		0, 2, 6,
		// left face
		1, 5, 7,
		7, 3, 1,
		// upper face
		0, 1, 5,
		4, 5, 0,
		// bottom face
		2, 3, 7,
		6, 7, 2
	};
	const int componentSize = sizeof(float) * 3, posAttrIndex=0;
	const bool shouldBeNormalized(false);

	cubeMesh = 
		std::make_unique<Mesh>(verts, sizeof(verts), componentSize, indices, sizeof(indices));
	
	cubeMesh->Use();
	cubeMesh->SetAttribute(
		posAttrIndex, 3, GL_FLOAT, shouldBeNormalized, componentSize, 0
	);
}
