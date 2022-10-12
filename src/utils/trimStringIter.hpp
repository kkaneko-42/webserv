#ifndef TRIM_STRING_ITER_HPP
#define TRIM_STRING_ITER_HPP

#include <string>

std::string trimString( const std::string& str );

template <typename T>
T trimStringIter( const T& arr ) {
    typename T::iterator it;
    T res(arr);

    for (it = res.begin(); it != res.end(); ++it) {
        *it = trimString(*it);
    }

    return res;
}

#endif
