#include "utils.hpp"
#include <dirent.h>
#include <algorithm>

std::vector<std::string> getFileList( const std::string& dir_path ) {
    DIR* dir = NULL;
    struct dirent* read_dir = NULL;
    std::vector<std::string> res;
    std::string filename;

    dir = opendir(dir_path.c_str());
    if (dir == NULL) {
        perror("opendir");
        return res;
    }

    while (true) {
        read_dir = readdir(dir);
        if (read_dir == NULL) {
            break;
        }
        filename = read_dir->d_name;
#ifdef _DIRENT_HAVE_D_TYPE
        if (read_dir->d_type == DT_DIR) {
            filename += "/";
        }
#endif
        res.push_back(filename);
    }

    std::sort(res.begin(), res.end());
    return res;
}
