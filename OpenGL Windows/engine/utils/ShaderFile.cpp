#include "ShaderFile.hpp"

ShaderFile::ShaderFile() { }

/**
 * Will read the code contents immediately
 * @param filename File to open
*/
ShaderFile::ShaderFile(const std::string& filename) {
    Read(filename);
}

ShaderFile::~ShaderFile() {
    code.clear();
}

/**
 * Will open the file and read the code contents
 * @param filename File to open
*/
bool ShaderFile::Read(const std::string& filename) {
	std::ifstream shaderfile(filename);
    std::string line;

    code.clear();

    if (!shaderfile.is_open())
    {
        Logger::Log("Couldn't open the file: " + filename);
        return false;
    }
    //else cout << "Unable to open file";

    while (std::getline(shaderfile, line))
    {
        code += line + '\n';
    }
    shaderfile.close();

    return true;
}

const std::string& ShaderFile::Code() const {
    return code;
}