#include "common.h"

int c_main(int argc, char* argv[]);
//int s_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    cout << "START" << endl;

    int c_argc = 7;
    char* c_argv[] = {"client", "-n", "Boss", "-sip", "192.168.1.2", "-sport", "7778"};
    c_main(c_argc, c_argv);

    int s_argc = 2;
    char* s_argv[] = {"server", "7778"};
    //s_main(s_argc, s_argv);

    cout << "END" << endl;

    return 0;
}
