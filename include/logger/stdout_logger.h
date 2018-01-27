#pragma once

#include "logger/logger_interface.h"

class StdoutLogger : public LoggerInterface {
public:
	StdoutLogger() = default;
	virtual ~StdoutLogger() = default;

	virtual void Log(const Http &http);
};
