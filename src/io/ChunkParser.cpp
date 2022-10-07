#include "ChunkParser.hpp"

template <typename T>
static void printVector(std::vector<T> v) {
    std::cout << "{" << std::endl;;
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << "  [" << i << "] " << v[i] << std::endl;
    }
    std::cout << "}" << std::endl;
}

static bool isHexString( const std::string& str ) {
    const std::string lower_str = toLowerString(str);

    for (size_t i = 0; i < lower_str.size(); ++i) {
        if (!isdigit(lower_str[i])
        && !('a' <= lower_str[i] && lower_str[i] <= 'f')) {
            return false;
        }
    }

    return true;
}

static ssize_t strToHex( const std::string& str ) {
    const std::string lower_str = toLowerString(str);
    ssize_t res = 0;

    if (!isHexString(str)) {
        return -1;
    }

    for (size_t i = 0; i < lower_str.size(); ++i) {
        if (isdigit(lower_str[i])) {
            res = (res * 10) + lower_str[i] - '0';
        } else {
            res = (res * 10) + lower_str[i] - 'a' + 10;
        }
        if (res > ChunkParser::MAX_CHUNK_SIZE) {
            return -1;
        }
    }

    return (res);
}

/*
c\r\n
hello, world\r\n
0\r\n
\r\n

=> ["c", "hello, world", "0", "", ""]
*/
std::string ChunkParser::parse( const std::string& chunked ) {
    const std::vector<std::string> tokens = splitString(chunked, "\r\n");
    std::cout << "tokens: " << std::endl;
    printVector(tokens);
    std::string res;

    std::vector<std::string>::const_iterator it = tokens.begin();
    while (it != tokens.end()) {
        ssize_t size = chunk_size(it);
        if (size < 0) {
            break;
        } else if (size == 0) {
            if (it == tokens.end() || *it != "") {
                break;
            }
            ++it;
            if (it == tokens.end() || *it != "") {
                break;
            }
            return res;
        }

        if (it == tokens.end()) {
            break;
        }
        if (chunk_data(it, size, res)) {
            break;
        }
    }

    throw std::logic_error("parse error");
}

ssize_t ChunkParser::chunk_size( iterator& it ) {
    return strToHex(*(it++));
}

int ChunkParser::chunk_data( iterator& it, ssize_t chunk_size, std::string& res ) {
    if (it->size() != chunk_size) {
        return 1;
    }

    res += *it;
    ++it;
    return 0;
}
