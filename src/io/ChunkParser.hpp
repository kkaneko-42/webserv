#ifndef CHUNK_PARSER_HPP
#define CHUNK_PARSER_HPP

#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include "../utils/utils.hpp"

class ChunkParser {
    public:
        typedef std::vector<std::string>::const_iterator iterator;
        
        static const ssize_t MAX_CHUNK_SIZE = 0xffff;
        static std::string parse( const std::string& chunked );
        
    private:
        static ssize_t chunk_size( iterator& it );
        static int chunk_data( iterator& it, ssize_t chunk_size, std::string& res );
};

#endif
