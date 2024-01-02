#pragma once

#include "../../Lib/src/includes.h"

#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <math.h>

#define MAX_TIME 2000
#define MAX_PROBES 3
struct ip_header{
	uint8_t version:4;
	uint8_t ihl:4;
	uint8_t service;
	uint16_t len;
	uint16_t id;
	uint16_t flags:3;
	uint16_t frag_offset:13;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t src_ip;
	uint32_t dst_ip;
};

struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;

};

struct response {
    size_t				time;
	char				*raw;
	struct ip_header	ip_header;
	struct icmp_header	icmp_header;
};

void traceroute(char *host);

struct icmp_header initIcmpHeader(uint8_t type, uint8_t code);
ssize_t sendIcmpPacket(int sockfd, struct sockaddr_in *addr, struct icmp_header *packet);
struct icmp_header extractIcmpHeader(char *buf);

struct ip_header extractIpHeader(char *buf);
char *ipv4ToString(uint32_t ip);

void startClock();
size_t stopClock();
void resetClock();
size_t getClock();

size_t getInterval();