#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "Logger.hpp"

class ShaderFile {
public:
	ShaderFile();
	ShaderFile(const std::string&);
	~ShaderFile();

	bool Read(const std::string&);

	const std::string& Code() const;
private:
	std::string code;
};