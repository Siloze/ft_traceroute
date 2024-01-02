#include "../includes/traceroute.h"

uint16_t checksum(void *data, size_t len){
    uint16_t *buf = data;
    uint32_t sum = 0;
    uint16_t result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(uint8_t *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

struct icmp_header initIcmpHeader(uint8_t type, uint8_t code){
    struct icmp_header header;
    header.type = type;
    header.code = code;
    header.checksum = 0;
    header.id = getpid();
    header.seq = 0;
    return header;
}

ssize_t sendIcmpPacket(int sockfd, struct sockaddr_in *addr, struct icmp_header *packet){
    packet->checksum = checksum(packet, sizeof(struct icmp_header));
    return sendto(sockfd, packet, sizeof(struct icmp_header), 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
}

struct icmp_header extractIcmpHeader(char *buf){
    char *ptr;
    struct icmp_header header;

    ptr = buf + sizeof(struct ip_header);
    ft_memcpy(&header, ptr, sizeof(struct icmp_header));
    return header;
}