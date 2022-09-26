#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024

static int initServer( const char* ip, int port, struct sockaddr_in* addr );
static int waitConnection( int sock, struct sockaddr_in* addr_cli );
// static void connectToClient( int sock_cli, struct sockaddr_in* addr_cli );
static std::string getRequest( int sock_cli );
static void echoBack( int sock_cli, struct sockaddr_in* addr_cli, const std::string& req );

int main( void ) {
    struct sockaddr_in addr_server, addr_cli;
    int sock = initServer("127.0.0.1", 4242, &addr_server);
    int sock_cli = waitConnection(sock, &addr_cli);
    // connectToClient(sock_cli, &addr_cli);

    while (true) {
        std::string req = getRequest(sock_cli);
        echoBack(sock_cli, &addr_cli, req);
    }

    return (0);
}

static int initServer( const char* ip, int port, struct sockaddr_in* addr ) {
    bzero(addr, sizeof(*addr));

    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    std::cerr << "socket created" << std::endl;

    // binding
    const size_t addr_size = sizeof(*addr);
    bzero(addr, addr_size);
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port); // ポート。ホストからネットワークへのバイトオーダー変換
    addr->sin_addr.s_addr = inet_addr(ip);

    if (bind(sock, (struct sockaddr*)addr, addr_size) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    std::cerr << "addr " << ip << ":" << port << std::endl;

    // listen
    if (listen(sock, 1024) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cerr << "listenning..." << std::endl;

    return (sock);
}

static int waitConnection( int sock, struct sockaddr_in* addr_cli ) {
    socklen_t addr_cli_size = sizeof(addr_cli);
    bzero(&addr_cli, addr_cli_size);

    int sock_cli = accept(
        sock,
        (struct sockaddr*)addr_cli,
        &addr_cli_size
    );
    if (sock_cli == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cerr << "connected!" << std::endl;

    return (sock_cli);
}
/*
static void connectToClient( int sock_cli, struct sockaddr_in* addr_cli ) {
    if (connect(sock_cli,
        (struct sockaddr*)addr_cli,
        sizeof(*addr_cli)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}
*/

static std::string getRequest( int sock_cli ) {
    char buf[BUF_SIZE];
    std::string req;

    read(sock_cli, buf, BUF_SIZE - 1);
    req = buf;

    return (req);
}

static void echoBack( int sock_cli, struct sockaddr_in* addr_cli, const std::string& req ) {
    std::cout << "req:" << req << std::endl;;
    write(sock_cli, req.c_str(), req.size());
}
