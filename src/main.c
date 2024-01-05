#include "includes/traceroute.h"

int main(int ac, char **argv){

    if (ac < 2)
        return printf("Error: not enough arguments\nUsage: %s [--help][<host> {packet-lenght}]\n", argv[0]), 1;
    else if (ac == 2 && !ft_strcmp(argv[1], "--help"))
        return printf("Help:\nUsage: %s [--help | <host> (optional)<packet-lenght>]\n", argv[0]), 0;
    else if (ac == 3 && !isNumber(argv[2]))
        return printf("Error: wrong packet lenght\nUsage: %s [--help][<host> {packet-lenght}]\n", argv[0]), 1;
    else if (ac == 3 && ft_atoi(argv[2]) < 52)
        return printf("Error: packet lenght must be greater than 51\nUsage: %s [--help][<host> {packet-lenght}]\n", argv[0]), 1;
    else if (ac == 3 && ft_atoi(argv[2]) > 32768)
        return printf("Error: packet lenght must be <= than 32768\nUsage: %s [--help][<host> {packet-lenght}]\n", argv[0]), 1;
    else if (ac >= 4)
        return printf("Error: too many arguments\nUsage: %s [--help][<host> {packet-lenght}]\n", argv[0]), 1;

    int MAX_TIME = 2000;
    int MAX_PROBES = 3;
    int MAX_HOPS = 64;
    int PACKET_LENGHT = ac == 3 ?   ft_atoi(argv[2]) - (sizeof(struct ip_header) + sizeof(struct udp_header)) 
                                    : 24;

    struct traceroute_args args = {
        .host = argv[1],
        .max_time = MAX_TIME,
        .max_probes = MAX_PROBES,
        .max_hops = MAX_HOPS,
        .max_retries = 3,
        .packet_size = PACKET_LENGHT
    };

    traceroute(args);

    return 0;
}