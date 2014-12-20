int c_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int c_argc = 5;
    char* c_argv[] = {"client", "-sip", "192.168.1.2", "-sport", "7777"};

    c_main(c_argc, c_argv);

    return 0;
}
