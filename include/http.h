#pragma once

#include <string>
#include <vector>

class Http {
public:
	friend class HttpParser;

	Http() = default;
	virtual ~Http() = default;

	std::string GetMethod() { return method_; }
	std::string GetHost() { return host_; }
	std::string GetPath() { return path_; }
	std::string GetPaylod() { return payload_; }

	std::string ToString();

	bool IsCredz();

private:
	static const std::vector<std::string> acctFields;
	static const std::vector<std::string> passwdFields;

	bool IsPayloadContainFields(std::vector<std::string> fields);

	std::string method_;
	std::string path_;
	std::string host_;
	std::string payload_;
};

