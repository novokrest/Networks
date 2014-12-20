#include "common.h"
#include "server.h"
#include "utils.h"

server::server(unsigned short int portno)
{
    listener_ = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(listener_, F_SETFL, O_NONBLOCK);

    bzero((char*)&addr_, sizeof(struct sockaddr_in));
    addr_.sin_family = AF_INET;

    struct in_addr a;
    inet_aton("192.168.1.2", &a);
    addr_.sin_addr.s_addr = a.s_addr;
    addr_.sin_port = htons(portno);
}

int server::start()
{
    return 0;
}

int server::listen()
{
    if (bind(listener_, (const sockaddr*)&addr_, sizeof(struct sockaddr)) < 0) {
        cout << "Couldn't bind socket" << endl;
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }

    cout << "Listener bind to: " << inet_ntoa(addr_.sin_addr) << " : " << ntohs(addr_.sin_port) << endl;

    FD_ZERO(&active_set);
    FD_SET(listener_, &active_set);

    timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    fd_set readfs;
    while(true) {
        FD_ZERO(&readfs);
        readfs = active_set;

        cout << "Listening for connections" << endl;
        int active_count = select(FD_SETSIZE, &readfs, NULL, NULL, &timeout);
        if (active_count < 0) {
            cout << "Error occurred while waiting for connections" << endl;
            perror(strerror(errno));
            exit(EXIT_FAILURE);
        }
        else if (active_count == 0) {
            cout << "No connections is active. Continue waiting..." << endl;
            continue;
        }

        cout << "There are " << active_count << " active connections. Start for its processing..." << endl;
        if (FD_ISSET(listener_, &readfs)) {
            cout << "New incoming connection is available! Try to connect..." << endl;
            create_client_connection();
        }

        //fork();
    }
}

int server::create_client_connection()
{
    struct sockaddr_in client_addr;
    int client_addrlen;
    int clientsock = accept(listener_, (sockaddr*)&client_addr, (socklen_t*)&client_addrlen);
    if (clientsock < 0) {
        cout << "Error occurred while client socket creating" << endl;
        return -1;
    }

    cout << "Incomming connection info: IP = " << inet_ntoa(client_addr.sin_addr)
         << ", port = " << ntohs(client_addr.sin_port) << endl;
    cout << "New client socket was created: " << clientsock << endl;

    clients_.push_back(clientsock);
    FD_SET(clientsock, &active_set);

    string hello = "Hello from server!";
    cout << "Send hello to client" << endl;
    if (send_all(clientsock, (char*)hello.c_str(), hello.length(), 0) < 0) {
        cout << "Error occurred while sending hello to client" << endl;
        perror(strerror(errno));
    }
    cout << "Hello message has been successfully!" << endl;

    return 0;
}

int server::process_request(int client_sock, request rst)
{
    return 0;
}
