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
