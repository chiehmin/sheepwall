#include "logger/file_logger.h"

extern "C" {
#include <stdio.h>
}

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include "http.h"
#include "http_parser.h"

using namespace std;

class HttpMock : public Http {
public:
	virtual string ToString() {
		return "FileLoggerTest";
	}
};

TEST(FileLoggerTest, basic)
{
	// arrange
	const string testFile = "/tmp/sheepwall_test.log";
	FileLogger logger(testFile);
	HttpMock httpMock;

	// act
	logger.Init();
	logger.Log(httpMock);

	// assert
	ifstream ifs(testFile);
	ostringstream sbuffer;
	sbuffer << ifs.rdbuf();

	string res = sbuffer.str();
	cout << res << endl;
	EXPECT_TRUE(string::npos != res.find("FileLoggerTest"));

	// cleanup
	remove(testFile.c_str());
}

TEST(FileLoggerTest, invalid)
{
	FileLogger logger("/tmp/jfisdo/rqwjiogf");

	// act and assert
	EXPECT_THROW(logger.Init(), runtime_error);
}
