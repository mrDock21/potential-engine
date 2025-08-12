#pragma once
#include <iostream>
#include <glad/glad.h>

#include "types/types.hpp"

#include "stb_img_loader.hpp"

namespace CEngine {


    class Texture {
        public:
            enum class TexFormat { R, RG, RGB, RGBA };
            enum class PixelFormat { 
                R, RG, RGB, BGR, RGBA, BGRA, 
                R_INT, RG_INT, RGB_INT, BGR_INT, 
                RGBA_INT, BRGA_INT, STENCIL 
            };
            enum class ImgType { JPEG, PNG };

            Texture();
            // Create and read texture with specific formats
            Texture(const std::string&, const TexFormat&, const PixelFormat&);
            // Create and read texture by specific image type
            Texture(const std::string&, const ImgType&);
            // Create an EMPTY texture with specific image type
            Texture(int, int, const TexFormat&, const PixelFormat&);

            ~Texture();

            void Use() const;

            friend class FrameBuffer;

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
}
