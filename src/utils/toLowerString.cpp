#include "utils.hpp"

std::string toLowerString( const std::string& str ) {
    std::string ret = "";
    for (size_t i = 0; i < str.size(); ++i) {
        ret += std::tolower(str[i]);
    }
    return ret;
}
