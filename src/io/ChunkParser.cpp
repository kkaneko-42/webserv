#include "ChunkParser.hpp"

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

std::string ChunckParser::parse( const std::string& chunked ) {
    const std::vector<std::string> tokens = splitString(chunked, "\r\n\r\n");
    std::string res;

    std::vector<std::string>::iterator it = tokens.begin();
    while (it != tokens.end()) {
        ssize_t chunk_size = chunk_size(it);
        if (chunk_size < 0) {
            break;
        } else if (chunk_size == 0) {
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
        if (chunk_data(it, chunk_size, res)) {
            break;
        }
    }

    throw std::logic_error("parse error");
}

ssize_t ChunkParser::chunk_size( iterator& it ) {
    return strToHex(*(it++));
}

int ChunkParser::chunk_data( iterator& it, ssize_t chunk_size, std::string& res ) {
    if (*it.size() != chunk_size) {
        return 1;
    }

    res += *it;
    ++it;
    return 0;
}
