#include "utils.hpp"

size_t stringToSize( const std::string& str ) {
    size_t res = 0;
    std::string::size_type i = 0;

    while (i < str.size() && isdigit(str[i])) {
        res = (res * 10) + str[i] - '0';
        ++i;
    }

    return (res);
}
