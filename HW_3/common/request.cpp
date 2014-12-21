#include "request.h"
#include "utils.h"

request request::generate_request()
{
    size_t elements_count = (size_t)get_random_range(ELEMENTS_MIN_COUNT, ELEMENTS_MAX_COUNT);
    return request(elements_count);
}

request::request()
    : is_valid_(false)
{}

request::request(size_t elements_count)
    : is_valid_(true)
{
    numbers_.resize(elements_count);
    for (size_t i = 0; i < elements_count; ++i) {
        numbers_[i] = get_random_range(ELEMENT_MIN_VALUE, ELEMENT_MAX_VALUE);
    }
}

bool request::operator ==(const request& other)
{
    return is_valid_ == other.is_valid_
            && numbers_ == other.numbers_;
}

bool request::is_valid()
{
    return is_valid_;
}

vint16& request::numbers()
{
    return numbers_;
}

//char* request::serialize(int* out_size)
//{
//    const uint32_t ELEMENTS_COUNT = numbers_.size();
//    const uint32_t BUF_SIZE = ELEMENTS_COUNT * ELEMENT_SIZE;

//    size_t offset = 0;
//    char* buf = new char[HEADER_SIZE + BUF_SIZE];
//    to_bytes<uint32_t>(buf + offset, HEADER_SIZE, ELEMENTS_COUNT); offset += HEADER_SIZE;

//    for (size_t i = 0; i < ELEMENTS_COUNT; ++i) {
//        to_bytes<int16_t>(buf + offset, ELEMENT_SIZE, numbers[i]);
//        offset += ELEMENT_SIZE;
//    }
//    assert(offset == BUF_SIZE);

//    *out_size = BUF_SIZE;
//    return buf;
//}

//int request::deserialize(char *buf, size_t len)
//{
//    size_t offset = 0;
//    uint32_t ELEMENTS_COUNT = from_bytes<uint32_t>(buf, HEADER_SIZE); offset += HEADER_SIZE;
//    if (HEADER_SIZE + ELEMENTS_COUNT * ELEMENT_SIZE != len) {
//        request.is_bad = true;
//        return -1;
//    }

//    for (size_t i = 0; i < ELEMENTS_COUNT; ++i) {
//        numbers_[i] = from_bytes<int16_t>(buf + offset, ELEMENT_SIZE);
//        offset += ELEMENT_SIZE;
//    }

//    return 0;
//}

void request::serialize_to_string(string& out_string)
{
    assert(out_string.size() == 0);

    char buf[4];
    size_t offset = 0;

    //packet_length
    uint32_t packet_length = REQUEST_MIN_LENGTH + numbers_.size() * ELEMENT_SIZE;
    to_bytes<uint32_t>(buf, 4, packet_length);
    out_string += string(buf, 4);
    offset += 4;

    //elements_count
    uint32_t elements_count = numbers_.size();
    to_bytes<uint32_t>(buf, 4, elements_count);
    out_string += string(buf, 4);
    offset += 4;

    //elements
    for (vint16::iterator it = numbers_.begin(); it != numbers_.end(); ++it) {
        to_bytes<int16_t>(buf, ELEMENT_SIZE, *it);
        out_string += string(buf, 2);
        offset += ELEMENT_SIZE;
    }

    assert(offset == packet_length);
}

bool request::deserialize_from_string(const string& in_string)
{
    if (in_string.length() < REQUEST_MIN_LENGTH) {
        this->is_valid_ = false;
        return false;
    }

    const char* buf = in_string.c_str();
    size_t offset = 0;

    //packet_length
    uint32_t packet_length = from_bytes<uint32_t>(buf + offset, 4);
    offset += 4;

    if (packet_length != in_string.length()) {
        this->is_valid_ = false;
        return false;
    }

    //elements_count
    uint32_t elements_count = from_bytes<uint32_t>(buf + offset, 4);
    offset += 4;

    //elements
    numbers_.resize(elements_count);
    for (size_t i = 0; i < elements_count; ++i) {
        int16_t number = from_bytes<int16_t>(buf + offset, ELEMENT_SIZE);
        numbers_[i] = number;
        offset += 2;
    }

    assert(offset == packet_length);
    this->is_valid_ = true;

    return true;
}

void request::print_content()
{
    cout << to_string() << endl;
}

string request::to_string()
{
    stringstream ss;
    ss << "request: " << (is_valid_ ? "" : "not ") << "valid" << endl
       << "elements: ";

    for (vint16::iterator it = numbers_.begin(); it != numbers_.end(); ++it) {
        ss << *it << " ";
    }

    ss << endl;

    return ss.str();
}








