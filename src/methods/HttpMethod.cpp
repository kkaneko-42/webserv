#include "../methods/GET.hpp"
#include "../methods/DELETE.hpp"
#include "../methods/POST.hpp"

HttpMethod* generateHttpMethod( const std::string& name ) {
    HttpMethod* method;

    if (name == "GET") {
        method = new GET();
    } else if (name == "DELETE") {
        method = new DELETE();
    } else if (name == "POST") {
        method = new POST();
    } else {
        method = NULL;
    }

    return (method);
}

bool isMethodAllowed( const std::string& name, const LocationInfo& location ) {
    return location.allow_methods.count(name) > 0;
}
