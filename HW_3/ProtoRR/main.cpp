#include <iostream>
#include "request.pb.h"

using namespace std;

template <typename S>
void to_bytes(char* buf, size_t len, S value)
{
    size_t svalue = (size_t)value;
    for (int i = 0; i < len; ++i) {
        value
    }
}

template <typename R>
R from_bytes(char* buf, size_t len)
{}


int main()
{
//    networks::Request request;
//    request.set_count(10);
//    request.add_elements(1);
//    request.add_elements(2);
//    request.add_elements(3);

//    string serialized_request;
//    request.SerializeToString(&serialized_request);

//    networks::Request drequest;
//    drequest.ParseFromString(serialized_request);

//    cout << drequest.elements_size() << endl;


    cout << "Hello World!" << endl;
    return 0;
}


