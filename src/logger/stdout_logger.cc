#include "logger/stdout_logger.h"

extern "C" {
#include <time.h>
}

#include <iostream>

using namespace std;

void StdoutLogger::Log(const Http &http)
{
	time_t curTime = time(nullptr);
	cout << ctime(&curTime); // return value from ctime has newline at the end already
	cout << http.ToString() << endl;
}

