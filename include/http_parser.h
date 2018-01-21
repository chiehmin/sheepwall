#pragma once

#include "http.h"

class HttpParser {
public:
	HttpParser() = default;
	~HttpParser() = default;

	Http Parse(const char* payload, int len);
};

