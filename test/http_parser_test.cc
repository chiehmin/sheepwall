#include "http_parser.h"

#include <gtest/gtest.h>

using namespace std;

TEST(HttpParserTest, Parse_get_request)
{
	// arrange
	// curl fatminmin.com/pages/about.html --trace-ascii -
	const char payload[] =
		"GET /pages/about.html HTTP/1.1\r\n"
		"Host: fatminmin.com\r\n"
		"User-Agent: curl/7.47.0\r\n"
		"Accept: */*\r\n"
		"\r\n";
	const int len = strlen(payload);

	// act
	HttpParser httpParser;
	Http http = httpParser.Parse(payload, len);

	// assert
	EXPECT_EQ("get", http.GetMethod());
	EXPECT_EQ("/pages/about.html", http.GetPath());
	EXPECT_EQ("fatminmin.com", http.GetHost());
	EXPECT_EQ("", http.GetCookie());
	EXPECT_EQ("", http.GetPaylod());
}

TEST(HttpParserTest, Parse_get_request_cookie)
{
	const char payload[] =
		"GET /App/Shop/Default.aspx HTTP/1.1\r\n"
		"Host: www.stellainjp.com.tw\r\n"
		"User-Agent: curl/7.47.0\r\n"
		"Accept: */*\r\n"
		"Cookie: BUYER_SHOP_CART=3170190p2q1282av1a7j; sessionid=12345;\r\n"
		"\r\n";

	const int len = strlen(payload);

	// act
	HttpParser httpParser;
	Http http = httpParser.Parse(payload, len);

	// assert
	EXPECT_EQ("get", http.GetMethod());
	EXPECT_EQ("/App/Shop/Default.aspx", http.GetPath());
	EXPECT_EQ("www.stellainjp.com.tw", http.GetHost());
	EXPECT_EQ("BUYER_SHOP_CART=3170190p2q1282av1a7j; sessionid=12345;", http.GetCookie());
	EXPECT_EQ("", http.GetPaylod());
}

TEST(HttpParserTest, Parse_post_requst)
{
	// curl -d "username=fatminmin&password=5566" -X POST "http://httpbin.org/post" --trace-ascii -
	const char payload[] =
		"POST /post HTTP/1.1\r\n"
		"Host: httpbin.org\r\n"
		"User-Agent: curl/7.47.0\r\n"
		"Accept: */*\r\n"
		"Content-Length: 32\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"\r\n"
		"username=fatminmin&password=5566";
	const int len = strlen(payload);

	// act
	HttpParser httpParser;
	Http http = httpParser.Parse(payload, len);

	// assert
	EXPECT_EQ("post", http.GetMethod());
	EXPECT_EQ("/post", http.GetPath());
	EXPECT_EQ("httpbin.org", http.GetHost());
	EXPECT_EQ("username=fatminmin&password=5566", http.GetPaylod());
}

