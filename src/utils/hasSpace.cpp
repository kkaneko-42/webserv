#include <string>

bool hasSpace( const std::string& str ) {
    for (std::string::size_type i = 0; i < str.size(); ++i) {
        if (isspace(str[i])) {
            return (true);
        }
    }

    return (false);
}
