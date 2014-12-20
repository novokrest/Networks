#ifndef CLIENT
#define CLIENT

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class client
{
    struct sockaddr_in server_addr_;
    int sock_;

public:
    client(sockaddr_in server_addr_);

    int connect_to_server();
    int send_request();
    int wait_for_response();
};

#endif // CLIENT

