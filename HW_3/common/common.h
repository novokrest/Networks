#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <chrono>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <cassert>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;
using std::stringstream;
using std::fstream;
using std::ifstream;
using std::ofstream;

typedef unsigned char uchar;
typedef std::vector<int> vint;
typedef std::vector<int16_t> vint16;
typedef std::vector<int32_t> vint32;
typedef std::vector<char> vchar;
typedef std::vector<string> vstring;
typedef std::vector<pid_t> vpid_t;

typedef std::chrono::system_clock c_clock;
typedef std::chrono::system_clock::time_point time_point;
typedef std::chrono::milliseconds milliseconds;

typedef std::vector<time_point> vtime_point;

#endif // COMMON_H

