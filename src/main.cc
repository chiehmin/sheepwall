extern "C" {
#include <unistd.h>
#include <getopt.h>
}

#include <memory>
#include <iostream>
#include <string>

#include "pcap_sniffer.h"
#include "logger/logger_interface.h"
#include "logger/stdout_logger.h"
#include "logger/file_logger.h"

using namespace std;

void PrintHelp(string appName)
{
	/* cerr << "Usage:" << endl */
	/* 	 << "  " << appName << " -i <interface> [-w <output file>]" << endl; */
	cerr << "Usage:" << endl
		 << "  " << appName << " <interface> [<output file>]" << endl;
}

int main(int argc, char **argv)
{
	char opt;
	string devName;
	string outputFile;

	// XXX: 1900 toolchain has a bug for getopt...so we are not allowed to use it
	/* static struct option long_options[] = { */
	/* 	{"iface", required_argument, 0, 'i'}, */
	/* 	{"write", required_argument, 0, 'w'}, */
	/* 	{"help", no_argument, 0, 'h'}, */
	/* 	{0, 0, 0, 0} */
	/* }; */
	/* while(-1 != (opt = getopt_long(argc, argv, "hiw", long_options, nullptr))) { */
	/* 	switch(opt) { */
	/* 	case 'i': */
	/* 		devName = string(optarg); */
	/* 		break; */
	/* 	case 'w': */
	/* 		outputFile = string(optarg); */
	/* 		break; */
	/* 	case 'h': */
	/* 		PrintHelp(argv[0]); */
	/* 		exit(0); */
	/* 	case ':': */
	/* 	case '?': */
	/* 		PrintHelp(argv[0]); */
	/* 		exit(-1); */
	/* 	} */
	/* } */

	if (2 > argc) {
		PrintHelp(argv[0]);
		exit(-1);
	}
	if (1 < argc) {
		devName = string(argv[1]);
	}
	if (2 < argc) {
		cout << argv[2] << endl;
		outputFile = argv[2];
	}

	if (0 == devName.size()) {
		cerr << "You must specify a interface for sniffering" << endl;
		PrintHelp(argv[0]);
		exit(-1);
	}

	cout << "Interface: " << devName << endl;

	unique_ptr<LoggerInterface> logger;
	if (0 < outputFile.size()) {
		unique_ptr<FileLogger> fileLogger = make_unique<FileLogger>(outputFile);
		fileLogger->Init();
		logger = move(fileLogger);
		cout << "Output: " << outputFile << endl;
	} else {
		logger = make_unique<StdoutLogger>();
		cout << "Output: stdout" << endl;
	}

	cout << "Start sniffering..." << endl;
	PcapSniffer sniffer(devName, move(logger));
	sniffer.Start();

	return 0;
}

