#include "includes/traceroute.h"

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

void setPort(struct addrinfo *addr, int port){
    ((struct sockaddr_in *)addr->ai_addr)->sin_port = htons(port);
}

void setNewTTL(int sock, int ttl){
	if (setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0){
		perror("setsockopt");
	}
}

int portUnreachable(struct response *res){
	return res->icmp_header.type == 3 && res->icmp_header.code == 3;
}

int timeoutReached(struct response *res){
	return res->icmp_header.type == 11 && res->icmp_header.code == 0;
}

short isEmpty(struct response *res, size_t size){
    if (!res){
        return 1;
	}		
	for (size_t i = 0; i < size; i++)
	{
		if (res[i].raw)
			return 0;
	}
	return 1;
}


short isNumber(char *str){
    for (size_t i = 0; str[i]; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

