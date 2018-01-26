#pragma once

#include <string>

class Http {
public:
	friend class HttpParser;

	Http() = default;
	virtual ~Http() = default;

	std::string GetMethod() { return method_; }
	std::string GetHost() { return host_; }
	std::string GetPath() { return path_; }
	std::string GetPaylod() { return payload_; }

private:
	std::string method_;
	std::string path_;
	std::string host_;
	std::string payload_;
};

