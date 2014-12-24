#ifndef SERVER
#define SERVER

#include "common.h"
#include "request.h"
#include "response.h"
#include "logger.h"

static const int MAX_BACKLOG = 1000;

class server
{
    int listener_;
    int clientsock_;
    struct sockaddr_in addr_;

    timeval timeout_;

    vint clients_;
    vpid_t workers_;

    string name_;
    logger logger_;

public:
    server(unsigned short int portno);
    ~server();

    string worker_name();
    int start();
    void wait_worker(pid_t pid);
    int create_client_connection();
    int process_client();
    int receive_request(request& out_client_request);
    int process_request(request& client_request, response& out_response_to_client);
    int send_response(response& response_to_client);
    void close_connection(int sock);

    void exit(int flag);
};

#endif // SERVER

