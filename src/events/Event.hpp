#ifndef EVENT_HPP
#define EVENT_HPP

#define CLOSED_FD -1

class Event {
    public:
        virtual ~Event( void ) {}
        virtual int handler( void ) = 0;
};

#endif
