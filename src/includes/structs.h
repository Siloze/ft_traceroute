#pragma once


#include <stdint.h>
#include <stdio.h>

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

struct udp_header {
	uint16_t src_port;
	uint16_t dst_port;
	uint16_t len;
	uint16_t checksum;
};

struct response {
    size_t				time;
	char				*raw;
	struct ip_header	ip_header;
	struct icmp_header	icmp_header;
};

struct traceroute_args {
	char	*host;
	size_t	max_probes;
	size_t	max_hops;
	size_t	max_time;
	size_t	max_retries;

	size_t	packet_size;
};