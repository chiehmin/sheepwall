#include "http_parser.h"

#include <gtest/gtest.h>

using namespace std;

TEST(HttpParserTest, Parse)
{
	//arrange
	const char* payload =
		"GET /pages/about.html HTTP/1.1\r\n"
		"HOST: fatminmin.com\r\n"
		"\r\n";
	const int len = strlen(payload);

	// act
	HttpParser httpParser;
	Http http = httpParser.Parse(payload, len);

	// assert
	EXPECT_EQ("fatminmin.com", http.GetHost());
}

