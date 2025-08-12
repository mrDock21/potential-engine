#include "stb_img_loader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "./../../../vendor/stb_image.h"

using namespace CEngine;

STBImageLoader::STBImageLoader() : data(nullptr) {

}

STBImageLoader::STBImageLoader(const std::string& imgName, int* const width, int* const height)
    : data(nullptr) {

    Load(imgName, width, height);
}

STBImageLoader::~STBImageLoader() {
	freeData();
}

void STBImageLoader::LoadTexture(const std::string& imgName, int* const width, int* const height) {

    stbi_set_flip_vertically_on_load(true);
    Load(imgName, width, height);
}

void STBImageLoader::LoadCubeMap(const std::string& imgName, int* const width, int* const height) {

    stbi_set_flip_vertically_on_load(false);
    Load(imgName, width, height);
}

void STBImageLoader::Load(const std::string& imgName, int* const width, int* const height) {
    int numChannels;

    // if object re-used
    freeData();
    
    // read image
    data = stbi_load(imgName.c_str(), width, height, &numChannels, 0);

    if (data == nullptr) {
        Logger::Warning("---- STBI ImageLoader: Could not load= " + imgName);
        data = nullptr;
    }
}

u_char* STBImageLoader::Data() const {
    return data;
}

bool STBImageLoader::HasData() const {
    return data != nullptr;
}

void STBImageLoader::freeData() {
	if (data != nullptr)
		stbi_image_free(data);

	data = nullptr;
}