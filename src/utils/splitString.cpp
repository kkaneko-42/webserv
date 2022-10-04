#include "utils.hpp"

std::vector<std::string> splitString(const std::string &str, const std::string& delim) {
    std::vector<std::string> vec;

    std::string::size_type prev = 0;
    while (true) {
        std::string::size_type pos = str.find(delim, prev);
        if (pos == std::string::npos)
            break;
        std::string sub = str.substr(prev, pos - prev);
        vec.push_back(sub);
        prev = pos + delim.size();
    }
    vec.push_back(str.substr(prev));
    return vec;
}
