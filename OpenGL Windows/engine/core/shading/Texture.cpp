#include "Texture.hpp"

using namespace CEngine;

Texture::Texture() { }

Texture::Texture(const std::string& imgName, const TexFormat& format, const PixelFormat& pxlFormat) {
    Init(imgName, format, pxlFormat);
}

Texture::Texture(const std::string& imgName, const ImgType& imgType) 
    : Texture(imgName, imgType, TexColorSpace::LINEAR) {

}

Texture::Texture(const std::string& name, const ImgType& imgType, const TexColorSpace& cSpace) {
    
    PixelFormat importedPixelFormat = PixelFormat::RGB;
    TexFormat importedFormat = 
        (cSpace == TexColorSpace::LINEAR) ? TexFormat::RGB : TexFormat::sRGB;

    if (imgType == ImgType::PNG) {
        importedFormat = (cSpace == TexColorSpace::LINEAR) ? TexFormat::RGBA : TexFormat::sRGBA;
        importedPixelFormat = PixelFormat::RGBA;
    }
    
    switch (imgType)
    {
    case ImgType::JPEG: break;
    case ImgType::PNG: break;
    default:
        Logger::Warning("Image type for [" + name + "] is not yet initialized...");
        break;
    }

    Init(name, importedFormat, importedPixelFormat);

}

Texture::Texture(int w, int h, const TexFormat& format, const PixelFormat& pxlFormat) {
    
    const u_char* emptyImg = nullptr;
    
    this->pixelFormat = pxlFormat;
    this->format = format;
    width = w;
    height = h;

    GenerateTexture(emptyImg);

}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::Use() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

/// <summary>
///     Will read the texture (image) file
/// </summary>
void Texture::Init(const std::string& imgName, const TexFormat& format, const PixelFormat& pxlFormat) {
    
    STBImageLoader imgLoader;

    imgLoader.LoadTexture(imgName, &width, &height);
    
    if (imgLoader.Data() == nullptr)
        Logger::Warning("---TEXTURE::ERROR::Cancelled: ");

    this->pixelFormat = pxlFormat;
    this->format = format;
    GenerateTexture( imgLoader.Data() );
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
    case TexFormat::sRGB:
        return GL_SRGB;
    case TexFormat::sRGBA:
        return GL_SRGB_ALPHA;
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