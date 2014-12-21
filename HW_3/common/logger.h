#ifndef LOGGER
#define LOGGER

#include "common.h"

class logger
{
    bool echo_mode_;

    string author_;

    time_point start_tp_;
    stringstream log_;
    string out_file_;

    vtime_point times_;
    vstring messages_;

public:
    logger();
    logger(const string& author);

    void set_echo(bool mode);
    void set_author(const string& author);

    int start();
    int start(time_point start_tp);

    int clear();

    string create_record(milliseconds& ms, const string& message);

    int log(const char* message);
    int log(const string& message);
    int log(time_point tp, const string& message);

    int save();
    int save(const string& out_file);
};

#endif // LOGGER

