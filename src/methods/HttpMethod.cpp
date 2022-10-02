#include "../methods/GET.hpp"
#include "../methods/DELETE.hpp"

HttpMethod* generateHttpMethod( const std::string& name ) {
    HttpMethod* method;

    if (name == "GET") {
        method = new GET();
    } else if (name == "DELETE") {
        method = new DELETE();
    } else {
        method = NULL;
    }

    return (method);
}

bool isMethodAllowed( const std::string& name, const LocationInfo& location ) {
    std::vector<std::string> allowed_methods = location.allow_methods;

    for (size_t i = 0; i < location.allow_methods.size(); ++i) {
        if (name == allowed_methods[i]) {
            return (true);
        }
    }

    return (false);
}
