#include "../methods/GET.hpp"

HttpMethod* generateHttpMethod( const std::string& name ) {
    HttpMethod* method;

    if (name == "GET") {
        method = new GET();
    } else {
        method = NULL;
    }

    return (method);
}
