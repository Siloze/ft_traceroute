#include "includes/traceroute.h"

int initSocket(){
	int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	return sock;
}

struct addrinfo getHostAddress(char *ip){
	struct addrinfo hints;
	struct addrinfo *result;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	int s = getaddrinfo(ip, NULL, &hints, &result);
	if (s != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	return *result;
}

void printPacket(char *buffer){
	struct icmp_header recevHeader = extractIcmpHeader(buffer);
	printf("ICMP Header from %s: ", ipv4ToString(extractIpHeader(buffer).src_ip));
	printf("type: %d | ", recevHeader.type);
	printf("code: %d | ", recevHeader.code);
	printf("checksum: %d", recevHeader.checksum);
}

void showNoResponse(int retry){
	write(1, "* ", 2);
	if (!retry)
		write(1, "\n", 1);
}

void showResponse(struct response res){
	write(1, ipv4ToString(res.ip_header.src_ip), 16);
	write(1, "  ", 2);
	write(1, ft_itoa(res.time), ft_strlen(ft_itoa(res.time), 0));
	write(1, "\n", 1);
}

int arrayContain(uint32_t *array, uint32_t ip, size_t len){
	for (size_t i = 0; i < len; i++)
	{
		if (array[i] == ip)
			return i;
	}
	return 0;
}

void showResponses(struct response *res, size_t len){
	// uint32_t showIp[len];
	for (size_t i = 0; i < len; i++)
	{
		if (res[i].raw)
		{
			showResponse(res[i]);
			// if (!arrayContain(showIp, res[i].ip_header.src_ip, len))
			// {
			// 	showIp[i] = res[i].ip_header.src_ip;
			// 	showResponse(res[i]);
			// }
		}
	}
}

void sendProbes(int sock, struct addrinfo *addr, struct icmp_header *packet, size_t nbProbes){
	while (nbProbes--)
		sendIcmpPacket(sock, (struct sockaddr_in *)addr->ai_addr, packet);
}

struct response *waitProbesResponse(int sock, struct addrinfo *addr, size_t nbProbes){
	char   buf[1024];
	struct response *res;

	res = malloc(sizeof(struct response) * nbProbes);

	startClock();
	while (getClock() < MAX_TIME && nbProbes){
		ft_memset(buf, 0, 1024);
		if (recvfrom(sock, buf, 1024, MSG_DONTWAIT, (struct sockaddr *)addr->ai_addr, &addr->ai_addrlen) > 0)
		{
			nbProbes--;
			res[nbProbes].raw = buf;
			res[nbProbes].ip_header = extractIpHeader(buf);
			res[nbProbes].icmp_header = extractIcmpHeader(buf);
			res[nbProbes].time = getClock();
		}
	}
	stopClock();
	return res;
}

short isEmpty(struct response *res){
	for (size_t i = 0; i < MAX_PROBES; i++)
	{
		if (res[i].raw)
			return 0;
	}
	return 1;
}

void startLoop(int sock, struct addrinfo addr, struct icmp_header packet){
	int ttl = 1;
	int retry = 3;
	struct	response *res;

	while (ttl < 10){


		if (retry == 3)
		{
			write(1, ft_itoa(ttl), ft_strlen(ft_itoa(ttl), 0));
			write (1, "  ", 2);
		}
	
		setsockopt(sock, IPPROTO_IP, IP_TTL, &(int){ttl}, sizeof(int));

		sendProbes(sock, &addr, &packet, MAX_PROBES);

		res = waitProbesResponse(sock, &addr, MAX_PROBES);

		if (isEmpty(res))
			showNoResponse(--retry);
		else
			showResponses(res, MAX_PROBES);
		
		if (!retry)
			retry = 3;
		if (retry == 3)
			ttl++;

		// free(res);

		// if (extractIcmpHeader(buf).type == ICMP_ECHOREPLY && extractIcmpHeader(buf).code == 0)
		// 	break;
	}
}

void traceroute(char *host){
	int sock;
	struct addrinfo addr;
	struct icmp_header packet;

	sock = initSocket();
	packet = initIcmpHeader(ICMP_ECHO, 0);
	addr = getHostAddress(host);
	
	startLoop(sock, addr, packet);

}