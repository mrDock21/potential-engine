
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
        std::string GetRootFolder(const std::string& asset) {
            return "./../../../../" + asset;
        }

        std::string GetShaderPath(const std::string& asset) {
            return "./../../../../Shaders/" + asset;
        }

        std::string GetAssetPath(const std::string& asset) {
            return "./../../../../assets/" + asset;
        }

        std::string GetShaderCode(std::string file) {
            ShaderFile s(file);

            return s.Code();
        }
    }
}
