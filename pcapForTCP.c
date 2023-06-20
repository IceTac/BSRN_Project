#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>

void parseTcpHeader(const unsigned char* packet) {
    // Move the packet pointer to the IP header
    const struct ip* ipHeader = (struct ip*)(packet + sizeof(struct ether_header));

    //Extract the IP header fields
    unsigned char* sourceIP = inet_ntoa(ipHeader->ip_src);
    unsigned char* destinationIP = inet_ntoa(ipHeader->ip_dst);

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
    printf("Source IP: %s\n", sourceIP);
    printf("Destination Port: %u\n", destinationPort);
    printf("Destination IP: %s\n", destinationIP);
    printf("Sequence Number: %u\n", sequenceNumber);
    printf("Acknowledgment Number: %u\n", acknowledgmentNumber);
    printf("Header Length: %u bytes\n", headerLength);
    printf("Flags: 0x%02X\n", flags);

    // Open the log file in append mode
    FILE* logFile = fopen("tcpheaders.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file\n");
        return;
    }

    // Write the TCP header information to the log file
    fprintf(logFile, "Source Port: %u\n", sourcePort);
    fprintf(logFile,"Source IP: %s\n", sourceIP);
    fprintf(logFile, "Destination Port: %u\n", destinationPort);
    fprintf(logFile,"Destination IP: %s\n", destinationIP);
    fprintf(logFile, "Sequence Number: %u\n", sequenceNumber);
    fprintf(logFile, "Acknowledgment Number: %u\n", acknowledgmentNumber);
    fprintf(logFile, "Header Length: %u bytes\n", headerLength);
    fprintf(logFile, "Flags: 0x%02X\n", flags);

    // Close the log file
    fclose(logFile);
}

void packetHandler(unsigned char* userData, const struct pcap_pkthdr* pkthdr, const unsigned char* packet) {
    printf("---------------\nPacket captured\n--------------\n");
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
    }else
        printf("Success opening device...\n");

    // Set a filter
    struct bpf_program fp;
    char filter_exp[] = "tcp";
    bpf_u_int32 subnet_mask, ip;

    // Compiling the filter to check existence
    if (pcap_compile(handle, &fp, filter_exp, 0, ip) == -1) {
        printf("Error compiling filter expression: %s\n", pcap_geterr(handle));
        return 1;
    }

    // Setting the filter only to TCP-Packets
    if (pcap_setfilter(handle, &fp) == -1) {
        printf("Error setting filter: %s\n", pcap_geterr(handle));
        return 1;
    }else
        printf("Success setting filter...\n");
    printf("TCP-Packet capturing is active...\n");

    // Start capturing packets
    pcap_loop(handle, 0, packetHandler, NULL);

    // Close the pcap handle
    pcap_close(handle);

    return 0;
}
