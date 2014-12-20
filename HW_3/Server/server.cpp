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
    addr_.sin_addr.s_addr = htonl(INADDR_ANY); // a.s_addr;
    addr_.sin_port = htons(portno);
}

int server::start()
{
    if (bind(listener_, (const sockaddr*)&addr_, sizeof(struct sockaddr)) < 0) {
        cout << "Couldn't bind listener socket" << endl;
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }
    cout << "Listener " << listener_ << " bind to: " << inet_ntoa(addr_.sin_addr) << " : " << ntohs(addr_.sin_port) << endl;

    if (listen(listener_, 5) < 0) {
        cout << "Couldn't start listen" << endl;
        perror(strerror(errno));
        return -1;
    }
    cout << "Start listen..." << endl;

    FD_ZERO(&active_set);
    FD_SET(listener_, &active_set);

    timeval timeout;
    timeout.tv_sec = 7;
    timeout.tv_usec = 0;

    fd_set readfs;
    while(true) {
        FD_ZERO(&readfs);
        readfs = active_set;

        cout << "Listening for connections" << endl;
        int active_count = select(listener_ + 1, &readfs, NULL, NULL, &timeout);
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
            int client_sock = create_client_connection();
            if (client_sock > 0) {

            }
        }
    }
}

int server::create_client_connection()
{
    struct sockaddr_in client_addr;
    int client_addrlen;
    int clientsock = accept(listener_, (sockaddr*)&client_addr, (socklen_t*)&client_addrlen);
    if (clientsock < 0) {
        cout << "Error occurred while client socket creating" << endl;
        perror(strerror(errno));
        return -1;
    }

    cout << "It's time to fork!" << endl;
    pid_t pid = fork();
    if (pid < 0) {
        cout << "Error occurred: couldn't fork. Close connection..." << endl;
        perror(strerror(errno));
        if (close(clientsock) < 0) {
            cout << "Error occurred while closing client connection: socket " << clientsock << endl;
        }
        return -1;
    }
    if (pid > 0) {
        workers_.push_back(pid);
        if (close(clientsock) < 0) {
            cout << "Error occurred while closing client connection: socket " << clientsock << endl;
        }
        return 0;
    }

    cout << "Worker " << getpid() << " is started" << endl;
    cout << "Incomming connection info: " << inet_ntoa(client_addr.sin_addr) << " : " << ntohs(client_addr.sin_port) << endl;
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

    read_request(clientsock);
    //process_request(clientsock);

    return 0;
}

void server::say_about_self(const char* message)
{
    cout << "Worker " << getpid() << ": " << message << endl;
}

int server::read_request(int client_sock)
{
    say_about_self("Begin read data from client");

    char buf[1000000];

    ssize_t bytes_readed = recv(client_sock, buf, sizeof(buf), 0);
    stringstream message;
    message << bytes_readed << " bytes was received from client " << client_sock;
    say_about_self(message.str().c_str());

    exit(EXIT_SUCCESS);
}

int server::process_request(int client_sock, request reqst)
{
    exit(EXIT_SUCCESS);
    return 0;
}
