#include "./includes/traceroute.h"

void showInit(struct traceroute_args args, struct addrinfo addr){
	printf("traceroute to %s (%s), %ld hops max, %ld byte packets\n", args.host, ipv4ToString(((struct sockaddr_in *)addr.ai_addr)->sin_addr.s_addr), args.max_hops, args.packet_size + sizeof(struct ip_header) + sizeof(struct udp_header));
}

void showResponseTime(struct response *res){
    char buffer[5];
    sprintf(buffer, "%ld", res->time);
    write(1, "  ", 2);
    write(1, buffer, ft_strlen(buffer, 0));
    write(1, " ms", 3);
}

void showNoResponse(size_t retry, struct traceroute_args args){
    if (retry < args.max_retries){
        write(1, "  ", 2);
    }
	write(1, "*", 2);
}

void showResponse(struct response res, size_t retry, struct traceroute_args args){
    if (retry < args.max_retries){
        write(1, "  ", 2);
    }
	write(1, ipv4ToString(res.ip_header.src_ip), ft_strlen(ipv4ToString(res.ip_header.src_ip), 0));
	write(1, "  ", 2);
	showResponseTime(&res);
}


void showHeader(int ttl){
    write(1, "\n", 1);
    write(1, ft_itoa(ttl), ft_strlen(ft_itoa(ttl), 0));
    write(1, "  ", 2);
}

void showError(struct response *res){
    if (res->icmp_header.type == 3 && res->icmp_header.code == 3)
        write(1, " !N", 3);
    else if (res->icmp_header.type == 11 && res->icmp_header.code == 0)
        write(1, " !T", 3);
    else if (res->icmp_header.type == 3 && res->icmp_header.code == 1)
        write(1, " !H", 3);
    else if (res->icmp_header.type == 3 && res->icmp_header.code == 2)
        write(1, " !P", 3);
    else if (res->icmp_header.type == 3 && res->icmp_header.code == 9)
        write(1, " !D", 3);
    else if (res->icmp_header.type == 3 && res->icmp_header.code == 10)
        write(1, " !C", 3);
    else if (res->icmp_header.type == 3 && res->icmp_header.code == 13)
        write(1, " !A", 3);
    else
        write(1, " !U", 3);
}