#include "common.h"
#include "utils.h"
#include "errors.h"


int init_random()
{
    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
    srand(cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000);

    return 0;
}

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
        if (n == -1) {
            return -1;
        }
        total += n;
    }

    return total;
}

int check_connection(int sock)
{
    int error = 0;
    socklen_t len = sizeof(error);
    int retval = getsockopt(sock, SOL_SOCKET,SO_ERROR, &error, &len);
    if (retval < 0) {
        cout << "bad connection" << endl;
    }

    return retval;
}

bool is_file_exists(string& file_name)
{
    ifstream file(file_name.c_str());
    return file.good();
}

bool comp_elements(int16_t i, int16_t j)
{
    return i < j;
}

