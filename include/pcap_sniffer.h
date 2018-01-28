#pragma once

extern "C" {
#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
}

#include <memory>
#include <string>

#include "sniff.h"
#include "http_parser.h"
#include "logger/logger_interface.h"
#include "logger/stdout_logger.h"

class PcapSniffer {
public:
	PcapSniffer(
			std::string devName,
			std::unique_ptr<LoggerInterface> logger = std::make_unique<StdoutLogger>()
		);
	virtual ~PcapSniffer();

	static void HandlePacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
	void Start();

private:
	void OpenDevice();
	void SetupFilter();

	std::string devName_;
	pcap_t *handle_ = nullptr;
	struct bpf_program fp_;	/* compiled filter program (expression) */

	HttpParser httpParser_;
	std::unique_ptr<LoggerInterface> logger_;
};

