#include "http_parser.h"

#include <cstring>

#include <iostream>
#include <map>
#include <sstream>
#include <utility>

#include <boost/algorithm/string.hpp>

using namespace std;

Http HttpParser::Parse(const char *payload, int len)
{
	if (strcasestr(payload, "GET") != payload &&
			strcasestr(payload, "POST") != payload) {
		// TODO: use custom exception
		throw runtime_error("not http packet");
	}

	// parse http header
	istringstream payload_ss{payload};
	map<string, string> headers;
	string line;
	while (getline(payload_ss, line) && "\r" != line) {
		int idx = line.find(':');
		if (string::npos == idx) {
			continue;
		}
		// TODO: impl trim
		string key = boost::algorithm::to_lower_copy(line.substr(0, idx));
		string value = boost::algorithm::to_lower_copy(line.substr(idx + 1));
		boost::algorithm::trim(key);
		boost::algorithm::trim(value);
		headers[key] = value;
		cout << key << ": " << value << endl;
	}

	Http http;
	http.host_ = headers["host"];

	return http;
}

