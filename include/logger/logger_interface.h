#pragma once

#include "http.h"

class LoggerInterface {
public:
	LoggerInterface() = default;
	virtual ~LoggerInterface() = default;

	virtual void Log(const Http &http) = 0;
};
