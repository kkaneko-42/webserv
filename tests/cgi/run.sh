#!/bin/bash

# build
make -C ../../ && cp ../../webserv ./;

# execute test
./webserv ./cgi_test.conf > /dev/null 2>&1 & \
for src in `ls -1 ./src/ | grep .py`
do
    python3 ./src/$src
done
pkill -KILL -f webserv;
