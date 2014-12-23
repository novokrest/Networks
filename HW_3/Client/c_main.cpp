#include "common.h"
#include "client.h"
#include "request.h"
#include "utils.h"
#include "errors.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

static const int ARGUMENTS_COUNT = 9;
static const char* HELP_MESSAGE = "client [-n <name>] [-sip <server_ip>] [-sport <server_port>] [-rs <requests_count>]";

static int parse_arguments(char* argv[], string& name, string& server_ip, string& server_port, size_t* request_size)
{
    for (int i = 1; i < ARGUMENTS_COUNT; i += 2) {
        if (!strcmp(argv[i], "-n")) {
            name = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-sip")) {
            server_ip = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-sport")) {
            server_port = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-rs")) {
            *request_size = atoi(argv[i + 1]);
        }
        else {
            cout << "Incorrect arguments!" << endl;
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    init_random();

    string name;
    string server_ip;
    string server_port;
    size_t request_size = 0;

    if (argc < ARGUMENTS_COUNT) {
        cout << "Few arguments! Try: " << HELP_MESSAGE << endl;
        exit(EXIT_SUCCESS);
    }

    parse_arguments(argv, name, server_ip, server_port, &request_size);

    struct sockaddr_in server_addr;
    int res = get_host_addr_by_ip_and_port(server_ip.c_str(), server_port.c_str(), &server_addr);
    if (res == ERROR_IP_NOT_VALID) {
        cout << "Server IP address is not valid" << endl;
        exit(EXIT_FAILURE);
    }

    client cl(name, server_addr);
    cl.start(request_size);

    return 0;
}

