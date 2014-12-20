#ifndef UTILS_H
#define UTILS_H

#include <netinet/in.h>

int get_host_addr_by_ip_and_port(const char* host_ip, const char* host_port, struct sockaddr_in* host_sockaddr_in);
int get_host_addr_by_ip_and_port(const char *host_ip, unsigned short int host_port, sockaddr_in *host_sockaddr_in);

int get_random(int max_value);
int get_random_range(int a, int b);

int send_all(int sock, char* buf, int len, int flags);

#endif // UTILS_H

