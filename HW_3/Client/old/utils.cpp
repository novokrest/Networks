#include "common.h"
#include "utils.h"
#include "errors.h"

int get_host_addr_by_ip_and_port(const char *host_ip, unsigned short int host_port, sockaddr_in *host_sockaddr_in)
{
    struct in_addr host_in_addr;
    if (!inet_aton(host_ip, &host_in_addr)) {
        return ERROR_IP_NOT_VALID;
    }

    bzero((char*) host_sockaddr_in, sizeof(struct sockaddr_in));
    host_sockaddr_in->sin_family = AF_INET;
    host_sockaddr_in->sin_addr = host_in_addr;
    host_sockaddr_in->sin_port = htons(host_port);

    return 0;
}

int get_host_addr_by_ip_and_port(const char* host_ip, const char* host_port, struct sockaddr_in* host_sockaddr_in)
{
    return get_host_addr_by_ip_and_port(host_ip, atoi(host_port), host_sockaddr_in);
}

int get_random(int max_value)
{
    return get_random_range(0, max_value);
}

int get_random_range(int a, int b)
{
    if (a > b) {
        int c = a;
        a = b;
        b = c;
    }

    return (rand() % (b + 1 - a)) + a;
}

int send_all(int sock, char* buf, int len, int flags)
{
    int total = 0;

    while(total < len) {
        int n = send(sock, buf, len, flags);
        cout << n << " bytes was sent" << endl;
        if (n == -1) {
            return -1;
        }
        total += n;
    }

    return total;
}

