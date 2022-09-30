#include "HttpRequest.hpp"
#include <iostream>

int main( void ) {
    std::string request_eg =
    "GET / HTTP/1.1\r\n"
    "Accept: image/gif, image/jpeg, */*\r\n"
    "Accept-Language: ja\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "User-Agent: Mozilla/4.0 (Compatible; MSIE 6.0; Windows NT 5.1;)\r\n"
    "Host: www.xxx.zzz\r\n"
    "Connection: Keep-Alive\r\n"
    "\r\n"
    "hogehoge\r\n"
    "fugafuga\r\n"
    "\r\n"
    "piyopiyo\r\n";

    try {
        HttpRequest req(request_eg);
        req.printRequest();
    } catch (...) {
        std::cout << "fail" << std::endl;
    }

    return (0);
}
