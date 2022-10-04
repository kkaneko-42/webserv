#ifndef CGI_EXECUTOR_HPP
#define CGI_EXECUTOR_HPP
#define BUF_SIZE 1024

#include "../io/HttpRequest.hpp"
#include "../io/HttpResponse.hpp"
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <cerrno>
#include <limits>
#include <sys/types.h>
#include <sys/wait.h>

// localhost:4242/a.py/b.py?value=42

// ./html/a.py
// /b.py (EXPORT PATH_INFO)
// value=42 (EXPORT QUERY_STRING)
// is_executable

class CgiExecutor {
    public:
        CgiExecutor( void );
        void init( const HttpRequest& req );
        HttpResponse execute( const HttpRequest& req ) const;
        bool isExecutable( void ) const { return is_executable_; }
        void debugCgiExecutor( void );

    private:
        std::string exec_path_;
        std::string path_info_;
        std::string query_string_;
        bool is_executable_;
};

extern char** environ;

#endif