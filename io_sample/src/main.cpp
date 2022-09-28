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
#include <vector>
#include <fstream>
#define MAX_POLL_FDS 200

#define FINISHED_FD -1


void addSocketDiscriptor( int new_sd, short events, struct pollfd* fds, nfds_t* size );
int initServer( const char* ip, int port );
static void handleEvents( struct pollfd* fds, nfds_t* current_size );
static void acceptIncomingConnections( int listen_sd, struct pollfd* fds, nfds_t* size );
static void deleteFinishedFd(struct pollfd* fds, nfds_t& size);
static void echoBack( int &client_sd, char* body );

int main( void ) {
    struct pollfd fds[MAX_POLL_FDS];
    nfds_t current_size = 0;
    int listen_sd = initServer("127.0.0.1", 4242); // Socket Discriptor(sd)
    int on = 1;

    // listen_sdを、non-blockingなfdにする
    // これをしないと、acceptIncomingConnections()ができない
    // なぜなら、待機キューが空の時のacceptが接続を待機してしまう(所謂blocking??)
    fcntl(listen_sd, F_SETFL, O_NONBLOCK);

    // NOTE: 2回連続起動するときに2回目でbindが失敗しないための対策？
    //       ないとき => Address already in use
    if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

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
            deleteFinishedFd(fds, current_size);
        }
    }

    return (0);
}

static void deleteFinishedFd(struct pollfd* fds, nfds_t& size) {
    // 0番目はlisten_sdなのでスキップ
    for (nfds_t i = 1; i < size; ++i) {
        if (fds[i].fd == FINISHED_FD) {
            // TODO: 二重ループ解消できる？
            for (nfds_t j = i; j < size; ++j) {
                fds[j] = fds[j + 1];
            }
            --size;
        }
    }
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

static std::vector<std::string> splitString(std::string &str, std::string delim) {
    std::vector<std::string> vec;

    std::string::size_type prev = 0;
    while (true) {
        std::string::size_type pos = str.find(delim, prev);
        std::cout << pos << std::endl;
        if (pos == std::string::npos)
            break;
        std::string sub = str.substr(prev, pos - prev);
        vec.push_back(sub);
        prev = pos + delim.size();
    }
    vec.push_back(str.substr(prev));
    return vec;
}

template <typename T>
void print(std::vector<T> v) {
    std::cout << "[" << std::endl;
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << "  " << "[" << i << "] " << "@" << v[i] << "@" << std::endl;
    }
    std::cout << "]" << std::endl;
}

// headerは適当に
std::string createResponseHeader( const std::string& content ) {
// TODO: error解消する 
//          curl: (18) transfer closed with 4278 bytes remaining to read
    return (
"HTTP/1.1 200 OK\n"
"Date: Sun, 11 Jan 2004 16:06:23 GMT\n"
"Server: Webserv/1.3.22 (Unix) (Red-Hat/Linux)\n"
"Last-Modified: Sun, 07 Dec 2003 12:34:18 GMT\n"
"ETag: \"1dba6-131b-3fd31e4a\"\n"
"Accept-Ranges: bytes\n"
"Content-Length: " + std::to_string(content.size() - 1) + "\n"
"Connection: close\n"
"Content-Type: text/html\n"
);
}

// index.htmlをstd::stringに変換
std::string loadHtml( const std::string& path ) {
    std::string res = "", buf = "";
    std::ifstream ifs(path.c_str());
    if (!ifs) {
        // TODO:
        std::exit(1);
    }

    while (!ifs.eof()) {
        std::getline(ifs, buf);
        res += buf + "\n";
    }

    return (res);
}

// httpレスポンスに整形
std::string createResponse( const std::string& path ) {
    std::string file = "./html" + path + "/index.html";
    std::string contents = loadHtml(file);
    
    return (
        createResponseHeader(contents)
        + "\n"
        + "<!DOCTYPE html>\n"
        + contents
    );
}

static void echoBack( int &client_sd, char* body ) {
    std::cout << "body:" << body << std::endl;
    
    std::string str = body;
    std::vector<std::string> splitSpace = splitString(str, " ");
    print(splitSpace);

    if (splitSpace[0] == "GET") {
        std::string get_body = createResponse(splitSpace[1]);
        // std::cout << "" << get_body.c_str()
        send(client_sd, get_body.c_str(), get_body.size(), 0);
    } else if (splitSpace[0] == "POST") {
        send(client_sd, body, strlen(body), 0);
    } else if (splitSpace[0] == "DELETE") {
        send(client_sd, body, strlen(body), 0);
    } else {
        std::cerr << "method not found" << std::endl;        
    }
    close(client_sd);
    client_sd = FINISHED_FD;
    // clientにsend
    // send(client_sd, body, strlen(body), 0);
}
