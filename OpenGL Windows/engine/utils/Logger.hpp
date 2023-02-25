#pragma once

#include <iostream>

class Logger {
public:
	static void Log(const std::string&);
	static void Warning(const std::string&);
private:
	static bool enabled;
};