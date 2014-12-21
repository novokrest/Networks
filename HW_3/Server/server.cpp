#include "common.h"
#include "server.h"
#include "utils.h"

server::server(unsigned short int portno)
    : name_("main_listener")
{
    listener_ = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(listener_, F_SETFL, O_NONBLOCK);

    bzero((char*)&addr_, sizeof(struct sockaddr_in));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(portno);

    timeout_.tv_sec = 5;
    timeout_.tv_usec = 3;

    logger_.set_author(name_);
    logger_.set_echo(true);
    logger_.start();
}

server::~server()
{
    logger_.log("Exit");
    logger_.save();
}

string server::worker_name()
{
    return name_;
}

int server::start()
{
    if (bind(listener_, (const sockaddr*)&addr_, sizeof(struct sockaddr)) < 0) {
        logger_.log("Couldn't bind socket for listening.");
        perror(strerror(errno));
        logger_.log("Exit");
        exit(EXIT_FAILURE);
    }

    stringstream ss;
    ss << "Listener " << listener_ << " bind to: " << inet_ntoa(addr_.sin_addr) << " : " << ntohs(addr_.sin_port) << endl;
    logger_.log(ss.str());
    ss.clear();

    if (listen(listener_, MAX_BACKLOG) < 0) {
        logger_.log("Couldn't start listening");
        perror(strerror(errno));
        return -1;
    }
    logger_.log("Start listening...");

    while(true) {
        fd_set readfs;
        FD_ZERO(&readfs);
        FD_SET(listener_, &readfs);

        logger_.log("Listening for connection");
        int active_count = select(listener_ + 1, &readfs, NULL, NULL, &timeout_);
        if (active_count < 0) {
            logger_.log("Error occurred while waiting for connections");
            perror(strerror(errno));
            logger_.log("Exit");
            exit(EXIT_FAILURE);
        }
        else if (active_count == 0) {
            logger_.log("No connections is active. Continue waiting...");
            continue;
        }

        ss << "There are " << active_count << " active connections. Start for its processing..." << endl;
        logger_.log(ss.str());
        ss.clear();

        if (FD_ISSET(listener_, &readfs)) {
            logger_.log("New incoming connection is available!");
            create_client_connection();
        }
    }
}

int server::create_client_connection()
{
    stringstream ss;

    logger_.log("Try to create connection to client");
    struct sockaddr_in client_addr;
    int client_addrlen;
    int clientsock = accept(listener_, (sockaddr*)&client_addr, (socklen_t*)&client_addrlen);
    if (clientsock < 0) {
        logger_.log("Error occurred while accept client connection. Connection hasn't been created");
        perror(strerror(errno));
        return -1;
    }

    logger_.log("Connection is accepted! It's time to fork!");
    pid_t pid = fork();
    if (pid < 0) {
        logger_.log("Error occurred: couldn't fork. Close connection...");
        perror(strerror(errno));
        if (close(clientsock) < 0) {
            ss << "Error occurred while closing client connection: socket " << clientsock;
            logger_.log(ss.str());
            ss.clear();
        }
        else {
            ss << "Client socket " << clientsock << "was succesfully closed";
            logger_.log(ss.str());
            ss.clear();
        }
        return -1;
    }
    if (pid > 0) {
        workers_.push_back(pid);
        ss << "New worker " << pid << " is created";
        logger_.log(ss.str());
        ss.clear();
        logger_.log("Close client connection...");
        if (close(clientsock) < 0) {
            ss << "Error occurred while closing client connection: socket " << clientsock;
            logger_.log(ss.str());
            ss.clear();
        }
        logger_.log("My work is done. Continue listening...");
        return 0;
    }

    name_ = "worker_" + std::to_string(pid);
    logger_.clear();
    logger_.set_author(name_);
    logger_.start();

    logger_.log("Hello, I'm new worker!");
    clientsock_ = clientsock;
    fcntl(clientsock_, F_SETFL, O_NONBLOCK);

    ss.clear();
    ss << "Incomming connection info: " << inet_ntoa(client_addr.sin_addr) << " : " << ntohs(client_addr.sin_port) << endl;
    ss << "New client socket was created: " << clientsock << endl;
    logger_.log(ss.str());
    ss.clear();

    process_client();

    return 0;
}

int server::process_client()
{
    while (true) {
        request client_request;
        response response_to_client;

        receive_request(client_request);
        process_request(client_request, response_to_client);
        send_response(response_to_client);
    }
}

int server::receive_request(request& out_client_request)
{
    stringstream ss;

    int attempts = 0;
    while (true) {
        fd_set readfs;
        FD_ZERO(&readfs);
        FD_SET(clientsock_, &readfs);

        logger_.log("Start wait for client data...");
        int active = select(clientsock_ + 1, &readfs, NULL, NULL, &timeout_);
        ++attempts;
        if (active > 0) {
            break;
        }
        logger_.log("No data received. Continue waiting...");
        if (attempts == 5) {
            ss << "Timeout is end. Close client socket " << clientsock_ << "Goodbye!";
            logger_.log(ss.str());
            ss.clear();
            if (close(clientsock_) < 0) {
                ss << "Error occurred while closing client connection: socket " << clientsock_;
                logger_.log(ss.str());
                ss.clear();
            }
            logger_.log("Exit");
            exit(EXIT_SUCCESS);
        }
    }

    char buf[REQUEST_MAX_LENGTH];
    ssize_t bytes_readed = recv(clientsock_, buf, sizeof(buf), 0);

    if (bytes_readed < 0) {
        logger_.log("Error was occurred while receiving data from client");
        close_connection(clientsock_);
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    if (bytes_readed == 0) {
        logger_.log("Receive 0 bytes. Client closed connection. Goodbye!");
        close_connection(clientsock_);
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    ss << bytes_readed << " bytes were received from client " << clientsock_;
    logger_.log(ss.str());
    ss.clear();

    out_client_request.deserialize_from_string(string(buf, bytes_readed));
    return 0;
}

int server::process_request(request& client_request, response& out_response_to_client)
{
    logger_.log("Start to process client request");
    if (!client_request.is_valid()) {
        logger_.log("Request is not valid!");
        out_response_to_client = response::create_bad_request(name_);
        return -1;
    }

    logger_.log("Request is valid. Begin result computing...");

    vint16& numbers = client_request.numbers();
    std::sort(numbers.begin(), numbers.end(), comp_elements);
    int32_t result = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        result += i % 2 == 0 ? i * numbers[i] : - i * numbers[i];
    }

    logger_.log("Computing completed");
    out_response_to_client = response::create_ok(name_, result);
    return 0;
}

int server::send_response(response& response_to_client)
{
    string serialized;
    response_to_client.serialize_to_string(serialized);

    logger_.log("Send response to client");
    if (send_all(clientsock_, (char*)serialized.c_str(), serialized.size(), 0) < 0) {
        logger_.log("Error occurred during send response");
        close_connection(clientsock_);
        perror(strerror(errno));
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    return 0;
}

void server::close_connection(int sock)
{
    stringstream ss;
    if (close(sock) < 0) {
        ss << "Error occurred while closing client connection: socket " << clientsock_;
        logger_.log(ss.str());
        ss.clear();
    }
}

void server::exit(int flag)
{
    logger_.save();
    ::exit(flag);
}
