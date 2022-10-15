#include "utils.hpp"

size_t stringToSize( const std::string& str ) {
    size_t res = 0;
    std::string::size_type i = 0;

    while (i < str.size() && isdigit(str[i])) {
        size_t num = (res * 10) + str[i] - '0';
        if (num / 10 != res) {
            throw std::overflow_error("stringToSize");
        } else {
            res = num;
        }
        ++i;
    }

    return (res);
}
