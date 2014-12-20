#ifndef SERVER
#define SERVER

#include "common.h"
#include "request.h"

static const int MAX_BACKLOG = 100;

class server
{
    int listener_;
    struct sockaddr_in addr_;

    fd_set active_set;

    vint clients_;
    vpid_t workers_;

public:
    server(unsigned short int portno);

    int start();
    int create_client_connection();

    void say_about_self(const char* message);
    int read_request(int client_sock);
    int process_request(int client_sock, request rst);
};

#endif // SERVER

