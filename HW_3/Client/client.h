#ifndef CLIENT
#define CLIENT

#include "common.h"
#include "request.h"
#include "response.h"
#include "logger.h"


class client
{
    string name_;
    struct sockaddr_in server_addr_;
    int sock_;

    logger logger_;

public:
    client(const string& name, sockaddr_in server_addr_);
    ~client();

    int start();
    int connect_to_server();
    int send_request();
    int receive_response();
    int close_connection();

    void exit(int flag);
};

#endif // CLIENT

