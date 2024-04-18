#include "horizontalScan.h"

// Fonction pour calculer le checksum (pour simplification, cette fonction peut ne pas gérer tous les cas)
unsigned short calculate_checksum(void *b, int len) {    
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int horizontalScan(char *scanTarget, char *scanMask, char **ips, int *ipsCount) {
    //remove \n from scanTarget and scanMask
    scanTarget[strcspn(scanTarget, "\n")] = 0;
    scanMask[strcspn(scanMask, "\n")] = 0;

    struct in_addr targetAddr, maskAddr, netAddr, broadAddr;
    int sockfd;
    struct sockaddr_in sa;
    char packet[sizeof(struct icmphdr)];
    struct icmphdr *icmp_hdr = (struct icmphdr *) packet;
    int devicesFound = 0;

    if (!inet_pton(AF_INET, scanTarget, &targetAddr) || !inet_pton(AF_INET, scanMask, &maskAddr)) {
        printf("Invalid IP address: %s\n", scanTarget);
        return -1;
    }

    netAddr.s_addr = targetAddr.s_addr & maskAddr.s_addr;
    broadAddr.s_addr = netAddr.s_addr | ~maskAddr.s_addr;

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;

    memset(packet, 0, sizeof(packet));
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->checksum = 0;
    icmp_hdr->un.echo.id = random();
    icmp_hdr->un.echo.sequence = 0;
    icmp_hdr->checksum = calculate_checksum(packet, sizeof(packet));

    for (uint32_t addr = ntohl(netAddr.s_addr); addr <= ntohl(broadAddr.s_addr); addr++) {
        //if broadcast address, skip => sinon erreur de sendto
        if (addr == ntohl(broadAddr.s_addr)) {
            continue;
        }
        sa.sin_addr.s_addr = htonl(addr);

        if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&sa, sizeof(sa)) <= 0) {
            perror("Ping send failed");
            continue;
        }

        //écoute de la réponse ICMP
        fd_set readfds;
        struct timeval tv; // timeout
        tv.tv_sec = 1; // 1s
        tv.tv_usec = 0; // 0us
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        printf("Scanning %s\n", inet_ntoa(sa.sin_addr));
        if (select(sockfd + 1, &readfds, NULL, NULL, &tv) <= 0) {
            printf("Timeout\n");
            continue;
        }

        char buffer[1024];
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addr_len) <= 0) {
            perror("Ping receive failed");
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
        ips[devicesFound] = strdup(ip);
        printf("Device found: %s\n", ip);
        devicesFound++;

        if (devicesFound >= 256) {
            break;
        }
    }

    close(sockfd);

    *ipsCount = devicesFound;
    return 0;
}