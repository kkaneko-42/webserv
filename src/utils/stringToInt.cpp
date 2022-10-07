#include "utils.hpp"

int stringToInt( const std::string& str ) {
    int sgn = 1;
    unsigned int res = 0;
    std::string::size_type i = 0;

    if (str.size() == 0) {
        return 0;
    }

    if (str[0] == '+' || str[0] == '-') {
        sgn = ',' - str[0];
        ++i;
    }
    while (i < str.size() && isdigit(str[i])) {
        res = (res * 10) + str[i] - '0';
        ++i;
    }

    return (sgn * static_cast<int>(res));
}
/*
#include <iostream>
int main() {
    std::cout << stringToInt("-2147483648") << std::endl;
    return (0);
}
*/
