#include "Server.hpp"

Server::Server( void )
: conf_(Config())
{

}

Server::~Server( void ) {
    
}

int Server::Init( const std::string& conf_file ) {
    conf_.Parse(conf_file);
    return 0;
}

int Server::Run( void ) {
    std::vector<ServerInfo> servers_info = this->conf_.GetConf().servers_info;

    for (size_t i = 0; i < servers_info.size(); ++i) {
        std::pair<std::string, int> listen_addr = \
            std::pair<std::string, int>(servers_info[i].listen.ip, servers_info[i].listen.port);

        if (this->isListenedAddress(listen_addr)) {
            continue;
        }

        try {
            int sock = createSocket();
            fcntl(sock, F_SETFL, O_NONBLOCK);
            this->bindSocket(sock, servers_info[i]);
            this->listenSocket(sock);
            this->registerDescriptor(sock, POLLIN);
            this->setListenSdToAddr(sock, listen_addr);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    while (true) {
        // usleep(1000);
        std::vector<Event*> events = waitForEvents();
        for (size_t i = 0; i < events.size(); ++i) {
            events[i]->handler();
            delete events[i];
        }
        this->deleteClosedDescriptor();
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
    int on = 1;
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(info.listen.port);
    addr.sin_addr.s_addr = inet_addr(info.listen.ip.c_str());

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
        SYSCALL_ERROR("setsockopt");
    }

    if (bind(sock, (struct sockaddr*)&addr, addr_size) < 0) {
        SYSCALL_ERROR("bind");
    }
}

void Server::listenSocket( int sock ) const {
    if (listen(sock, BACKLOG) < 0) {
        SYSCALL_ERROR("listen");
    }
}

void Server::registerDescriptor( int sock, short events ) {
    if (fds_.size() == std::numeric_limits<nfds_t>::max()) {
        std::cerr << "fds has already full" << std::endl;
        return;
    }

    try {
        struct pollfd new_sd = (struct pollfd){sock, events, 0};
        fds_.push_back(new_sd);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Server::deleteClosedDescriptor( void ) {
    std::vector<struct pollfd> new_fds;

    std::vector<struct pollfd>::size_type i;
    for (i = 0; i < fds_.size(); ++i) {
        if (fds_[i].fd != CLOSED_FD) {
            try {
                new_fds.push_back(fds_[i]);
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }
    fds_ = new_fds;
}

std::vector<Event*> Server::waitForEvents( void ) {
    std::vector<Event*> events;

    std::cout << "wait Events..." << std::endl;
    std::cout << "nfds: " << fds_.size() << std::endl;
    int rc = poll(&fds_[0], fds_.size(), TIMEOUT);
    if (rc == -1) {
        // error
        perror("poll");
    } else if (rc == 0) {
        // TIMEOUT
        std::cout << "TIMEOUT" << std::endl;
    } else {
        events = this->getRaisedEvents();
    }

    return events;
}

std::vector<Event*> Server::getRaisedEvents( void ) {
    std::vector<Event*> events;
    short revents;

    std::vector<struct pollfd>::size_type i;
    for (i = 0; i < fds_.size(); ++i) {
        revents = fds_[i].revents;
        std::cout << "i: " << i << std::endl;
        std::cout << "revents: " << revents << std::endl;

        if (revents == 0) {
            continue;
        }

        if ((revents & (POLLIN | POLLOUT)) == 0) {
            perror("invalid event");
            exit(EXIT_FAILURE);
        }

        if (this->isListeningDescriptor(fds_[i].fd)) {
            events.push_back(new NewConnectionEvent(fds_[i].fd, *this));
        } else if ((revents & POLLOUT) != 0 && resp_cache_.count(fds_[i].fd) != 0) {
            events.push_back(new SendResponseEvent(fds_[i].fd, *this));
        } else if ((revents & POLLIN) != 0) {
            events.push_back(new RecieveRequestEvent(fds_[i].fd, *this));
        }
    }

    return events;
}

bool Server::isListeningDescriptor( int sock ) const {
    return (listen_sd_to_addr_.count(sock) > 0);
}

void Server::cacheResponse( int client_sd, const HttpResponse& resp ) {
    resp_cache_.insert(std::make_pair(client_sd, resp));
}

void Server::removeCachedResponse( int client_sd ) {
    resp_cache_.erase(client_sd);
}

HttpResponse Server::getCachedResponse( int client_sd ) {
    return resp_cache_[client_sd];
}

bool Server::isListenedAddress( const std::pair<std::string, int>& addr ) {
    std::map< int, std::pair<std::string, int> >::iterator it;

    for (it = listen_sd_to_addr_.begin(); it != listen_sd_to_addr_.end(); ++it) {        
        std::pair<std::string, int> listened_addr = (*it).second;
        if (addr == listened_addr) {
            return true;
        }
    }

    return false;
}

const std::pair<std::string, int>& Server::getListenSdToAddr( int sd ) {
    return listen_sd_to_addr_[sd];
};

const std::pair<std::string, int>& Server::getClientSdToAddr( int sd ) {
    return client_sd_to_addr_[sd];
};

void Server::setListenSdToAddr( int sd, std::pair<std::string, int> addr ) {
    listen_sd_to_addr_[sd] = addr;
}

void Server::setClientSdToAddr( int sd, std::pair<std::string, int> addr ) {
    client_sd_to_addr_[sd] = addr;
}

void Server::removeClientSdToAddr( int sd ) {
    client_sd_to_addr_.erase(sd);
}
