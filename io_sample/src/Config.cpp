#include "Config.hpp"
#include <fstream>

static int loadFile( const std::string& filepath, std::string& content );
static int splitDirective(
    const std::string& file_content,
    std::vector<std::string>& buffer,
    std::string::size_type pos = 0
);
static std::vector<std::string> splitString( std::string &str, std::string delim );

int Config::parse( const std::string& filepath ) {
    // load config file
    std::string file_content;
    if (loadFile(filepath, file_content)) {
        return (parseError(1));
    }

    // split directive
    std::vector<std::string> server_directives;
    if (splitDirective(file_content, server_directives)) {
        return (parseError(1));
    }

    // create servers info
    for (size_t i = 0; i < server_directives.size(); ++i) {
        if (addServerInfo(server_directives[i])) {
            return (parseError(1));
        }
    }

    return (0);
}

int Config::addServerInfo( const std::string& directive ) {
    ServerInfo info;
    std::map<std::string, std::string> conf_named;

    if (parseDirective(directive, conf_named)) {
        return (1);
    }

    std::map<std::string, std::string>::iterator it;
    for (it = tokens.begin(); it < tokens.end(); ++it) {
    }

    serversInfo.push_back(info);
}

int Config::parseError( int status ) {
    std::cerr << "parse error" << std::endl;
    serversInfo.clear();
    return (status);
}

static int loadFile( const std::string& filepath, std::string& content ) {
    std::string buf;
    std::ifstream ifs(filepath);

    if (!ifs) {
        return (1);
    }

    content = "";
    while (!ifs.eof()) {
        std::getline(ifs, buf);
        content += buf + "\n";
    }

    return (0);
}

static int parseDirective(
    const std::string& directive,
    std::map<std::string, std::string> conf_named
) {

}

// input: config file content
// output: array of directives content
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
                begin_pos = i + 1; // NOTE: possibly segv
            }
        }
    }

    if (is_in_directive || is_in_sub_directive) {
        return (1);
    } else {
        return (0);
    }
}
