#pragma once
#include <iostream>
#include <glad/glad.h>

#include "types/types.hpp"

class Texture {
    public:
        enum class TexFormat { R, RG, RGB, RGBA };
        enum class PixelFormat { 
            R, RG, RGB, BGR, RGBA, BGRA, 
            R_INT, RG_INT, RGB_INT, BGR_INT, 
            RGBA_INT, BRGA_INT, STENCIL 
        };

        Texture();
        Texture(const std::string&, const TexFormat&, const PixelFormat&);
        void Use() const;
    private:
        uint ID;
        int width, height;
        TexFormat format;
        PixelFormat pixelFormat;

        void Init(const std::string&, const TexFormat&, const PixelFormat&);
        void GenerateTexture(const u_char*);

        static uint GetGLFormat(const TexFormat&);
        static uint GetGLPixelFormat(const PixelFormat&);
};