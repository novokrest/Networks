#ifndef UTILS_H
#define UTILS_H

#include <netinet/in.h>

int init_random();

int get_host_addr_by_ip_and_port(const char* host_ip, const char* host_port, struct sockaddr_in* host_sockaddr_in);
int get_host_addr_by_ip_and_port(const char *host_ip, unsigned short int host_port, sockaddr_in *host_sockaddr_in);

int get_random(int max_value);
int get_random_range(int a, int b);

int send_all(int sock, char* buf, int len, int flags);
int check_connection(int sock);

bool is_file_exists(string& file_name);
bool comp_elements(int16_t i, int16_t j);

template <typename S>
void to_bytes(char* buf, size_t len, S value)
{
    size_t svalue = (size_t)value;
    for (size_t i = 0; i < len; ++i) {
        buf[i] = (unsigned char)(svalue >> 8 * i);
    }
}

template <typename R>
R from_bytes(const char* buf, size_t len)
{
    size_t value = 0;
    for (size_t i = 0; i < len; ++i) {
        value += ((unsigned char)(buf[i]) << 8 * i);
    }

    return (R)(value);
}


#endif // UTILS_H

