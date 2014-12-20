#ifndef REQUEST
#define REQUEST

#include <vector>

static const int REQUEST_MAX_LENGTH = 1000000;
static const int REQUEST_MAX_VALUE =  100000;
static const int REQUEST_MIN_VALUE = -100000;

class request
{
    typedef std::vector<int32_t> vint;
    vint numbers_;

public:
    static request make_request();

    request(int count);

    void serialize(char* buf);
    static request deserialize(char* buf);
};

#endif // REQUEST

