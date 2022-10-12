#include <string>

bool hasCharset( const std::string& str, const std::string& charset ) {
    for (std::string::size_type i = 0; i < str.size(); ++i) {
        if (charset.find(str[i]) != std::string::npos) {
            return (true);
        }
    }

    return (false);
}
