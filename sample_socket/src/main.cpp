#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024

// headerは適当に
std::string createResponseHeader( void ) {
    return ("HTTP/1.1 200 OK\n\
Date: Sun, 11 Jan 2004 16:06:23 GMT\n\
Server: Webserv/1.3.22 (Unix) (Red-Hat/Linux)\n\
Last-Modified: Sun, 07 Dec 2003 12:34:18 GMT\n\
ETag: \"1dba6-131b-3fd31e4a\"\n\
Accept-Ranges: bytes\n\
Content-Length: 4891\n\
Keep-Alive: timeout=15, max=100\n\
Connection: Keep-Alive\n\
Content-Type: text/html\n\
");
}

// index.htmlをstd::stringに変換
std::string loadHtml( const char* path ) {
    std::string res = "", buf = "";
    std::ifstream ifs(path);
    if (!ifs) {
        std::exit(1);
    }

    while (!ifs.eof()) {
        std::getline(ifs, buf);
        res += buf + "\n";
    }

    return (res);
}

// httpレスポンスに整形
std::string createResponse() {
    return (
        createResponseHeader()
        + "\n"
        + "<!DOCTYPE html>\n"
        + loadHtml("./html/index.html")
    );
}

int main( void ) {
    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }
    std::cerr << "socket created" << std::endl;

    // binding
    int port = 4242;
    struct sockaddr_in addr;
    const size_t addr_size = sizeof(addr);
    bzero(&addr, addr_size);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // ポート。ホストからネットワークへのバイトオーダー変換
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip address(localhost) 

    if (bind(sock, (struct sockaddr*)&addr, addr_size) == -1) {
        perror("bind");
        exit(1);
    }
    std::cerr << "binding" << std::endl;
    std::cerr << "addr:" << std::hex << addr.sin_addr.s_addr << std::endl;

    // listen
    if (listen(sock, 1024) == -1) {
        perror("listen");
        exit(1);
    }
    std::cerr << "listenning..." << std::endl;

    // accept
    struct sockaddr_in acc_addr;
    socklen_t acc_addr_size = sizeof(acc_addr);
    bzero(&acc_addr, acc_addr_size);
    int sock_cli = accept(
        sock,
        (struct sockaddr*)&acc_addr,
        &acc_addr_size
    );
    std::cerr << "accepted!!" << std::endl;

    // send response
    std::string response = createResponse();
    if (write(sock_cli, response.c_str(), response.size()) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    std::cerr << "response sent" << std::endl;
    
    // close sockets
    close(sock_cli);
    close(sock);

    return (0);
}
