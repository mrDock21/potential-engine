
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <filesystem>


#include "ShaderFile.hpp"

namespace CEngine {

    namespace Random {
        /// <summary>
        ///     Returns random number between 0 - 1
        /// </summary>
        float Get() {
            int r = rand() % 10;
            return (float)r / 10.0f;
        }
    }

    namespace Assets {
        inline std::string GetRootFolder(const std::string& asset) {
            return "./../../../../" + asset;
        }

        inline std::string GetShaderPath(const std::string& asset) {
            return "./../../../../Shaders/" + asset;
        }

        inline std::string GetAssetPath(const std::string& asset) {
            return "./../../../../assets/" + asset;
        }

        std::string GetShaderCode(std::string shaderFilename) {
            std::string assetPath = GetShaderPath(shaderFilename);
            ShaderFile s(assetPath);

            return s.Code();
        }

        inline std::string GetConfigFilePath(const std::string& configFilename) {
            return "./../../../../OpenGL Windows/excercises/" + configFilename;
        }
    }
}
