#pragma once

#include "../../Lib/src/includes.h"
#include "structs.h"

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
#include <math.h>
#include <string.h>

void traceroute(struct traceroute_args args);

ssize_t     sendUdpPacket(int sockfd, struct sockaddr *ai_addr, size_t packet_size);
ssize_t     sendIcmpPacket(int sockfd, struct sockaddr_in *addr, struct icmp_header *packet);

struct ip_header    extractIpHeader(char *buf);
struct icmp_header  extractIcmpHeader(char *buf);


char            *ipv4ToString(uint32_t ip);
struct addrinfo getHostAddress(char *ip);
uint16_t        checksum(void *data, size_t len);


void    showHeader(int ttl);
void    showError(struct response *res);
void    showResponseTime(struct response *res);
void    showNoResponse(size_t retry, struct traceroute_args args);
void    showInit(struct traceroute_args args, struct addrinfo addr);
void    showResponse(struct response res, size_t retry, struct traceroute_args args);

int     portUnreachable(struct response *res);
int     timeoutReached(struct response *res);
short   isEmpty(struct response *res, size_t size);
short   isNumber(char *str);

void    setNewTTL(int sock, int ttl);
void    setPort(struct addrinfo *addr, int port);

void    startClock();
void    resetClock();
size_t  getClock();
size_t  stopClock();
size_t  getInterval();