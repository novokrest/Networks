#ifndef REQUEST
#define REQUEST

#include "common.h"

static const int REQUEST_MAX_LENGTH = 1000000;
static const int REQUEST_MAX_VALUE =  100000;
static const int REQUEST_MIN_VALUE = -100000;


class request
{
    vint32 numbers_;

public:
    static request make_request();

    request(int count);

    char* serialize();
    static request deserialize(char* buf);
};

#endif // REQUEST

