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

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(7778);
    server_addr_.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    cout << "Try connect socket " << sock_ << " to server: IP = " << inet_ntoa(server_addr_.sin_addr) << ", port = " << ntohs(server_addr_.sin_port) <<endl;
    if (connect(sock_, (const sockaddr*)&server_addr_, sizeof(server_addr_)) < 0) {
        cout << "Error occurred while connecting to server" << endl;
        perror(strerror(errno));
        exit(EXIT_FAILURE);
        //return ERROR_CONNECTING;
    }
    cout << "Succesfully connected" << endl;

    return 0;
}

int client::send_request()
{
    const int REQUEST_LENGTH = get_random(REQUEST_MAX_LENGTH);
    //std::vector<char> buf;
    request reqst(REQUEST_LENGTH);
    reqst.serialize();

    char buf[1000000];
    cout << "Start sending request with length = " << sizeof(buf) << endl;
//    if (send_all(sock_, (char*)&buf, REQUEST_LENGTH, 0) < 0) {

    int bytes = send_all(sock_, buf, sizeof(buf), 0);
    if (bytes < 0) {
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }
    cout << "Send all data: " << bytes << " bytes" << endl;

    return 0;
}

int client::wait_for_response()
{
    const int RESPONSE_LENGTH = 100;

    return 0;
}

int client::close_connection()
{
    if (close(sock_) < 0) {
        cout << "Error occurred while socket closing" << endl;
        perror(strerror(errno));
        return -1;
    }

    return 0;
}

