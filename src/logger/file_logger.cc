#include "logger/file_logger.h"

extern "C" {
#include <libgen.h> // dirname
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
}

#include <stdexcept>
#include <iostream>

using namespace std;

FileLogger::FileLogger(string filePath):
	filePath_(filePath)
{}

void FileLogger::Init()
{
	// check if parent directory exists
	string filePathCopy = filePath_; // because dirname will change the arg, we pass a copy here
	string parentDir = string(dirname(filePathCopy.c_str()));

	struct stat sb;
	if (0 != stat(parentDir.c_str(), &sb) || !S_ISDIR(sb.st_mode)) {
		throw runtime_error("parent directory not exists or path is invalid");
	}

	// open file
	ofs_.open(filePath_, ofstream::out | ofstream::app);
}

void FileLogger::Log(const Http &http)
{
	time_t curTime = time(nullptr);
	ofs_ << ctime(&curTime); // return value from ctime has newline at the end already
	ofs_ << http.ToString() << endl;
	ofs_.flush();
}

