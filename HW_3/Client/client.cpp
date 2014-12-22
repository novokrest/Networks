#include "client.h"
#include "utils.h"


client::client(const string& name, struct sockaddr_in server_addr)
    : name_(name), server_addr_(server_addr)
{
    logger_.set_author(name_);
    logger_.set_echo(true);
    logger_.start();
}

client::~client()
{
    logger_.log("Exit");
    logger_.save();
}

int client::connect_to_server()
{
    stringstream ss;

    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0) {
        logger_.log("Couldn't create socket");
        perror(strerror(errno));
        logger_.log("Exit");
        exit(EXIT_FAILURE);
    }

    ss << "Try connect socket " << sock_
       << " to server: IP = " << inet_ntoa(server_addr_.sin_addr)
       << ", port = " << ntohs(server_addr_.sin_port) << endl;
    logger_.log(ss.str());
    ss.clear();

    if (connect(sock_, (const sockaddr*)&server_addr_, sizeof(server_addr_)) < 0) {
        logger_.log("Error occurred while connecting to server");
        perror(strerror(errno));
        logger_.log("Exit");
        exit(EXIT_FAILURE);
    }

    logger_.log("Succesfully connected");

    return 0;
}

int client::send_request()
{
    stringstream ss;

    logger_.log("Generating request...");
    request request_to_server = request::generate_request();
    string serialized;
    request_to_server.serialize_to_string(serialized);

    logger_.log("Try to send data to server");
    int bytes = send_all(sock_, (char*)serialized.c_str(), serialized.size(), 0);
    if (bytes < 0) {
        logger_.log("Not all data has been sent to server. Close connection");
        perror(strerror(errno));
        close_connection();
        logger_.log("Exit");
        exit(EXIT_FAILURE);
    }

    ss << "All data has been sent: " << bytes << " bytes" << endl;
    logger_.log(ss.str());
    ss.clear();

    return 0;
}

int client::receive_response()
{
    stringstream ss;

    logger_.log("Wait for response...");

    char buf[RESPONSE_MAX_LENGTH];
    response response_from_server;
    int bytes_readed = recv(sock_, buf, RESPONSE_MAX_LENGTH, 0);

    if (bytes_readed < 0) {
        logger_.log("Error occurred while receiving response");
        perror(strerror(errno));
        close_connection();
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    if (bytes_readed == 0) {
        logger_.log("Receive 0 bytes. Server closed connection. So i'm closed connection too. Goodbye!");
        close_connection();
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    ss << "Response has been received. Total: " << bytes_readed << " bytes" << endl;
    logger_.log(ss.str());
    ss.str("");
    ss.clear();

    response_from_server.deserialize_from_string(string(buf, bytes_readed));
    if (!response_from_server.is_valid()) {
        logger_.log("Response not valid!");
        close_connection();
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    logger_.log("Response is valid. Getting result");

    int32_t result = response_from_server.result();

    ss << "Result: " << result;
    logger_.log(ss.str());
    ss.str("");
    ss.clear();

    return 0;
}

int client::close_connection()
{
    logger_.log("Close connection to server");
    if (close(sock_) < 0) {
        logger_.log("Error occurred while socket closing");
        perror(strerror(errno));
        return -1;
    }

    return 0;
}

int client::start()
{
    connect_to_server();
    send_request();
    receive_response();
    close_connection();

    return 0;
}

void client::exit(int flag)
{
    logger_.save();
    ::exit(flag);
}
