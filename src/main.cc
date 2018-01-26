#include <iostream>

#include "pcap_sniffer.h"

using namespace std;

int main(int argc, char **argv)
{
	/* check for capture device name on command-line */
	if (argc == 2) {
		string devName = argv[1];
		PcapSniffer sniffer(devName);
		sniffer.Start();
	}

	return 0;
}

