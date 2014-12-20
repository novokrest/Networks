#include "request.h"
#include "utils.h"

request request::make_request()
{}

request::request(int count)
{
    numbers_.resize(count);
    for (int i = 0; i < count; ++i) {
        numbers_[i] = get_random_range(REQUEST_MIN_VALUE, REQUEST_MAX_VALUE);
    }
}

char* request::serialize()
{

}

request request::deserialize(char *buf)
{
    return request(0);
}
