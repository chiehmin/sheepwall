extern "C" {
#include <unistd.h>
#include <getopt.h>
}

#include <memory>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "pcap_sniffer.h"
#include "logger/logger_interface.h"
#include "logger/stdout_logger.h"
#include "logger/file_logger.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
	char opt;
	string devName;
	string outputFile;

	po::options_description desc("options");
	desc.add_options()
		("help,h", "Print help messages")
		("interface,i", po::value<string>(), "Interface sniffered by sheepwall")
		("write_to,w", po::value<string>(), "Log file")
		("daemon,D", "Daemonizing... run in the background");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cerr << desc << endl;
		exit(0);
	}

	if (vm.count("interface")) {
		devName = vm["interface"].as<string>();
	} else {
		cerr << "You must specify an interface to sniffer..." << endl;
		cerr << desc << endl;
		exit(-1);
	}

	if (vm.count("write_to")) {
		outputFile = vm["write_to"].as<string>();
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

	if (vm.count("daemon")) {
		daemon(0, 0);
	}

	PcapSniffer sniffer(devName, move(logger));
	sniffer.Start();

	return 0;
}

