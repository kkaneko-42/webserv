#include "utils.hpp"

std::string trimString(const std::string& str) {
    if (str.size() == 0) {
        return str;
    }
    
    std::string::size_type front, back;
    front = 0;
    back = str.size() - 1;
    while ((str[front] == 0x20 || str[front] == 0x09) && front < back) {
        front++;
    }
    while ((str[front] == 0x20 || str[front] == 0x09) && front < back) {
        back--;
    }

    return str.substr(front, back + 1);
}
