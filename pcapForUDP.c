#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

void parseUdpHeader(const unsigned char* packet) {
    // Move the packet pointer to the IP header
    const struct ip* ipHeader = (struct ip*)(packet + sizeof(struct ether_header));

    //Extract IP header fields
    unsigned char* sourceIP = inet_ntoa(ipHeader->ip_src);
    unsigned char* destinationIP = inet_ntoa(ipHeader->ip_dst);

    // Move the packet pointer to the UDP header
    const struct udphdr* udpHeader = (struct udphdr*)(packet + sizeof(struct ether_header) + ipHeader->ip_hl * 4);

    // Extract UDP header fields
    unsigned int sourcePort = ntohs(udpHeader->uh_sport);
    unsigned int destinationPort = ntohs(udpHeader->uh_dport);
    unsigned int length = ntohs(udpHeader->uh_ulen);
    unsigned int checksum = ntohs(udpHeader->uh_sum);

    // Print the extracted UDP header fields
    printf("Source Port: %u\n", sourcePort);
    printf("Source IP: %s\n", sourceIP);
    printf("Destination Port: %u\n", destinationPort);
    printf("Destination IP: %s\n", destinationIP);
    printf("Length: %u bytes\n", length);
    printf("Checksum: 0x%04X\n", checksum);

    // Open the log file in append mode
    FILE* logFile = fopen("udpheaders.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file\n");
        return;
    }

    // Write the UDP header information to the log file
    fprintf(logFile, "Source Port: %u\n", sourcePort);
    fprintf(logFile,"Source IP: %s\n", sourceIP);
    fprintf(logFile, "Destination Port: %u\n", destinationPort);
    fprintf(logFile,"Destination IP: %s\n", destinationIP);
    fprintf(logFile, "Length: %u bytes\n", length);
    fprintf(logFile, "Checksum: 0x%04X\n\n", checksum);

    // Close the log file
    fclose(logFile);
}

void packetHandler(unsigned char* userData, const struct pcap_pkthdr* pkthdr, const unsigned char* packet) {
    printf("Packet captured\n");
    parseUdpHeader(packet);
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
    char filter_exp[] = "udp";
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
