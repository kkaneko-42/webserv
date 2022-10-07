#include "CgiExecutor.hpp"

CgiExecutor::CgiExecutor( void )
: exec_path_(""), path_info_(""),
query_string_(""), is_executable_(false) {}

// localhost:4242/hoge/a.py/b.py
// location /hoge/ {
//     root ./html/fuga
// }
// location /c.py/ {
//     root ./html;
// }

// getPath() => ./html/a.py/b.py?value=42
// getRawPath() => /hoge/a.py/

// path_ == ./html/a.py
// path_info == /b.py (EXPORT PATH_INFO)
// query_string == value=42 (EXPORT QUERY_STRING)
// is_executable == true

void CgiExecutor::init( const HttpRequest& req ) {
    const LocationInfo location = req.getLocationInfo();
    std::string req_target = req.getRawPath().substr(location.location_path.size());

    std::string::size_type min_pos = std::numeric_limits<std::string::size_type>::max();
    std::string extention;
    for (size_t i = 0; i < location.allow_cgi_extensions.size(); ++i) {
        std::string allow = location.allow_cgi_extensions[i];
        std::string::size_type pos = req_target.find(allow);
        if (pos == std::string::npos) {
            continue;
        }
        if (min_pos > pos) {
            min_pos = pos;
            extention = allow;
        }
    }
    if (min_pos == std::numeric_limits<std::string::size_type>::max()) {
        is_executable_ = false;
        debugCgiExecutor();
        return;
    }
    is_executable_ = true;

    // /a.py/b.py?value=42

    // /a.py
    std::string before = req_target.substr(0, min_pos + extention.size());
    // /b.py?value=42
    std::string after = req_target.substr(before.size());

    // exec_path_ == ./html/a.py
    exec_path_ = location.root + before;

    // path_info_ == /b.py (EXPORT as PATH_INFO)
    // query_string_ == value=42 (EXPORT as QUERY_STRING)
    // NOTE: validate query string
    std::string::size_type query_pos = after.find("?");
    if (query_pos == std::string::npos) {
        path_info_ = after;
    } else {
        path_info_ = after.substr(0, query_pos);
        query_string_  = after.substr(query_pos + 1);
    }

    debugCgiExecutor();
}

HttpResponse CgiExecutor::execute( const HttpRequest& req ) const {
    const ServerInfo host_info = req.getHostInfo();
    int fd_cgi_to_server[2];
    int fd_server_to_cgi[2];

    if ((pipe(fd_cgi_to_server) < 0) || (pipe(fd_server_to_cgi) < 0)) {
        perror("pipe");
        return HttpResponse::createErrorResponse(
            HttpResponse::INTERNAL_SERVER_ERROR,
            host_info
        );
    }

    // export env
    setenv("PATH_INFO", path_info_.c_str(), 1);
    setenv("QUERY_STRING", query_string_.c_str(), 1);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return HttpResponse::createErrorResponse(
            HttpResponse::INTERNAL_SERVER_ERROR,
            host_info
        );
    } else if (pid == 0) {
        // CGI
        close(fd_cgi_to_server[0]);
        close(fd_server_to_cgi[1]);
        if (dup2(fd_cgi_to_server[1], STDOUT_FILENO) < 0
            || dup2(fd_server_to_cgi[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            _exit(1);
        }
        char *args[] = {NULL};
        if (execve(exec_path_.c_str(), args, environ)) {
            perror("execve");
            _exit(1);
        }
    }

    // server
    close(fd_cgi_to_server[1]);
    close(fd_server_to_cgi[0]);

    // write
    std::string body = req.getBody();
    write(fd_server_to_cgi[1], body.c_str(), body.size());
    close(fd_server_to_cgi[1]);

    int status_ptr;
    waitpid(pid, &status_ptr, 0);

    int status = WEXITSTATUS(status_ptr);
    if (status != 0) {
        return HttpResponse::createErrorResponse(
            HttpResponse::BAD_REQUEST,
            host_info
        );
    }

    std::string resp_str;
    char buf[BUF_SIZE];
    for (;;) {
        ssize_t read_size = read(fd_cgi_to_server[0], buf, BUF_SIZE - 1);
        if (read_size < 0) {
            return HttpResponse::createErrorResponse(
                HttpResponse::BAD_REQUEST,
                host_info
            );
        }
        if (read_size == 0) {
            break;
        }
        buf[read_size] = '\0';
        resp_str += buf;
    }
    close(fd_cgi_to_server[0]);
    
    std::cout << "====== cgi log ===============" << std::endl;
    std::cout << "resp_str.size(): " << resp_str.size() << std::endl;
    std::cout << resp_str << std::endl;
    std::cout << "==============================" << std::endl;

    return HttpResponse::unmarshal(resp_str);
}

void CgiExecutor::debugCgiExecutor( void ) {
    std::cout << "====== CGI Executor Infomation ======" << std::endl;
    std::cout << "exec_path: " << exec_path_ << std::endl;
    std::cout << "path_info: " << path_info_ << std::endl;
    std::cout << "query_string: " << query_string_ << std::endl;
    std::cout << "is_executable: " << (is_executable_ ? "true" : "false") << std::endl;
    std::cout << "=====================================" << std::endl;
}
