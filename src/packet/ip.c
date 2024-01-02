#include "../includes/traceroute.h"

char *ipv4ToString(uint32_t ip){
    char *str;
    struct in_addr src_addr;

    str = malloc(INET_ADDRSTRLEN * sizeof(char));

    src_addr.s_addr = ip;
    inet_ntop(AF_INET, &src_addr, str, INET_ADDRSTRLEN);

    return str;
}


struct ip_header extractIpHeader(char *buf){
    struct ip_header header;
    ft_memcpy(&header, buf, sizeof(struct ip_header));
    return header;
}