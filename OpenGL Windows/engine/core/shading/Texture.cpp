#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "./../../../vendor/stb_image.h"

Texture::Texture() { }

Texture::Texture(const std::string& imgName, const TexFormat& format, const PixelFormat& pxlFormat) {
    Init(imgName, format, pxlFormat);
}

Texture::Texture(const std::string& imgName, const ImgType& imgType) {
    switch (imgType)
    {
    case ImgType::JPEG: Init(imgName, TexFormat::RGB, PixelFormat::RGB); break;
    case ImgType::PNG: Init(imgName, TexFormat::RGBA, PixelFormat::RGBA); break;
    default:
        Logger::Warning("Image type for [" + imgName + "] is not yet initialized...");
        break;
    }
}

void Texture::Use() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

/// <summary>
///     Will read the texture (image) file
/// </summary>
void Texture::Init(const std::string& imgName, const TexFormat& format, const PixelFormat& pxlFormat) {
    int numChannels;
    u_char* data;

    stbi_set_flip_vertically_on_load(true);
    // read image
    data = stbi_load(imgName.c_str(), &width, &height, &numChannels, 0);
    
    if (data) {
        this->pixelFormat = pxlFormat;
        this->format = format;
        GenerateTexture(data);
        
        stbi_image_free(data);
    }
    else
        std::cout << "TEXTURE::ERROR::Couldn't read texture: " << imgName << std::endl;

}

/// <summary>
///     Creates the texture internally in OpenGL
/// </summary>
void Texture::GenerateTexture(const u_char* data) {
    glGenTextures(1, &ID);
    Use();
    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // gen base image
    glTexImage2D (
        GL_TEXTURE_2D, 
        0, // 0 means this is the base texture
        GetGLFormat(format), 
        width, 
        height,
        0,  // legacy stuff (must always be 0)
        GetGLPixelFormat(this->pixelFormat),
        GL_UNSIGNED_BYTE, 
        data
    );
    // mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
}

uint Texture::GetGLFormat(const TexFormat& format) {
    switch (format)
    {
    case TexFormat::R:
        return GL_RED;
    case TexFormat::RG:
        return GL_RG;
    case TexFormat::RGB:
        return GL_RGB;
    case TexFormat::RGBA:
        return GL_RGBA;
    default:
        break;
    }
    return 0;
}

uint Texture::GetGLPixelFormat(const PixelFormat& format) {
    switch (format)
    {
        case PixelFormat::R:
            return GL_RED;
        case PixelFormat::RG:
            return GL_RG;
        case PixelFormat::RGB:
            return GL_RGB;
        case PixelFormat::BGR:
            return GL_BGR;
        case PixelFormat::RGBA:
            return GL_RGBA;
        case PixelFormat::BGRA:
            return GL_BGRA;
        case PixelFormat::R_INT:
            return GL_RED_INTEGER;
        case PixelFormat::RG_INT:
            return GL_RG_INTEGER;
        case PixelFormat::RGB_INT:
            return GL_RGB_INTEGER;
        case PixelFormat::BGR_INT:
            return GL_BGR_INTEGER;
        case PixelFormat::RGBA_INT:
            return GL_RGBA_INTEGER;
        case PixelFormat::BRGA_INT:
            return GL_BGRA_INTEGER;
        case PixelFormat::STENCIL:
            return GL_STENCIL_INDEX;
        default:
            break;
    }
    return 0;
}