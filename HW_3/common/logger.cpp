#include "logger.h"
#include "utils.h"

logger::logger()
    : echo_mode_(false)
{}

logger::logger(const string& author)
    : echo_mode_(false), author_(author)
{}

void logger::set_echo(bool mode)
{
    echo_mode_ = mode;
}

void logger::set_author(const string &author)
{
    author_ = author;
}

int logger::start()
{
    start_tp_ = c_clock::now();

    return 0;
}

int logger::start(time_point start_tp)
{
    start_tp_ = start_tp;

    return 0;
}

int logger::clear()
{
    times_.clear();
    messages_.clear();

    return 0;
}

int logger::log(const char *message)
{
    return log(string(message));

    return 0;
}

int logger::log(const string& message)
{
    return log(c_clock::now(), message);

    return 0;
}

int logger::log(time_point tp, const string& message)
{
    times_.push_back(tp);
    messages_.push_back(message);

    if (echo_mode_) {
        cout << author_ << ": " << message << endl;
    }

    return 0;
}

string logger::create_record(milliseconds& ms, const string& message)
{
    stringstream ss;
    ss << "[ " << ms.count() << " ms ] " << author_ << ": " << message << endl;

    return ss.str();
}

int logger::save()
{
    int version = 0;
    string file_name = author_ + ".log";
    while(is_file_exists(file_name)) {
        file_name = author_ + std::to_string(version) + ".log";
        ++version;
    }

    save(file_name);

    return 0;
}

int logger::save(const string& file_name)
{
    ofstream out_file(file_name, std::ofstream::out | std::ofstream::app);
    if (out_file.fail()) {
        cout << "Logger: Couldn't open a file: " << file_name << endl;
        return -1;
    }

    cout << "Logger: File \"" << file_name << "\" is open" << endl;
    out_file << "[ " << 0 << " ms ] " << "START" << endl;
    for (size_t i = 0; i < messages_.size(); ++i) {
        out_file << "[ "
                 << std::chrono::duration_cast<milliseconds>(times_[i] - start_tp_).count()
                 << " ms ] "
                 << author_ << ": "
                 << messages_[i]
                 << endl;
    }
    out_file << "[ " << std::chrono::duration_cast<milliseconds>(c_clock::now() - start_tp_).count() << " ms ] " << "END" << endl;

    return 0;
}


