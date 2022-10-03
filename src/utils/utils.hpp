#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

int loadFile( const std::string& path, std::string& content );
bool pathIsDir( const std::string& path );
std::vector<std::string> getFileList( const std::string& dir_path );

#endif
