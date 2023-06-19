#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>

void parseTcpHeader(const unsigned char* packet) {
    // Move the packet pointer to the IP header
    const struct ip* ipHeader = (struct ip*)(packet + sizeof(struct ether_header));

    // Move the packet pointer to the TCP header
    const struct tcphdr* tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + ipHeader->ip_hl * 4);

    // Extract TCP header fields
    unsigned int sourcePort = ntohs(tcpHeader->th_sport);
    unsigned int destinationPort = ntohs(tcpHeader->th_dport);
    unsigned int sequenceNumber = ntohl(tcpHeader->th_seq);
    unsigned int acknowledgmentNumber = ntohl(tcpHeader->th_ack);
    unsigned int headerLength = tcpHeader->th_off * 4;
    unsigned int flags = tcpHeader->th_flags;

    // Print the extracted TCP header fields
    printf("Source Port: %u\n", sourcePort);
    printf("Destination Port: %u\n", destinationPort);
    printf("Sequence Number: %u\n", sequenceNumber);
    printf("Acknowledgment Number: %u\n", acknowledgmentNumber);
    printf("Header Length: %u bytes\n", headerLength);
    printf("Flags: 0x%02X\n", flags);

    // Open the log file in append mode
    FILE* logFile = fopen("tcpheaders.log", "w");
    if (logFile == NULL) {
        printf("Error opening log file\n");
        return;
    }

    // Write the TCP header information to the log file
    fprintf(logFile, "Source Port: %u\n", sourcePort);
    fprintf(logFile, "Destination Port: %u\n", destinationPort);
    fprintf(logFile, "Sequence Number: %u\n", sequenceNumber);
    fprintf(logFile, "Acknowledgment Number: %u\n", acknowledgmentNumber);
    fprintf(logFile, "Header Length: %u bytes\n", headerLength);
    fprintf(logFile, "Flags: 0x%02X\n", flags);

    // Close the log file
    fclose(logFile);
}

void packetHandler(unsigned char* userData, const struct pcap_pkthdr* pkthdr, const unsigned char* packet) {
    printf("Packet captured\n");
    parseTcpHeader(packet);
}

int main() {
    pcap_t* handle;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Open network interface for capturing packets
    handle = pcap_open_live("lo", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        printf("Error opening device: %s\n", errbuf);
        return 1;
    }

    // Set a filter if desired (optional)
    struct bpf_program fp;
    char filter_exp[] = "tcp";
    bpf_u_int32 subnet_mask, ip;

    if (pcap_compile(handle, &fp, filter_exp, 0, ip) == -1) {
        printf("Error compiling filter expression: %s\n", pcap_geterr(handle));
        return 1;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        printf("Error setting filter: %s\n", pcap_geterr(handle));
        return 1;
    }

    // Start capturing packets
    pcap_loop(handle, 0, packetHandler, NULL);

    // Close the pcap handle
    pcap_close(handle);

    return 0;
}
