#ifndef SERVER
#define SERVER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "request.h"

class server
{
    int listener_;
    struct sockaddr_in addr_;

    fd_set active_set;

    vint clients_;

public:
    server(unsigned short int portno);

    int bind();
    int listen();
    int start();

    int create_client_connection();

    int process_request(int client_sock, request rst);
};

#endif // SERVER

