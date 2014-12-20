#include "common.h"
#include "server.h"


int s_main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Few arguments! Try: server <port>" << endl;
        exit(EXIT_SUCCESS);
    }

    unsigned short int port = atoi(argv[1]);
    server serv(port);

    if (serv.bind() < 0) {
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}
