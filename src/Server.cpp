#include "Server.hpp"

Server::Server( void )
: conf_(Config())
{
    memset(fds_, 0, sizeof(fds_));
}

Server::~Server( void ) {

}

int Server::Init( const std::string& conf_file ) {
    if (conf_.parse(conf_file)) {
        return 1;
    }

    return 0;
}

int Server::Run( void ) {
    std::vector<ServerInfo> servers_info = this->conf_.getServersInfo();
    for (size_t i = 0; i < servers_info.size(); ++i) {
        try {
            int sock = createSocket();
            this->bindSocket(sock, servers_info[i]);
            this->listenSocket(sock);
            this->addSocketDescriptor(sock, POLLIN);
            this->addListeningDescriptor(sock);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    while (true) {
        std::vector<Event*> events = waitForEvents();
        for (size_t i = 0; i < events.size(); ++i) {
            events[i]->handler();
            delete events[i];
        }
        // delete closed sd
    }

    return 0;
}

int Server::createSocket( void ) const {
    int new_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (new_sock < 0) {
        SYSCALL_ERROR("socket");
    }

    return new_sock;
}

void Server::bindSocket( int sock, const ServerInfo& info ) const {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(info.listen.port);
    addr.sin_addr.s_addr = inet_addr(info.listen.addr.c_str());

    if (bind(sock, (struct sockaddr*)&addr, addr_size) < 0) {
        SYSCALL_ERROR("bind");
    }
}

void Server::listenSocket( int sock ) const {
    if (listen(sock, BACKLOG) < 0) {
        SYSCALL_ERROR("listen");
    }
}

void Server::addSocketDescriptor( int sock, short events ) {
    if (nfds_ == MAX_POLL_FDS) {
        RUNTIME_ERROR("fds is full");
    }

    fds_[nfds_].fd = sock;
    fds_[nfds_].events = events;
    ++nfds_;
}

std::vector<Event*> Server::waitForEvents( void ) {
    std::vector<Event*> events;

    std::cout << "wait Events..." << std::endl;
    int rc = poll(fds_, nfds_, TIMEOUT);
    if (rc == -1) {
        // error
        perror("poll");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        // TIMEOUT
        std::cout << "TIMEOUT" << std::endl;
        exit(EXIT_SUCCESS);
    } else {
        events = getRaisedEvents();
    }

    return events;
}

std::vector<Event*> Server::getRaisedEvents( void ) {
    std::vector<Event*> events;

    for (nfds_t i = 0; i < nfds_; ++i) {
        if (fds_[i].revents == 0) {
            continue;
        }

        if (fds_[i].revents != POLLIN) {
            perror("invalid event");
            exit(EXIT_FAILURE);
        }

        if (isListeningDescriptor(fds_[i].fd)) {
            events.push_back(new NewConnectionEvent(fds_[i].fd, *this));
        } else {
            events.push_back(new RecieveRequestEvent(fds_[i].fd));
        }
    }

    return events;
}

void Server::addListeningDescriptor( int sock ) {
    listening_fds_.insert(sock);
}

bool Server::isListeningDescriptor( int sock ) const {
    return (listening_fds_.count(sock) > 0);
}
