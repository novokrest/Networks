#include "common.h"
#include "server.h"


int s_main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Few arguments! Try: server <port>" << endl;
        exit(EXIT_SUCCESS);
    }

    cout << "Start server" << endl;
    unsigned short int port = atoi(argv[1]);
    server serv(port);
    serv.listen();

    return 0;
}
