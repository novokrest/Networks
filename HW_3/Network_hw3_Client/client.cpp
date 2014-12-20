#include "common.h"
#include "client.h"
#include "request.h"
#include "utils.h"


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

    cout << "Try connect to server: IP = " << inet_ntoa(server_addr_.sin_addr) << ", port = " << ntohs(server_addr_.sin_port) <<endl;
    if (connect(sock_, (const sockaddr*)&server_addr_, sizeof(struct sockaddr_in)) < 0) {
        cout << "Error occurred while connecting to server" << endl;
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
    reqst.serialize();

    cout << "Strt sending request with length = " << REQUEST_LENGTH << endl;
//    if (send_all(sock_, (char*)&buf, REQUEST_LENGTH, 0) < 0) {
    if (send_all(sock_, "hello, server!", 14, 0) < 0) {
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



