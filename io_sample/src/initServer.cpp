#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

static int createSocket( void );
static void bindSocket( int sock, const char* ip, int port );

int initServer( const char* ip, int port ) {

    int sock = createSocket();
    bindSocket(sock, ip, port);

    // listen
    if (listen(sock, 1024) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cerr << "listenning..." << std::endl;

    return (sock);
}

// create socket
static int createSocket( void ) {
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    std::cerr << "socket created" << std::endl;

    return (sock);
}

// binding
static void bindSocket( int sock, const char* ip, int port ) {
    struct sockaddr_in addr;
    const size_t addr_size = sizeof(addr);
    bzero(&addr, addr_size);

    addr.sin_family = AF_INET; //IPv4 internet protocol
    addr.sin_port = htons(port); // ポート。ホストからネットワークへのバイトオーダー変換
    addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(sock, (struct sockaddr*)&addr, addr_size) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    std::cerr << "addr " << ip << ":" << port << std::endl;
}
