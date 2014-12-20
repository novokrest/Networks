#include "common.h"
#include "client.h"
#include "utils.h"
#include "errors.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

static const int ARGUMENTS_COUNT = 5;
static const char* HELP_MESSAGE = "client [-sip <server_ip>] [-sport <server_port>]";

static int parse_arguments(char* argv[], string& server_ip, string& server_port)
{
    for (int i = 1; i < ARGUMENTS_COUNT; i += 2) {
        if (!strcmp(argv[i], "-sip")) {
            server_ip = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-sport")) {
            server_port = argv[i + 1];
        }
        else {
            cout << "Incorrect arguments!" << endl;
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int c_main(int argc, char* argv[])
{
    struct sockaddr_in server_addr;
    string server_ip;
    string server_port;

    if (argc < ARGUMENTS_COUNT) {
        cout << "Few arguments! Try: " << HELP_MESSAGE << endl;
        exit(EXIT_SUCCESS);
    }

    parse_arguments(argv, server_ip, server_port);

    int res = get_host_addr_by_ip_and_port(server_ip.c_str(), server_port.c_str(), &server_addr);
    if (res == ERROR_IP_NOT_VALID) {
        cout << "Server IP address is not valid" << endl;
        exit(EXIT_FAILURE);
    }

    client cl(server_addr);
    cl.connect_to_server();
    cl.send_request();
    cl.wait_for_response();
    cl.close_connection();

    return 0;
}

