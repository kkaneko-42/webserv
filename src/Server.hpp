#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Config.hpp"
#include "./event/NewConnectionEvent.hpp"

#define SYSCALL_ERROR( name ) \
    { \ 
        std::string str = name; \
        throw std::runtime_error(str + ": " + strerror(errno)); \
    }

#define RUNTIME_ERROR( msg ) \
    { \ 
        throw std::runtime_error(std::string("runtime error: ") + msg); \
    }

class Server {
    public:
        Server( void );
        ~Server( void );

        int Init( const std::string& conf_file );
        int Run( void );

        int createSocket( void ) const;
        void bindSocket( int sock, const ServerInfo& info ) const;
        void listenSocket( int sock ) const;
        int addSocketDescripter( int sock, short events );
        std::vector<Event*> waitForEvents( void );
        std::vector<Event*> getRaisedEvents( void );

        const static size_t MAX_POLL_FDS = 200;
        const static int BACKLOG = 1024;
        const static int TIMEOUT = -1; // infinity

    private:
        struct pollfd fds_[MAX_POLL_FDS];
        nfds_t nfds_;
        Config conf_;
};

#endif
