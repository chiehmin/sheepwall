#pragma once

#include <string>
#include <vector>

class Http {
public:
	friend class HttpParser;

	Http() = default;
	virtual ~Http() = default;

	virtual std::string GetMethod() { return method_; }
	virtual std::string GetPath() { return path_; }
	virtual std::string GetHost() { return host_; }
	virtual std::string GetCookie() { return cookie_; }
	virtual std::string GetPaylod() { return payload_; }

	virtual std::string ToString();

	virtual bool IsCredz();

private:
	static const std::vector<std::string> acctFields;
	static const std::vector<std::string> passwdFields;
	static const std::vector<std::string> sessionFields;

	bool IsContainFields(std::string content, std::vector<std::string> fields);

	std::string method_;
	std::string path_;
	std::string host_;
	std::string cookie_;
	std::string payload_;
};

