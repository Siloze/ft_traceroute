#include "includes/traceroute.h"

int initSocket(int type, int protocol){
	int sock = socket(AF_INET, type, protocol);
	if (sock < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	return sock;
}

void nextGateway(size_t *ttl, int *retry, uint32_t *lastIp, struct traceroute_args args){
	*ttl += 1;
	*retry = args.max_retries;
	*lastIp = 0;
}

void sendProbes(int sock, struct addrinfo *addr, struct icmp_header *packet, size_t nbProbes){
	while (nbProbes--)
	{
		sendIcmpPacket(sock, (struct sockaddr_in *)addr->ai_addr, packet);
		usleep(100);
	}
}

struct response *waitResponse(int sock, struct traceroute_args args){
	char  				buf[2024];
	struct	response	*res;

	res = NULL;
	ft_memset(buf, 0, 2024);

	startClock();
	while (getClock() < (args.max_time) && !res){
		if (recvfrom(sock, buf, 2024, MSG_DONTWAIT, NULL, NULL) > 0)
		{
			res = malloc(sizeof(struct response));
			res->raw = buf;
			res->ip_header = extractIpHeader(buf);
			res->icmp_header = extractIcmpHeader(buf);
			res->time = getClock();
		}
	}
	stopClock();
	return res;
}


void startLoop(int udp_sock, int icmp_sock, struct addrinfo *addr, struct traceroute_args args){
	size_t				ttl = 1;
	uint32_t			lastIp = 0;
	int					retry = args.max_retries;
	struct	response	*res;

	while (ttl < args.max_hops){

		if (retry == 3)
			showHeader(ttl);

		setNewTTL(udp_sock, ttl);
		setPort(addr, 33434 + ttl - 1);

		sendUdpPacket(udp_sock, addr->ai_addr, args.packet_size);
		res = waitResponse(icmp_sock, args);

		if (isEmpty(res, 1))
			showNoResponse(retry, args);
		else if (lastIp != res->ip_header.src_ip)
			showResponse(*res, retry, args);
		else
			showResponseTime(res);

		if (res)
		{
			lastIp = res->ip_header.src_ip;
			if (portUnreachable(res))
				break;
			else if (!timeoutReached(res))
				showError(res);
			free(res);
		}

		if (!--retry)
			nextGateway(&ttl, &retry, &lastIp, args);

	}
	write(1, "\n", 1);
	if (res)
		free(res);
}

void traceroute(struct traceroute_args args){
	struct addrinfo	addr;
	int				udp_sock;
	int				icmp_sock;

	addr 		=	getHostAddress(args.host);
	udp_sock 	=	initSocket(SOCK_DGRAM, IPPROTO_UDP);
	icmp_sock 	=	initSocket(SOCK_RAW, IPPROTO_ICMP);
	
	startLoop(udp_sock, icmp_sock, &addr, args);

	close(udp_sock);
	close(icmp_sock);
}