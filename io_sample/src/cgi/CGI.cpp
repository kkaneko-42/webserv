if (/* reqがCGIの起動を意図したものであるとき */) {
    exe_path = 
    path_info = 
    putenv(("PATH_INFO=" + path_info).c_str());
    executeCGI(exe_path); // 拡張子で分岐(.pyなら、std::system("python " + exe_path))
}

int executeCGI( const std::string& exe_path ) {
    std::string::size_type ext_pos = exe_path.rfind(".");
    std::string extension = exe_path.substr(ext_pos);
    std::string cmd;

    if (extension == ".py") {
        cmd = "python " + exe_path;
    } else if (extension == ".sh") {
        cmd = "sh " + exe_path;
    }

    return (std::system(cmd));
}

