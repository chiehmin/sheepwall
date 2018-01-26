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

	Http resHttp;

	// parse http header
	istringstream payload_ss{payload};
	map<string, string> headers;
	string line;

	// parse action
	getline(payload_ss, line);
	vector<string> actions;
	boost::algorithm::split(actions, line, boost::algorithm::is_any_of(" "));

	if (0 < actions.size()) {
		resHttp.method_ = boost::algorithm::to_lower_copy(actions[0]);
	}
	if (1 < actions.size()) {
		resHttp.path_ = boost::algorithm::to_lower_copy(actions[1]);
	}

	// parse HTTP headers
	while (getline(payload_ss, line) && "\r" != line) {
		int idx = line.find(':');
		if (string::npos == idx) {
			continue;
		}

		string key = line.substr(0, idx);
		string value = line.substr(idx + 1);
		boost::algorithm::trim(key);
		boost::algorithm::trim(value);
		boost::algorithm::to_lower(key);
		boost::algorithm::to_lower(value);

		headers[key] = value;
		cout << key << ": " << value << endl;
	}

	// parse HTTP payload
	string httpPayload = "";
	while(getline(payload_ss, line)) {
		httpPayload += line;
	}

	resHttp.host_ = headers["host"];
	resHttp.payload_ = httpPayload;

	return resHttp;
}

