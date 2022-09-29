#include <iostream>
#include <vector>
#include <string>

static int splitDirective(
    const std::string& file_content,
    std::vector<std::string>& buffer,
    std::string::size_type pos = 0
) {
    const char directive_begin = '{', directive_end = '}';
    bool is_in_directive = false, is_in_sub_directive = false;
    std::string::size_type begin_pos, end_pos;
    
    for (std::string::size_type i = 0; i < file_content.size(); ++i) {
        if (file_content[i] == directive_end) {
            if (is_in_sub_directive) {
                is_in_sub_directive = false;
            } else {
                buffer.push_back(file_content.substr(begin_pos, i - begin_pos));
                is_in_directive = false;
            }
        }
        
        if (file_content[i] == directive_begin) {
            if (is_in_directive) {
                is_in_sub_directive = true;
            } else {
                is_in_directive = true;
                begin_pos = i + 1;
            }
        }
    }

    if (is_in_directive || is_in_sub_directive) {
        return (1);
    } else {
        return (0);
    }
}

int main() {
    std::string content =
"server {\n"
"    listen  0.0.0.0:4242;\n"
"    server_name     example.com;\n"
"    client_max_body_size 42;\n"
"\n"
"    error_page 404 /404.html;\n"
"\n"
"    location / {\n"
"        (必須)root ./html;\n"
"        allow_methods GET POST DELETE;\n"
"        return https://www.google.com/;\n"
"        autoindex on;\n"
"        index index.html;\n"
"        allow_file_upload off;\n"
"        save_folder ./upload;\n"
"\n"
"        allow_cgi_extensions py sh;\n"
"    }\n"
"\n"
"    location /hoge/ {\n"
"        root ./html/hoge\n"
"        index index.html\n"
"    }\n"
"}\n"
"\n"
"server {\n"
"    listen  0.0.0.0:4242;\n"
"    server_name     example.com;\n"
"    client_max_body_size 42;\n"
"\n"
"    error_page 404 /404.html;\n"
"\n"
"    location / {\n"
"        (必須)root ./html;\n"
"        allow_methods GET POST DELETE;\n"
"        return https://www.google.com/;\n"
"        autoindex on;\n"
"        index index.html;\n"
"        allow_file_upload off;\n"
"        save_folder ./upload;\n"
"\n"
"        allow_cgi_extensions py sh;\n"
"    }\n"
"\n"
"    location /hoge/ {\n"
"        root ./html/hoge\n"
"        index index.html\n"
"    }\n"
"}\n";

    std::vector<std::string> buf;
    int ret = splitDirective(content, buf);

    std::cout << "ret: " << ret << std::endl;
    for (size_t i = 0; i < buf.size(); ++i) {
        std::cout << "[" << i << "] @" << buf[i] << "@" << std::endl;
    }
    return (0);
}
