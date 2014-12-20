#include "client.h"
#include "request.h"
#include "utils.h"

#include <cerrno>

client::client(struct sockaddr_in server_addr)
    : server_addr_(server_addr)
{
}

int client::connect_to_server()
{
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0) {
        perror(strerror(errno));
        exit(EXIT_FAILURE);
        //return ERROR_OPEN_SOCKET;
    }

    if (connect(sock_, (const sockaddr*)&server_addr_, sizeof(struct sockaddr_in)) < 0) {
        perror(strerror(errno));
        exit(EXIT_FAILURE);
        //return ERROR_CONNECTING;
    }

    return 0;
}

int client::send_request()
{
    const int REQUEST_LENGTH = get_random(REQUEST_MAX_LENGTH);
    std::vector<char> buf;
    request reqst(REQUEST_LENGTH);
    reqst.serialize((char*)&buf);

    if (send_all(sock_, (char*)&buf, REQUEST_LENGTH, 0) < 0) {
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}

int client::wait_for_response()
{
    const int RESPONSE_LENGTH = 100;

    return 0;
}



