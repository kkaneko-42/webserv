#include "utils.hpp"

bool pathIsDir( const std::string& path ) {
    if (path.size() == 0) {
        return false;
    }

    return (path[path.size() - 1] == '/');
}
