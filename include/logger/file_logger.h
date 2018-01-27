#pragma once

#include <string>
#include <fstream>

#include "logger/logger_interface.h"

class FileLogger : public LoggerInterface {
public:
	FileLogger(std::string filePath);
	virtual ~FileLogger() = default;

	virtual void Log(const Http &http);

private:
	std::string filePath_;
	std::ofstream ofs_;
};

