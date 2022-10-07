#include "utils.hpp"

static void convertRecursive( size_t n, std::string& res ) {
    if (n < 10) {
        res += n + '0';
        return;
    }
    convertRecursive(n / 10, res);
    convertRecursive(n % 10, res);
}

std::string sizeToString( size_t n ) {
    std::string res = "";
    convertRecursive(n, res);
    return (res);
}
/*
#include <iostream>
int main() {
    std::cout << sizeToString(static_cast<size_t>(-1)) << std::endl;
    return (0);
}
*/
