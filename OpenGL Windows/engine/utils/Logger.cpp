#include "Logger.hpp"

bool Logger::enabled = true;

void Logger::Log(const std::string& str) {

	if (!Logger::enabled)
		return;

	std::cout << "[LOG] " << str << std::endl;
}