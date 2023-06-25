#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <time.h>

// Methode zum Extrahieren der Informationen aus IP und UDP Header
void parseUdpHeader(const unsigned char* packet) {
    // Verschiebung des Fokus vom "Ethernet Header" auf den "IP Header"
    const struct ip* ipHeader = (struct ip*)(packet + sizeof(struct ether_header));

    // Extrahierung der beiden IP-Adressen für Quelle und Ziel aus dem IP Header
    unsigned char* sourceIP = inet_ntoa(ipHeader->ip_src);
    unsigned char* destinationIP = inet_ntoa(ipHeader->ip_dst);

    // Verschiebung des Fokus vom "IP Header" auf den "TCP Header"
    const struct udphdr* udpHeader = (struct udphdr*)(packet + sizeof(struct ether_header) + ipHeader->ip_hl * 4);

    // Extrahierung der Ports, Header-Länge und CheckSumme
    unsigned int sourcePort = ntohs(udpHeader->uh_sport);
    unsigned int destinationPort = ntohs(udpHeader->uh_dport);
    unsigned int length = ntohs(udpHeader->uh_ulen);
    unsigned int checksum = ntohs(udpHeader->uh_sum);

    // Ausgabe aller extrahierten Informationen mit einer passenden Beschreibung
    printf("Source Port: %u\n", sourcePort);
    printf("Source IP: %s\n", sourceIP);
    printf("Destination Port: %u\n", destinationPort);
    printf("Destination IP: %s\n", destinationIP);
    printf("Length: %u bytes\n", length);
    printf("Checksum: 0x%04X\n", checksum);

    // Erstellen / Öffnen des LOG-Files
    FILE* logFile = fopen("udpheaders.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file\n");
        return;
    }

    // Datum und Zeit herausfinden
    time_t t;
    t = time(NULL);
    struct tm tm;
    tm = *localtime(&t);

    // Anhängen der extrahierten Informationen mit Beschreibung an das geöffnete LOG-File
    fprintf(logFile, "Timestamp: %d:%d:%d %d-%d-%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    fprintf(logFile, "Source Port: %u\n", sourcePort);
    fprintf(logFile, "Source IP: %s\n", sourceIP);
    fprintf(logFile, "Destination Port: %u\n", destinationPort);
    fprintf(logFile, "Destination IP: %s\n", destinationIP);
    fprintf(logFile, "Length: %u bytes\n", length);
    fprintf(logFile, "Checksum: 0x%04X\n\n", checksum);

    // Schließen des LOG-Files
    fclose(logFile);
}

// Methode zum Bestätigen des Einfangens eines Pakets und Weitergabe des Pakets zur Extrahier-Methode
void packetHandler(unsigned char* userData, const struct pcap_pkthdr* pkthdr, const unsigned char* packet) {
    printf("---------------\nPacket captured\n---------------\n");
    parseUdpHeader(packet);
}

// Main Methode zum Erstellen eines PCAP-Geräts, um Netzwerk-Kommunikation mitzuschneiden
int main() {
    // Benötigte Variable zum Speichern und Weitergeben aller Informationen (Das PCAP-Gerät)
    pcap_t* handle;
    // Benötigter Error-Puffer im Falle eines Fehlers
    char errbuf[PCAP_ERRBUF_SIZE];

    // Öffnen des Netzwerk-Interfaces mit Erfolg- und Fehler-Meldung
    handle = pcap_open_live("lo", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        printf("Error opening device: %s\n", errbuf);
        return 1;
    }else
        printf("Success opening device...\n");

    // Einstellen des Filters auf das gewünschte Protokoll
    struct bpf_program fp;
    char filter_exp[] = "udp";
    bpf_u_int32 subnet_mask, ip;

    // Kompilieren des Filters, zur Überprüfung ob der Filter existiert mit möglicher Fehler-Anzeige
    if (pcap_compile(handle, &fp, filter_exp, 0, ip) == -1) {
        printf("Error compiling filter expression: %s\n", pcap_geterr(handle));
        return 1;
    }

    // Finales Einstellen des Filters mit Erfolg- und Fehler-Meldung
    if (pcap_setfilter(handle, &fp) == -1) {
        printf("Error setting filter: %s\n", pcap_geterr(handle));
        return 1;
    }else
        printf("Success setting filter...\n");
    // Finale Aktivierungsnachricht
    printf("UDP-Packet capturing is active...\n");

    // Methode zum eigentlichen Einfangen der Pakete
    pcap_loop(handle, 0, packetHandler, NULL);

    // Schließen des PCAP Geräts
    pcap_close(handle);

    return 0;
}
