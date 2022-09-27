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
#define MAX_POLL_FDS 200

void addSocketDiscriptor( int new_sd, short events, struct pollfd* fds, nfds_t* size );
int initServer( const char* ip, int port );
static void handleEvents( struct pollfd* fds, nfds_t* current_size );
static void acceptIncomingConnections( int listen_sd, struct pollfd* fds, nfds_t* size );
static void echoBack( int client_sd, char* body );

int main( void ) {
    struct pollfd fds[MAX_POLL_FDS];
    nfds_t current_size = 0;
    int listen_sd = initServer("127.0.0.1", 4242); // Socket Discriptor(sd)

    // listen_sdを、non-blockingなfdにする
    // これをしないと、acceptIncomingConnections()ができない
    // なぜなら、待機キューが空の時のacceptが接続を待機してしまう(所謂blocking??)
    fcntl(listen_sd, F_SETFL, O_NONBLOCK);

    // init fds
    bzero(fds, sizeof(fds));
    addSocketDiscriptor(listen_sd, POLLIN, fds, &current_size);

    // set timeout[ms]
    int timeout = 3 * 60 * 1000;

    while (true) {
        std::cout << "wait poll()..." << std::endl;
        int rc = poll(fds, current_size, timeout); // イベント待機

        if (rc == -1) {
            // error
            perror("poll");
            exit(EXIT_FAILURE);
        } else if (rc == 0) {
            // timeout
            std::cout << "timeout" << std::endl;
            break;
        } else {
            // rcはイベントが発生したfdの数
            handleEvents(fds, &current_size);
        }
    }

    return (0);
}

void addSocketDiscriptor( int new_sd, short events, struct pollfd* fds, nfds_t* size ) {
    if (*size >= MAX_POLL_FDS) {
        std::cerr << "fds is full" << std::endl;
        return;
    }

    fds[*size].fd = new_sd;
    fds[*size].events = events;
    ++(*size);
}

static void handleEvents( struct pollfd* fds, nfds_t* current_size ) {
    int listen_sd = fds[0].fd;

    // fdsを走査
    for (nfds_t i = 0; i < *current_size; ++i) {
        // イベントが発生していないfdの時
        if (fds[i].revents == 0) {
            continue;
        }

        // 発生したイベントがPOLLIN(読み出し可能イベント)でない時
        if (fds[i].revents != POLLIN) {
            perror("invalid event");
            exit(EXIT_FAILURE);
        }

        // listen_socketでPOLLIN発生の時、新たなclient connectionが来た
        /* 参考 man acceptより
        "A readable event will be delivered when a new connection is attempted"
        */
        if (fds[i].fd == listen_sd) {
            acceptIncomingConnections(listen_sd, fds, current_size);
        } else {
            // fds[i]( != listen_sd)でPOLLIN
            // つまりclientのsocketが読み出し可
            // つまりclientからrequestが来た

            char buf[1024];
            recv(fds[i].fd, buf, 1023, 0);
            echoBack(fds[i].fd, buf);
        }
    }
}

static void acceptIncomingConnections( int listen_sd, struct pollfd* fds, nfds_t* size ) {
    while (true) {
        // clinet connectionをすべてaccept
        int new_sd = accept(listen_sd, NULL, NULL);
        
        if (new_sd < 0) {
            // clientの待ち受けキューが空
            // つまりすべてのconnectionをacceptし終わった時
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            } else {
                // error
                perror("accept");
                exit(EXIT_FAILURE);
            }
        }

        addSocketDiscriptor(new_sd, POLLIN, fds, size);
    }
}

static void echoBack( int client_sd, char* body ) {
    std::cout << "body:" << body << std::endl;
    send(client_sd, body, strlen(body), 0);
}
