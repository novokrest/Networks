#ifndef RESPONSE
#define RESPONSE

#include "common.h"

static const int RESPONSE_MIN_LENGTH = 10;
static const int RESPONSE_MAX_LENGTH = 305;

static const int STATUS_OK = 0;
static const int STATUS_BAD_REQUEST = 1;

/**
 * Response packet has following structure:
 *
 * [ <packet_length> ]-[ <author_name_length> ]-[ <author_name> ]-[ <status> ]-[ <results> ]
 * [     4 bytes     ]-[       1 bytes        ]-[ 0..255 bytes  ]-[  1 byte  ]-[  4 bytes  ]
 *
 */

struct response
{
    static response create_ok(const string& author, int32_t result);
    static response create_bad_request(const string& author);

    response();
    response(const string& author, uint8_t status, int32_t result);

    bool operator==(const response& other);

    uint32_t length();
    bool is_valid();
    const string& author();
    uint8_t status();
    int32_t result();

    void serialize_to_string(string& out_string);
    bool deserialize_from_string(const string& in_string);

    string to_string();

private:
    uint32_t length_;

    bool is_valid_;
    string author_;
    uint8_t status_;
    int32_t result_;
};

#endif // RESPONSE

