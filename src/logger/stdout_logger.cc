#include "logger/stdout_logger.h"

#include <time.h>

#include <iostream>

using namespace std;

void StdoutLogger::Log(const Http &http)
{
	time_t curTime = time(nullptr);
	cout << ctime(&curTime) << endl;
	cout << http.ToString() << endl;
}

