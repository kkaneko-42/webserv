#ifndef EVENT_HPP
#define EVENT_HPP

class Event {
    public:
        virtual ~Event( void ) {}
        virtual int handler( void ) = 0;
};

#endif
