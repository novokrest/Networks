#include "response.h"
#include "utils.h"

response response::create_ok(const string& author, int32_t result)
{
    return response(author, STATUS_OK, result);
}

response response::create_bad_request(const string& author)
{
    return response(author, STATUS_BAD_REQUEST, 0);
}

response::response()
    : is_valid_(false)
{}

response::response(const string &author, uint8_t status, int32_t result)
    : is_valid_(true), author_(author), status_(status), result_(result)
{}

bool response::operator ==(const response& other)
{
    return is_valid_ == other.is_valid_
            && author_ == other.author_
            && status_ == other.status_
            && result_ == other.result_;
}

uint32_t response::length()
{
    return length_;
}

bool response::is_valid()
{
    return is_valid_;
}

const string& response::author()
{
    return author_;
}

uint8_t response::status()
{
    return status_;
}

int32_t response::result()
{
    return result_;
}

void response::serialize_to_string(string& out_string)
{
    assert(out_string.size() == 0);

    char buf[4];
    size_t offset = 0;

    //packet_length
    uint32_t packet_length = RESPONSE_MIN_LENGTH + author_.size();
    to_bytes<uint32_t>(buf, 4, packet_length);
    out_string += string(buf, 4);
    offset += 4;

    //author_name_length
    out_string += (uint8_t)(author_.length());
    offset += 1;

    //author
    out_string += author_;
    offset += author_.length();

    //status
    out_string += (uint8_t)status_;
    offset += 1;

    //result
    to_bytes<int32_t>(buf, 4, result_);
    out_string += string(buf, 4);
    offset += 4;

    assert(out_string.length() == offset);
}

bool response::deserialize_from_string(const string& in_string)
{
    if (in_string.length() < RESPONSE_MIN_LENGTH) {
        this->is_valid_ = false;
        return false;
    }

    const char* buf = in_string.c_str();
    size_t offset = 0;

    //packet_length
    uint32_t packet_length = from_bytes<uint32_t>(buf + offset, 4);
    offset += 4;

    if (packet_length != in_string.size()) {
        this->is_valid_ = false;
        return false;
    }

    //author
    uint8_t author_name_length = from_bytes<uint8_t>(buf + offset, 1);
    offset += 1;
    this->author_ = in_string.substr(offset, author_name_length);
    offset += author_name_length;

    //status
    this->status_ = from_bytes<uint8_t>(buf + offset, 1);
    offset += 1;

    //result
    this->result_ = from_bytes<int32_t>(buf + offset, 4);
    offset += 4;

    assert(offset == in_string.length());
    this->is_valid_ = true;

    return true;
}

string response::to_string()
{
    stringstream ss;
    ss << "response: " << (is_valid_ ? "" : "not ") << "vaild" << endl
       << "author " << author_
       << ", status " << status_
       << ", result " << result_ << endl;

    return ss.str();
}

