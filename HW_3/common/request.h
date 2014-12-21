#ifndef REQUEST
#define REQUEST

#include "common.h"

static const int REQUEST_MIN_LENGTH = 8;
static const int REQUEST_MAX_LENGTH = 20008;

/**
 * Request has following structure:
 *
 * [ <packet_length> ]-[ <elements_count> ]-[          <elements>          ]
 * [     4 bytes     ]-[     4 bytes      ]-[ 0..10000 of 2-bytes elements ]
 *
 */

struct request
{
    static const size_t  ELEMENT_SIZE       = 2;
    static const int16_t ELEMENT_MIN_VALUE  = 32767;
    static const int16_t ELEMENT_MAX_VALUE  = -32767;
    static const size_t  ELEMENTS_MIN_COUNT = 1;
    static const size_t  ELEMENTS_MAX_COUNT = (REQUEST_MAX_LENGTH - 8) / 2;

    static request generate_request();

    request();
    request(size_t elements_count);
    bool operator==(const request& other);

    bool is_valid();
    vint16& numbers();

//    char* serialize(int* out_size);
//    int deserialize(char* buf, size_t len);

    void serialize_to_string(string& out_string);
    bool deserialize_from_string(const string& in_string);

    void print_content();
    string to_string();

private:
    bool is_valid_;
    vint16 numbers_;
};

#endif // REQUEST

