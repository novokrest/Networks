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

#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::string;

typedef std::vector<int> vint;
typedef std::vector<int32_t> vint32;
typedef std::vector<char> vchar;
typedef std::vector<pid_t> vpid_t;

#endif // COMMON_H

