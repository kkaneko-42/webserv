#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <algorithm>

int loadFile( const std::string& path, std::string& content );
bool pathIsDir( const std::string& path );
std::vector<std::string> getFileList( const std::string& dir_path );
std::vector<std::string> splitString( const std::string &str, const std::string& delim );
std::string toLowerString( const std::string& str );
std::string trimString( const std::string& str );
std::string sizeToString( size_t n );
int stringToInt( const std::string& str );
size_t stringToSize( const std::string& str );

#endif
