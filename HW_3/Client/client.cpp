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
    //fcntl(sock_, F_SETFL, O_NONBLOCK);
    if (sock_ < 0) {
        logger_.log("Couldn't create socket");
        logger_.log(strerror(errno));
        logger_.log("Exit");
        exit(EXIT_FAILURE);
    }

    ss << "Try connect socket " << sock_
       << " to server: IP = " << inet_ntoa(server_addr_.sin_addr)
       << ", port = " << ntohs(server_addr_.sin_port) << endl;
    logger_.log(ss.str());
    ss.clear(); ss.str("");

    if (connect(sock_, (const sockaddr*)&server_addr_, sizeof(server_addr_)) < 0) {
        logger_.log("Error occurred while connecting to server");
        logger_.log(strerror(errno));
        logger_.log("Exit");
        exit(EXIT_FAILURE);
    }

    logger_.log("Succesfully connected");

    return 0;
}

int client::send_request(string& serialized_request)
{
    stringstream ss;

    logger_.log("Try to send data to server");
    int bytes = send_all(sock_, (char*)serialized_request.c_str(), serialized_request.size(), 0);
    if (bytes < 0) {
        logger_.log("Not all data has been sent to server. Close connection");
        logger_.log(strerror(errno));
        close_connection();
        logger_.log("Exit");
        exit(EXIT_FAILURE);
    }

    ss << "All data has been sent: " << bytes << " bytes" << endl;
    logger_.log(ss.str());
    ss.clear(); ss.str("");

    return 0;
}

int client::receive_response()
{
    stringstream ss;

    char buf[RESPONSE_MAX_LENGTH];
    response response_from_server;

    logger_.log("Wait for response...");

    int bytes_readed = recv(sock_, buf, RESPONSE_MAX_LENGTH, 0);

    if (bytes_readed < 0) {
        logger_.log("Error occurred while receiving response");
        logger_.log(strerror(errno));
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
    ss.clear(); ss.str("");

    response_from_server.deserialize_from_string(string(buf, bytes_readed));
    if (!response_from_server.is_valid()) {
        logger_.log("Response not valid!");
        close_connection();
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    logger_.log("Response is valid");
    if (response_from_server.status() == STATUS_BAD_REQUEST) {
        logger_.log("STATUS BAD_REQUEST");
        close_connection();
        logger_.log("Exit");
        exit(EXIT_SUCCESS);
    }

    logger_.log("STATUS OK");

    logger_.log("Getting result from response");
    int32_t result = response_from_server.result();

    ss << "RESULT " << result;
    logger_.log(ss.str());
    ss.str("");
    ss.clear(); ss.str("");

    return 0;
}

int client::close_connection()
{
    logger_.log("Close connection to server");
    if (close(sock_) < 0) {
        logger_.log("Error occurred while socket closing");
        logger_.log(strerror(errno));
        return -1;
    }

    return 0;
}

int client::start(size_t request_size)
{
    logger_.log("GENERATE_REQUEST_START");
    request request_to_server = request_size > 0 ? request::generate_request(request_size) : request::generate_request();
    string serialized_request;
    request_to_server.serialize_to_string(serialized_request);
    logger_.log("GENERATE_REQUEST_END");

    logger_.log("CONNECT_TO_SERVER_START");
    connect_to_server();
    logger_.log("CONNECT_TO_SERVER_END");

    logger_.log("SEND_REQUEST_START");
    send_request(serialized_request);
    logger_.log("SEND_REQUEST_END");

    logger_.log("RECEIVE_RESPONSE_START");
    receive_response();
    logger_.log("RECEIVE_RESPONSE_END");

    logger_.log("CLOSE_CONNECTION_START");
    close_connection();
    logger_.log("CLOSE_CONNECTION_END");

    return 0;
}

void client::exit(int flag)
{
    logger_.save();
    ::exit(flag);
}
