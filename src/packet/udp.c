#include "../includes/traceroute.h"

ssize_t sendUdpPacket(int sockfd, struct sockaddr *ai_addr, size_t packet_size){
    char packet[packet_size];

    ssize_t send = sendto(sockfd, packet, packet_size, 0, ai_addr, sizeof(*ai_addr));
    if (send < 0 )
        perror("sendto");
    return send;
}