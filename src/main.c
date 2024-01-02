#include "includes/traceroute.h"


int main(int ac, char **argv){

    if (ac < 2)
        return 1;
    else if (ac == 3 && !ft_strcmp(argv[2], "--help"))
        return printf("Help:\nUsage: %s <host>\n", argv[0]), 0;
    else if (ac >= 3)
        return printf("Error: wronge arguments\nUsage: %s <host>\n", argv[0]), 1;
    

    traceroute(argv[1]);

    return 0;
}